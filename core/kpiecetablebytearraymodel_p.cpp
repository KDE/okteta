/***************************************************************************
                          kpiecetablebytearraymodel_p.cpp  -  description
                             -------------------
    begin                : Fri Jan 18 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "kpiecetablebytearraymodel_p.h"
#include "kpiecetablebytearraymodel.h"

// C
#include <string.h>


namespace KHECore
{

KPieceTableByteArrayModel::Private::Private( KPieceTableByteArrayModel *parent, const char *data, unsigned int size,
                                             bool careForMemory )
  : p( parent ),
   mReadOnly( false ),
   mModified( false ),
   mAutoDelete( true )
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
   mModified( false ),
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

//     const char result = ( storageId == KPieceTable::Piece::OriginalStorage ) ?
//                         mData[storageOffset] :
//                         mChangeByteArray[storageOffset];
    char result = 0; //TODO: check why we get impossible indizes here sometimes!!!!
    if( storageId == KPieceTable::Piece::OriginalStorage )
    {
        result = mData[storageOffset];
    }
    else
    {
        if( storageOffset < mChangeByteArray.size() )
            result = mChangeByteArray[storageOffset];
    }
    return result;
}

void KPieceTableByteArrayModel::Private::setData( const char *data, unsigned int size, bool careForMemory )
{
    if( mAutoDelete )
        delete mData;
    const int oldSize = mPieceTable.size();

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

    mModified = false;
    emit p->contentsReplaced( 0, oldSize, size );
    emit p->contentsChanged( 0, oldSize-1 );
    emit p->modificationChanged( false );
}

void KPieceTableByteArrayModel::Private::setDatum( unsigned int offset, const char byte )
{
    if( mReadOnly )
        return;

    mPieceTable.replaceOne( offset, mChangeByteArray.size() );
    mChangeByteArray.append( byte );
    mModified = true;

    emit p->contentsReplaced( offset, 1, 1 );
    emit p->contentsChanged( offset, offset );
    emit p->modificationChanged( true );
    emit p->headVersionChanged( mPieceTable.changesCount() );

}


static void appendToByteArray( QByteArray *byteArray, const char *data, int dataLength )
{
    const int oldByteArraySize = byteArray->size();
    byteArray->resize( oldByteArraySize + dataLength );

    memcpy( byteArray->data()+oldByteArraySize, data, dataLength );
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

    const bool newChange = mPieceTable.insert( offset, insertLength, mChangeByteArray.size() );
    appendToByteArray( &mChangeByteArray, insertData, insertLength );

    mModified = true;

    emit p->contentsReplaced( offset, 0, insertLength );
    emit p->contentsChanged( offset, mPieceTable.size()-1 );
    emit p->modificationChanged( true );
    if( newChange )
        emit p->headVersionChanged( mPieceTable.changesCount() );
    else
        emit p->headVersionDescriptionChanged( mPieceTable.headChangeDescription() );
    return insertLength;
}


int KPieceTableByteArrayModel::Private::remove( const KSection &r )
{
    KSection removeSection( r );
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeSection.restrictEndTo( oldSize-1 );
    // check parameters
    if( removeSection.start() >= oldSize || removeSection.width() == 0 )
        return 0;

    const bool newChange = mPieceTable.remove( removeSection );

    mModified = true;

    emit p->contentsReplaced( removeSection.start(), removeSection.width(), 0 );
    //emit p->contentsReplaced( offset, 0,  ); TODO: how to signal the inserted data?
    emit p->contentsChanged( removeSection.start(), oldSize-1 );
    emit p->modificationChanged( true );
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

    mPieceTable.replace( removeSection, insertLength, mChangeByteArray.size() );
    appendToByteArray( &mChangeByteArray, insertData, insertLength );

    mModified = true;

    const int sizeDiff = insertLength - removeSection.width();
    const int lastChanged = ( sizeDiff == 0 ) ? removeSection.end() :
                            ( sizeDiff > 0 ) ?  mPieceTable.size() - 1 :
                                                oldSize - 1;

    emit p->contentsReplaced( removeSection.start(), removeSection.width(), insertLength );
    //emit p->contentsReplaced( offset, 0,  ); TODO: how to signal the changed data at the end?
    emit p->contentsChanged( removeSection.start(), lastChanged );
    emit p->modificationChanged( true );
    emit p->headVersionChanged( mPieceTable.changesCount() );
    return insertLength;
}


bool KPieceTableByteArrayModel::Private::swap( int firstStart, const KSection &second )
{
return false;
    KSection secondSection( second );
    // correct parameters
    secondSection.restrictEndTo( mPieceTable.size()-1 );
    // check parameters
    if( secondSection.start() >= mPieceTable.size() || secondSection.width() <= 0
        || firstStart > mPieceTable.size() || secondSection.start() == firstStart )
        return false;

    mPieceTable.swap( firstStart, secondSection );

    mModified = true;

    emit p->contentsSwapped( firstStart, secondSection.start(),secondSection.width()  );
    emit p->contentsChanged( firstStart, secondSection.end() );
    emit p->modificationChanged( true );
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

    const int filledLength = ( lengthToEnd > fillLength ) ? fillLength : lengthToEnd;
    const int oldChangeByteArraySize = mChangeByteArray.size();
    mPieceTable.replace( offset, filledLength, fillLength, oldChangeByteArraySize );

    mChangeByteArray.resize( oldChangeByteArraySize + fillLength );
    memset( mChangeByteArray.data()+oldChangeByteArraySize, fillByte, fillLength );

    mModified = true;

    emit p->contentsReplaced( offset, fillLength, fillLength );
    emit p->contentsChanged( offset, offset+fillLength-1 );
    emit p->modificationChanged( true );
    emit p->headVersionChanged( mPieceTable.changesCount() );
    return fillLength;
}

void KPieceTableByteArrayModel::Private::revertToVersionByIndex( int versionIndex )
{
    QList<KHE::ReplacementScope> replacementList;
    KHE::KSectionList changedRanges;

    const bool anyChanges =
        mPieceTable.revertBeforeChange( versionIndex, &changedRanges, &replacementList );

    if( !anyChanges )
        return;

//     if( mUnmodifiedVersion == versionIndex )
    // TODO: !mModified ^= Saved should be bound to a version
    bool modificationChanged = true;

    emit p->contentsReplaced( replacementList );
    emit p->contentsChanged( changedRanges );
    if( modificationChanged ) emit p->modificationChanged( mModified );
    emit p->revertedToVersionIndex( versionIndex );
}


KPieceTableByteArrayModel::Private::~Private()
{
    if( mAutoDelete )
        delete [] mData;
}

}
