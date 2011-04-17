/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "piecetablebytearraymodel_p.h"
#include "piecetablebytearraymodel.h"

// lib
#include <addressrangelist.h>


namespace Okteta
{

static const int InvalidVersionIndex = -1;

PieceTableByteArrayModelPrivate::PieceTableByteArrayModelPrivate( PieceTableByteArrayModel* parent, const QByteArray& data )
  : p( parent ),
    mReadOnly( false ),
    mInitialData( data )
{
    mPieceTable.init( data.size() );
}

PieceTableByteArrayModelPrivate::PieceTableByteArrayModelPrivate( PieceTableByteArrayModel *parent, int size, Byte fillByte )
  : p( parent ),
    mReadOnly( false ),
    mInitialData( size, fillByte )
{
    mPieceTable.init( size );
}


// TODO: getStorageData needs some caching, optimize for successive access
Byte PieceTableByteArrayModelPrivate::byte( Address offset ) const
{
    int storageId;
    Address storageOffset;
    mPieceTable.getStorageData( &storageId, &storageOffset, offset );

    const Byte result = ( storageId == KPieceTable::Piece::OriginalStorage ) ?
                        mInitialData[storageOffset] :
                        mChangesDataStorage[storageOffset];
    return result;
}

void PieceTableByteArrayModelPrivate::setData( const QByteArray& data )
{
    const int oldSize = mPieceTable.size();
    const bool wasModifiedBefore = isModified();
    const QList<Bookmark> bookmarks = mBookmarks.list();

    mInitialData = data;
    mPieceTable.init( data.size() );
    mChangesDataStorage.clear();
    mBookmarks.clear();

    // TODO: how to tell this to the synchronizer?
    emit p->contentsChanged( ArrayChangeMetricsList::oneReplacement(0,oldSize,data.size()) );
    if( wasModifiedBefore ) emit p->modifiedChanged( false );
    if( !bookmarks.empty() ) emit p->bookmarksRemoved( bookmarks );
    emit p->headVersionChanged( 0 );
    emit p->revertedToVersionIndex( 0 );
}

void PieceTableByteArrayModelPrivate::setByte( Address offset, Byte byte )
{
    if( mReadOnly )
        return;

    const bool wasModifiedBefore = isModified();
    const bool oldVersionIndex = versionIndex();

    Address storageOffset;
    const bool newChange = mPieceTable.replaceOne( offset, &storageOffset );
    mChangesDataStorage.append( storageOffset, byte );

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement( offset, 1, 1 );
    const ByteArrayChange modification( metrics, mChangesDataStorage.data(storageOffset,1) );
    QList<Okteta::ByteArrayChange> modificationsList;
    modificationsList.append( modification );

    emit p->contentsChanged( ArrayChangeMetricsList(metrics) );
    emit p->changesDone( modificationsList, oldVersionIndex, versionIndex() );
    if( ! wasModifiedBefore ) emit p->modifiedChanged( true );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
}


Size PieceTableByteArrayModelPrivate::insert( Address offset, const Byte* insertData, int insertLength )
{
    // correct parameters
    const int oldSize = mPieceTable.size();
    if( offset > oldSize )
        offset = oldSize;
    // check all parameters
    if( mReadOnly || insertLength == 0 )
        return 0;

    beginChanges();

    doInsertChange( offset, insertData, insertLength );

    endChanges();

    return insertLength;
}


//TODO: is anyone interested in the removed data? so we need a signal beforeRemoving(section)?
Size PieceTableByteArrayModelPrivate::remove( const AddressRange& _removeRange )
{
    AddressRange removeRange( _removeRange );
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeRange.restrictEndTo( oldSize-1 );
    // check parameters
    if( removeRange.start() >= oldSize || removeRange.width() == 0 )
        return 0;

    beginChanges();

    doRemoveChange( removeRange );

    endChanges();

    return removeRange.width();
}

Size PieceTableByteArrayModelPrivate::replace( const AddressRange& _removeRange, const Byte* insertData, int insertLength )
{
    AddressRange removeRange( _removeRange );
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeRange.restrictEndTo( oldSize-1 );
    // check parameters
    if( (removeRange.startsBehind( oldSize-1 ) && removeRange.width()>0)
        || (removeRange.width()==0 && insertLength==0) )
        return 0;

    beginChanges();

    doReplaceChange( removeRange, insertData, insertLength );

    endChanges();

    return insertLength;
}


bool PieceTableByteArrayModelPrivate::swap( Address firstStart, const AddressRange& _secondRange )
{
    AddressRange secondRange( _secondRange );
    // correct parameters
    secondRange.restrictEndTo( mPieceTable.size()-1 );
    // check parameters
    if( secondRange.start() >= mPieceTable.size() || secondRange.width() <= 0
        || firstStart > mPieceTable.size() || secondRange.start() == firstStart )
        return false;

    beginChanges();

    doSwapChange( firstStart, secondRange );

    endChanges();

    return true;
}

Size PieceTableByteArrayModelPrivate::fill( Byte fillByte, Address offset, Size fillLength )
{
    // correct parameters
    const int lengthToEnd = mPieceTable.size() - offset;
    if( fillLength < 0 )
        fillLength = lengthToEnd;
    const int filledLength = ( fillLength < lengthToEnd ) ? fillLength : lengthToEnd;
    // check parameters
    const bool nothingToFill = ( (int)offset >= mPieceTable.size() );
    if( nothingToFill )
        return 0;

    beginChanges();

    doFillChange( offset, filledLength, fillByte, fillLength );

    endChanges();

    return fillLength;
}

void PieceTableByteArrayModelPrivate::revertToVersionByIndex( int versionIndex )
{
    ArrayChangeMetricsList changeList;
    AddressRangeList changedRanges;

    const bool oldModified = isModified();

    const bool anyChanges =
        mPieceTable.revertBeforeChange( versionIndex, &changedRanges, &changeList ); // TODO: changedRanges no longer used

    if( !anyChanges )
        return;

    const bool newModified = isModified();
    const bool isModificationChanged = ( oldModified != newModified );

//TODO: what about the bookmarks? They need version support, too.
// Modell of the bookmarks. But shouldn't they be independent?

    emit p->contentsChanged( changeList );
    if( isModificationChanged ) emit p->modifiedChanged( newModified );
    emit p->revertedToVersionIndex( versionIndex );
}

void PieceTableByteArrayModelPrivate::openGroupedChange( const QString& description )
{
    const bool isModifiedBefore = isModified();
    mBeforeGroupedChangeVersionIndex = mPieceTable.appliedChangesCount();
    mPieceTable.openGroupedChange( description );

    if( ! isModifiedBefore ) emit p->modifiedChanged( true );
    emit p->headVersionChanged( mPieceTable.changesCount() );
}

void PieceTableByteArrayModelPrivate::cancelGroupedChange()
{
    if( mBeforeGroupedChangeVersionIndex != InvalidVersionIndex )
        revertToVersionByIndex( mBeforeGroupedChangeVersionIndex );
}

void PieceTableByteArrayModelPrivate::closeGroupedChange( const QString& description )
{
    mPieceTable.closeGroupedChange( description );
    mBeforeGroupedChangeVersionIndex = InvalidVersionIndex;

    emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
}

QList<ByteArrayChange> PieceTableByteArrayModelPrivate::changes( int firstVersionIndex, int lastVersionIndex ) const
{
    QList<ByteArrayChange> result;

    for( int i=firstVersionIndex; i<lastVersionIndex; ++i )
    {
        ArrayChangeMetrics metrics;
        Address storageOffset;
        mPieceTable.getChangeData( &metrics, &storageOffset, i );

        QByteArray data;
        if( metrics.type() == ArrayChangeMetrics::Replacement )
            data = mChangesDataStorage.data( storageOffset, metrics.insertLength() );
        result.append( ByteArrayChange(metrics,data) );
    }

    return result;
}

void PieceTableByteArrayModelPrivate::doChanges( const QList<ByteArrayChange>& changes,
                                                 int oldVersionIndex, int newVersionIndex )
{
// kDebug() << this<<" is at "<<versionIndex()<<", should from "<<oldVersionIndex<<" to "<<newVersionIndex;
    // changes already done? TODO: should this check be here?
    if( newVersionIndex == versionIndex() )
        return;

    // collision! TODO: what to do?
    if( oldVersionIndex != versionIndex() )
        return;

    beginChanges();

    foreach( const ByteArrayChange& change, changes )
    {
        const ArrayChangeMetrics& metrics = change.metrics();
        switch( metrics.type() )
        {
        case ArrayChangeMetrics::Replacement:
        {
            const int oldSize = mPieceTable.size();
            const AddressRange removeRange = AddressRange::fromWidth( metrics.offset(), metrics.removeLength() );
            // check parameters
            if( removeRange.startsBehind(oldSize-1) && (removeRange.width()>0) ) 
            {
                // something is not matching
                ; // TODO: set flag to undo all changes
            }

            const QByteArray& insertData = change.data();
            doReplaceChange( removeRange, reinterpret_cast<const Byte*>(insertData.constData()), insertData.size() );
            break;
        }
        case ArrayChangeMetrics::Swapping:
        {
            const AddressRange secondRange( metrics.secondStart(), metrics.secondEnd() );
            doSwapChange( metrics.offset(), secondRange );
            break;
        }
        default:
            ;
        }
    }

    // not the same versioning? TODO: where and how to define the version id?
//     if( newVersionIndex != versionIndex() )
//         return;
// kDebug()<<this<<" is now at "<<versionIndex();

    endChanges();
}

void PieceTableByteArrayModelPrivate::beginChanges()
{
    mBeforeChangesVersionIndex = versionIndex();
    mBeforeChangesModified = isModified();
}

void PieceTableByteArrayModelPrivate::endChanges()
{
    const int currentVersionIndex = versionIndex();
    const bool newChange = ( mBeforeChangesVersionIndex != currentVersionIndex );
    const bool currentIsModified = isModified();
    const bool modifiedChanged = ( mBeforeChangesModified != currentIsModified );

    emit p->contentsChanged( mChangeMetrics );
    emit p->changesDone( mChanges, mBeforeChangesVersionIndex, currentVersionIndex );
    if( mBookmarksModified ) emit p->bookmarksModified( true );
    if( modifiedChanged ) emit p->modifiedChanged( currentIsModified );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );

    // clean
    mChangeMetrics.clear();
    mChanges.clear();
    mBookmarksModified = false;
}

void PieceTableByteArrayModelPrivate::doInsertChange( Address offset,
                                                      const Byte* insertData, int insertLength )
{
    Address storageOffset;
    mPieceTable.insert( offset, insertLength, &storageOffset );
    mChangesDataStorage.append( storageOffset, reinterpret_cast<const char*>(insertData), insertLength );

    mBookmarksModified |= mBookmarks.adjustToReplaced( offset, 0, insertLength );

    const ArrayChangeMetrics metrics = ArrayChangeMetrics::asReplacement( offset, 0, insertLength );
    const ByteArrayChange change( metrics, mChangesDataStorage.data(storageOffset,insertLength) );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}

void PieceTableByteArrayModelPrivate::doRemoveChange( const AddressRange& removeRange )
{
    mPieceTable.remove( removeRange );

    mBookmarksModified |= mBookmarks.adjustToReplaced( removeRange.start(), removeRange.width(), 0 );

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement( removeRange.start(), removeRange.width(), 0 );
    const ByteArrayChange change( metrics );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}

void PieceTableByteArrayModelPrivate::doReplaceChange( const AddressRange& removeRange,
                                                       const Byte* insertData, int insertLength )
{
    Address storageOffset;
    mPieceTable.replace( removeRange, insertLength, &storageOffset );
    mChangesDataStorage.append( storageOffset, reinterpret_cast<const char*>(insertData), insertLength );

    mBookmarksModified |= mBookmarks.adjustToReplaced( removeRange.start(), removeRange.width(), insertLength );

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement( removeRange.start(), removeRange.width(), insertLength );
    const ByteArrayChange change( metrics, mChangesDataStorage.data(storageOffset,insertLength) );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}

void PieceTableByteArrayModelPrivate::doSwapChange( Address firstStart, const AddressRange& secondRange )
{
    mPieceTable.swap( firstStart, secondRange );

    mBookmarksModified |= mBookmarks.adjustToSwapped( firstStart, secondRange.start(),secondRange.width() );

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asSwapping( firstStart, secondRange.start(), secondRange.width() );
    const ByteArrayChange change( metrics );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}

void PieceTableByteArrayModelPrivate::doFillChange( Address offset, Size filledLength,
                                                    Byte fillByte, int fillLength )
{
    Address storageOffset;
    mPieceTable.replace( offset, filledLength, fillLength, &storageOffset );

    mChangesDataStorage.appendFill( storageOffset, fillByte, fillLength );

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement( offset, fillLength, fillLength );
    const ByteArrayChange change( metrics );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}


PieceTableByteArrayModelPrivate::~PieceTableByteArrayModelPrivate()
{
}

}
