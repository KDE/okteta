/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include <KDebug>

namespace KHECore
{

static const int InvalidVersionIndex = -1;

PieceTableByteArrayModel::Private::Private( PieceTableByteArrayModel *parent, const char *data, unsigned int size,
                                             bool careForMemory )
  : p( parent ),
   mReadOnly( false ),
   mAutoDelete( true ),
   mBeforeGroupedChangeVersionIndex( InvalidVersionIndex ),
   mBookmarksModified( false )
{
    if( data == 0 )
        size = 0;
    if( careForMemory )
        mInitialData = data;
    else
    {
        char *dataCopy = new char[size];
        memcpy( dataCopy, data, size );
        mInitialData = dataCopy;
    }
    mInitialSize = size;
    mPieceTable.init( size );
}

PieceTableByteArrayModel::Private::Private( PieceTableByteArrayModel *parent, unsigned int size, char fillByte )
  : p( parent ),
   mReadOnly( false ),
   mAutoDelete( true )
{
    char *data = new char[size];
    memset( data, fillByte, size );
    mInitialData = data;
    mInitialSize = size;
    mPieceTable.init( size );
}


// TODO: getStorageData needs some caching, optimize for successive access
char PieceTableByteArrayModel::Private::datum( unsigned int offset ) const
{
    int storageId;
    int storageOffset;
    mPieceTable.getStorageData( &storageId, &storageOffset, offset );

    const char result = ( storageId == KPieceTable::Piece::OriginalStorage ) ?
                        mInitialData[storageOffset] :
                        mChangesDataStorage[storageOffset];
    return result;
}

void PieceTableByteArrayModel::Private::setData( const char *data, unsigned int size, bool careForMemory )
{
    if( mAutoDelete )
        delete [] mInitialData;
    const unsigned int oldSize = mPieceTable.size();
    const bool wasModifiedBefore = isModified();
    const QList<KHECore::Bookmark> bookmarks = mBookmarks.list();

    if( data == 0 )
        size = 0;

    if( careForMemory )
        mInitialData = data;
    else
    {
        char *dataCopy = new char[size];
        memcpy( dataCopy, data, size );
        mInitialData = dataCopy;
    }
    mPieceTable.init( size );
    mChangesDataStorage.clear();
    mBookmarks.clear();

    // TODO: how to tell this to the synchronizer?
    emit p->contentsChanged( KHE::ArrayChangeMetricsList::oneReplacement(0,oldSize,size) );
    if( wasModifiedBefore ) emit p->modificationChanged( false );
    if( !bookmarks.empty() ) emit p->bookmarksRemoved( bookmarks );
    emit p->headVersionChanged( mPieceTable.changesCount() );
}

void PieceTableByteArrayModel::Private::setDatum( unsigned int offset, const char byte )
{
    if( mReadOnly )
        return;

    const bool wasModifiedBefore = isModified();
    const bool oldVersionIndex = versionIndex();

    int storageOffset;
    const bool newChange = mPieceTable.replaceOne( offset, &storageOffset );
    mChangesDataStorage.append( storageOffset, byte );

    const KHE::ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement( offset, 1, 1 );
    const ByteArrayChange modification( metrics, mChangesDataStorage.data(storageOffset,1) );
    QList<KHECore::ByteArrayChange> modificationsList;
    modificationsList.append( modification );

    emit p->contentsChanged( KHE::ArrayChangeMetricsList(metrics) );
    emit p->changesDone( modificationsList, oldVersionIndex, versionIndex() );
    if( !wasModifiedBefore ) emit p->modificationChanged( true );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
}


int PieceTableByteArrayModel::Private::insert( int offset, const char *insertData, int insertLength )
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
int PieceTableByteArrayModel::Private::remove( const Section &_removeSection )
{
    Section removeSection( _removeSection );
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeSection.restrictEndTo( oldSize-1 );
    // check parameters
    if( removeSection.start() >= oldSize || removeSection.width() == 0 )
        return 0;

    beginChanges();

    doRemoveChange( removeSection );

    endChanges();

    return removeSection.width();
}

unsigned int PieceTableByteArrayModel::Private::replace( const Section &r, const char *insertData, unsigned int insertLength )
{
    Section removeSection( r );
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeSection.restrictEndTo( oldSize-1 );
    // check parameters
    if( (removeSection.startsBehind( oldSize-1 ) && removeSection.width()>0)
        || (removeSection.width()==0 && insertLength==0) )
        return 0;

    beginChanges();

    doReplaceChange( removeSection, insertData, insertLength );

    endChanges();

    return insertLength;
}


bool PieceTableByteArrayModel::Private::swap( int firstStart, const Section &second )
{
    Section secondSection( second );
    // correct parameters
    secondSection.restrictEndTo( mPieceTable.size()-1 );
    // check parameters
    if( secondSection.start() >= mPieceTable.size() || secondSection.width() <= 0
        || firstStart > mPieceTable.size() || secondSection.start() == firstStart )
        return false;

    beginChanges();

    doSwapChange( firstStart, secondSection );

    endChanges();

    return true;
}

int PieceTableByteArrayModel::Private::fill( const char fillByte, unsigned int offset, int fillLength )
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

void PieceTableByteArrayModel::Private::revertToVersionByIndex( int versionIndex )
{
    KHE::ArrayChangeMetricsList changeList;
    KHE::SectionList changedRanges;

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
    if( isModificationChanged ) emit p->modificationChanged( newModified );
    emit p->revertedToVersionIndex( versionIndex );
}

void PieceTableByteArrayModel::Private::openGroupedChange( const QString &description )
{
    const bool isModifiedBefore = isModified();
    mBeforeGroupedChangeVersionIndex = mPieceTable.appliedChangesCount();
    mPieceTable.openGroupedChange( description );

    if( ! isModifiedBefore ) emit p->modificationChanged( true );
    emit p->headVersionChanged( mPieceTable.changesCount() );
}

void PieceTableByteArrayModel::Private::cancelGroupedChange()
{
    if( mBeforeGroupedChangeVersionIndex != InvalidVersionIndex )
        revertToVersionByIndex( mBeforeGroupedChangeVersionIndex );
}

void PieceTableByteArrayModel::Private::closeGroupedChange( const QString &description )
{
    mPieceTable.closeGroupedChange( description );
    mBeforeGroupedChangeVersionIndex = InvalidVersionIndex;

    emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
}

QList<ByteArrayChange> PieceTableByteArrayModel::Private::changes( int firstVersionIndex, int lastVersionIndex ) const
{
    QList<ByteArrayChange> result;

    for( int i=firstVersionIndex; i<lastVersionIndex; ++i )
    {
        ArrayChangeMetrics metrics;
        int storageOffset;
        mPieceTable.getChangeData( &metrics, &storageOffset, i );

        QByteArray data;
        if( metrics.type() == ArrayChangeMetrics::Replacement )
            data = mChangesDataStorage.data( storageOffset, metrics.insertLength() );
        result.append( ByteArrayChange(metrics,data) );
    }

    return result;
}

QByteArray PieceTableByteArrayModel::Private::initialData() const
{
    return QByteArray( mInitialData, mInitialSize );
}

void PieceTableByteArrayModel::Private::doChanges( const QList<KHECore::ByteArrayChange>& changes,
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
        const KHE::ArrayChangeMetrics& metrics = change.metrics();
        switch( metrics.type() )
        {
        case ArrayChangeMetrics::Replacement:
        {
            const int oldSize = mPieceTable.size();
            const Section removeSection = Section::fromWidth( metrics.offset(), metrics.removeLength() );
            // check parameters
            if( removeSection.startsBehind(oldSize-1) && (removeSection.width()>0) ) 
            {
                // something is not matching
                ; // TODO: set flag to undo all changes
            }

            const QByteArray& insertData = change.data();
            doReplaceChange( removeSection, insertData.data(), insertData.size() );
            break;
        }
        case ArrayChangeMetrics::Swapping:
        {
            const Section secondSection( metrics.secondStart(), metrics.secondEnd() );
            doSwapChange( metrics.offset(), secondSection );
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

void PieceTableByteArrayModel::Private::beginChanges()
{
    mBeforeChangesVersionIndex = versionIndex();
    mBeforeChangesModified = isModified();
}

void PieceTableByteArrayModel::Private::endChanges()
{
    const int currentVersionIndex = versionIndex();
    const bool newChange = ( mBeforeChangesVersionIndex != currentVersionIndex );
    const bool currentIsModified = isModified();
    const bool modificationChanged = ( mBeforeChangesModified != currentIsModified );

    emit p->contentsChanged( mChangeMetrics );
    emit p->changesDone( mChanges, mBeforeChangesVersionIndex, currentVersionIndex );
    if( mBookmarksModified ) emit p->bookmarksModified( true );
    if( modificationChanged ) emit p->modificationChanged( currentIsModified );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );

    // clean
    mChangeMetrics.clear();
    mChanges.clear();
    mBookmarksModified = false;
}

void PieceTableByteArrayModel::Private::doInsertChange( unsigned int offset,
                                                         const char* insertData, unsigned int insertLength )
{
    int storageOffset;
    mPieceTable.insert( offset, insertLength, &storageOffset );
    mChangesDataStorage.append( storageOffset, insertData, insertLength );

    mBookmarksModified |= mBookmarks.adjustToReplaced( offset, 0, insertLength );

    const KHE::ArrayChangeMetrics metrics = ArrayChangeMetrics::asReplacement( offset, 0, insertLength );
    const ByteArrayChange change( metrics, mChangesDataStorage.data(storageOffset,insertLength) );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}

void PieceTableByteArrayModel::Private::doRemoveChange( const Section& removeSection )
{
    mPieceTable.remove( removeSection );

    mBookmarksModified |= mBookmarks.adjustToReplaced( removeSection.start(), removeSection.width(), 0 );

    const KHE::ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement( removeSection.start(), removeSection.width(), 0 );
    const ByteArrayChange change( metrics );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}

void PieceTableByteArrayModel::Private::doReplaceChange( const Section& removeSection,
                                                          const char* insertData, unsigned int insertLength )
{
    int storageOffset;
    mPieceTable.replace( removeSection, insertLength, &storageOffset );
    mChangesDataStorage.append( storageOffset, insertData, insertLength );

    mBookmarksModified |= mBookmarks.adjustToReplaced( removeSection.start(), removeSection.width(), insertLength );

    const KHE::ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement( removeSection.start(), removeSection.width(), insertLength );
    const ByteArrayChange change( metrics, mChangesDataStorage.data(storageOffset,insertLength) );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}

void PieceTableByteArrayModel::Private::doSwapChange( int firstStart, const Section& secondSection )
{
    mPieceTable.swap( firstStart, secondSection );

    mBookmarksModified |= mBookmarks.adjustToSwapped( firstStart, secondSection.start(),secondSection.width() );

    const KHE::ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asSwapping( firstStart, secondSection.start(), secondSection.width() );
    const ByteArrayChange change( metrics );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}

void PieceTableByteArrayModel::Private::doFillChange( unsigned int offset, unsigned int filledLength,
                                                       const char fillByte, unsigned int fillLength )
{
    int storageOffset;
    mPieceTable.replace( offset, filledLength, fillLength, &storageOffset );

    mChangesDataStorage.appendFill( storageOffset, fillByte, fillLength );

    const KHE::ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement( offset, fillLength, fillLength );
    const ByteArrayChange change( metrics );

    mChangeMetrics.append( metrics );
    mChanges.append( change );
}


PieceTableByteArrayModel::Private::~Private()
{
    if( mAutoDelete )
        delete [] mInitialData;
}

}
