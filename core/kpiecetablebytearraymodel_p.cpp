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

#include "kpiecetablebytearraymodel_p.h"
#include "kpiecetablebytearraymodel.h"

// lib
#include <arraychangemetricslist.h>

#include <KDebug>

namespace KHECore
{

static const int InvalidVersionIndex = -1;

KPieceTableByteArrayModel::Private::Private( KPieceTableByteArrayModel *parent, const char *data, unsigned int size,
                                             bool careForMemory )
  : p( parent ),
   mReadOnly( false ),
   mAutoDelete( true ),
   mBeforeGroupedChangeVersionIndex( InvalidVersionIndex )
{
    if( data == 0 )
        size = 0;
    if( careForMemory )
        mData = data;
    else
    {
        char *dataCopy = new char[size];
        memcpy( dataCopy, data, size );
        mData = dataCopy;
    }
    mPieceTable.init( size );
}

KPieceTableByteArrayModel::Private::Private( KPieceTableByteArrayModel *parent, unsigned int size, char fillByte )
  : p( parent ),
   mReadOnly( false ),
   mAutoDelete( true )
{
    char *data = new char[size];
    memset( data, fillByte, size );
    mData = data;
    mPieceTable.init( size );
}


// TODO: getStorageData needs some caching, optimise for successive access
char KPieceTableByteArrayModel::Private::datum( unsigned int offset ) const
{
    int storageId;
    int storageOffset;
    mPieceTable.getStorageData( &storageId, &storageOffset, offset );

    const char result = ( storageId == KPieceTable::Piece::OriginalStorage ) ?
                        mData[storageOffset] :
                        mChangeByteArray[storageOffset];
    return result;
}

void KPieceTableByteArrayModel::Private::setData( const char *data, unsigned int size, bool careForMemory )
{
    if( mAutoDelete )
        delete mData;
    const unsigned int oldSize = mPieceTable.size();
    const bool wasModifiedBefore = isModified();
    const QList<KHECore::KBookmark> bookmarks = mBookmarks.list();

    if( data == 0 )
        size = 0;

    if( careForMemory )
        mData = data;
    else
    {
        char *dataCopy = new char[size];
        memcpy( dataCopy, data, size );
        mData = dataCopy;
    }
    mPieceTable.init( size );
    mChangeByteArray.clear();
    mBookmarks.clear();

    emit p->contentsChanged( KHE::ArrayChangeMetricsList::oneReplacement(0,oldSize,size) );
    emit p->contentsChanged( 0, (size>oldSize?size:oldSize)-1 );
    if( wasModifiedBefore ) emit p->modificationChanged( false );
    if( !bookmarks.empty() ) emit p->bookmarksRemoved( bookmarks );
    emit p->headVersionChanged( mPieceTable.changesCount() );
}

void KPieceTableByteArrayModel::Private::setDatum( unsigned int offset, const char byte )
{
    if( mReadOnly )
        return;

    const bool wasModifiedBefore = isModified();

    int storageOffset;
    const bool newChange = mPieceTable.replaceOne( offset, &storageOffset );
    mChangeByteArray.resize( storageOffset );
    mChangeByteArray.append( byte );

    emit p->contentsChanged( KHE::ArrayChangeMetricsList::oneReplacement(offset,1,1) );
    emit p->contentsChanged( offset, offset );
    if( !wasModifiedBefore ) emit p->modificationChanged( true );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
}


static inline void appendToByteArray( QByteArray *byteArray, int oldSize, const char *data, int dataLength )
{
    byteArray->resize( oldSize + dataLength );

    memcpy( byteArray->data()+oldSize, data, dataLength );
}

int KPieceTableByteArrayModel::Private::insert( int offset, const char *insertData, int insertLength )
{
    // correct parameters
    const int oldSize = mPieceTable.size();
    if( offset > oldSize )
        offset = oldSize;
    // check all parameters
    if( mReadOnly || insertLength == 0 )
        return 0;

    const bool wasModifiedBefore = isModified();

    int storageOffset;
    const bool newChange = mPieceTable.insert( offset, insertLength, &storageOffset );
    appendToByteArray( &mChangeByteArray, storageOffset, insertData, insertLength );

    const bool bookmarksModified = mBookmarks.adjustToReplaced( offset, 0, insertLength );

    emit p->contentsChanged( KHE::ArrayChangeMetricsList::oneReplacement(offset,0,insertLength) );
    emit p->contentsChanged( offset, mPieceTable.size()-1 );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    if( !wasModifiedBefore ) emit p->modificationChanged( true );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
    return insertLength;
}


//TODO: is anyone interested in the removed data? so we need a signal beforeRemoving(section)?
int KPieceTableByteArrayModel::Private::remove( const KSection &_removeSection )
{
    KSection removeSection( _removeSection );
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeSection.restrictEndTo( oldSize-1 );
    // check parameters
    if( removeSection.start() >= oldSize || removeSection.width() == 0 )
        return 0;

    const bool wasModifiedBefore = isModified();

    const bool newChange = mPieceTable.remove( removeSection );

    const bool bookmarksModified = mBookmarks.adjustToReplaced( removeSection.start(), removeSection.width(), 0 );

    emit p->contentsChanged( KHE::ArrayChangeMetricsList::oneReplacement(removeSection.start(),removeSection.width(),0) );
    emit p->contentsChanged( removeSection.start(), oldSize-1 );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    if( !wasModifiedBefore ) emit p->modificationChanged( true );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
    return removeSection.width();
}


unsigned int KPieceTableByteArrayModel::Private::replace( const KSection &r, const char *insertData, unsigned int insertLength )
{
    KSection removeSection( r );
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeSection.restrictEndTo( oldSize-1 );
    // check parameters
    if( removeSection.startsBehind( oldSize-1 ) || (removeSection.width()==0 && insertLength==0) )
        return 0;

    const bool wasModifiedBefore = isModified();

    int storageOffset;
    const bool newChange = mPieceTable.replace( removeSection, insertLength, &storageOffset );
    appendToByteArray( &mChangeByteArray, storageOffset, insertData, insertLength );

    const bool bookmarksModified = mBookmarks.adjustToReplaced( removeSection.start(), removeSection.width(), insertLength );

    const int sizeDiff = insertLength - removeSection.width();
    const int lastChanged = ( sizeDiff == 0 ) ? removeSection.end() :
                            ( sizeDiff > 0 ) ?  mPieceTable.size() - 1 :
                                                oldSize - 1;

    emit p->contentsChanged(
        KHE::ArrayChangeMetricsList::oneReplacement(removeSection.start(),removeSection.width(),insertLength) );
    emit p->contentsChanged( removeSection.start(), lastChanged );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    if( !wasModifiedBefore ) emit p->modificationChanged( true );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
    return insertLength;
}


bool KPieceTableByteArrayModel::Private::swap( int firstStart, const KSection &second )
{
    KSection secondSection( second );
    // correct parameters
    secondSection.restrictEndTo( mPieceTable.size()-1 );
    // check parameters
    if( secondSection.start() >= mPieceTable.size() || secondSection.width() <= 0
        || firstStart > mPieceTable.size() || secondSection.start() == firstStart )
        return false;

    const bool wasModifiedBefore = isModified();

    mPieceTable.swap( firstStart, secondSection );

    const bool bookmarksModified = mBookmarks.adjustToSwapped( firstStart, secondSection.start(),secondSection.width() );

    emit p->contentsChanged(
        KHE::ArrayChangeMetricsList::oneSwapping(firstStart,secondSection.start(),secondSection.width()) );
    emit p->contentsChanged( firstStart, secondSection.end() );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    if( !wasModifiedBefore ) emit p->modificationChanged( true );
    emit p->headVersionChanged( mPieceTable.changesCount() );
    return true;
}


int KPieceTableByteArrayModel::Private::fill( const char fillByte, unsigned int offset, int fillLength )
{
    // correct parameters
    const int lengthToEnd = mPieceTable.size() - offset;
    if( fillLength < 0 )
        fillLength = lengthToEnd;
    // check parameters
    const bool nothingToFill = ( (int)offset >= mPieceTable.size() );
    if( nothingToFill )
        return 0;

    const bool wasModifiedBefore = isModified();

    const int filledLength = ( lengthToEnd > fillLength ) ? fillLength : lengthToEnd;
    int oldChangeByteArraySize;
    mPieceTable.replace( offset, filledLength, fillLength, &oldChangeByteArraySize );

    mChangeByteArray.resize( oldChangeByteArraySize + fillLength );
    memset( mChangeByteArray.data()+oldChangeByteArraySize, fillByte, fillLength );

    emit p->contentsChanged( KHE::ArrayChangeMetricsList::oneReplacement(offset,fillLength,fillLength) );
    emit p->contentsChanged( offset, offset+fillLength-1 );
    if( !wasModifiedBefore ) emit p->modificationChanged( true );
    emit p->headVersionChanged( mPieceTable.changesCount() );
    return fillLength;
}

void KPieceTableByteArrayModel::Private::revertToVersionByIndex( int versionIndex )
{
    KHE::ArrayChangeMetricsList changeList;
    KHE::KSectionList changedRanges;

    const bool oldModified = isModified();

    const bool anyChanges =
        mPieceTable.revertBeforeChange( versionIndex, &changedRanges, &changeList );

    if( !anyChanges )
        return;

    const bool newModified = isModified();
    const bool isModificationChanged = ( oldModified != newModified );

//TODO: what about the bookmarks? They need version support, too.
// Modell of the bookmarks. But shouldn't they be independent?

    emit p->contentsChanged( changeList );
    emit p->contentsChanged( changedRanges );
    if( isModificationChanged ) emit p->modificationChanged( newModified );
    emit p->revertedToVersionIndex( versionIndex );
}

void KPieceTableByteArrayModel::Private::openGroupedChange( const QString &description )
{
    mBeforeGroupedChangeVersionIndex = mPieceTable.appliedChangesCount();
    mPieceTable.openGroupedChange( description );

    emit p->headVersionChanged( mPieceTable.changesCount() );
}

void KPieceTableByteArrayModel::Private::cancelGroupedChange()
{
    if( mBeforeGroupedChangeVersionIndex != InvalidVersionIndex )
        revertToVersionByIndex( mBeforeGroupedChangeVersionIndex );
}

void KPieceTableByteArrayModel::Private::closeGroupedChange( const QString &description )
{
    mPieceTable.closeGroupedChange( description );
    mBeforeGroupedChangeVersionIndex = InvalidVersionIndex;

    emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
}

KPieceTableByteArrayModel::Private::~Private()
{
    if( mAutoDelete )
        delete [] mData;
}

}
