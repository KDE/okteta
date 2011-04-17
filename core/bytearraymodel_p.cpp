/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003,2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraymodel_p.h"

// C
#include <string.h>
#include <stdlib.h>


static const int minChunkSize = 512;
static const int maxChunkSize = 1024*10; // TODO: get max. memory page size

// TODO: think about realloc & Co.

namespace Okteta
{


ByteArrayModelPrivate::ByteArrayModelPrivate( ByteArrayModel *parent,
                                              Byte* data, int size, int rawSize, bool keepsMemory )
 : p( parent ),
   mData( data ),
   mSize( size ),
   mRawSize( rawSize<size?size:rawSize ),
   mMaxSize( -1 ),
   mKeepsMemory( keepsMemory ),
   mAutoDelete( false ),
   mReadOnly( true ),
   mModified( false )
{
}

ByteArrayModelPrivate::ByteArrayModelPrivate( ByteArrayModel *parent,
                                              const Byte* data, int size )
 : p( parent ),
   mData( const_cast<Byte*>(data) ),
   mSize( size ),
   mRawSize( size ),
   mMaxSize( -1 ),
   mKeepsMemory( true ),
   mAutoDelete( false ),
   mReadOnly( true ),
   mModified( false )
{
}

ByteArrayModelPrivate::ByteArrayModelPrivate( ByteArrayModel *parent,
                                              int size, int maxSize )
 : p( parent ),
   mData( (size>0) ? new Byte[size] : 0 ),
   mSize( size ),
   mRawSize( size ),
   mMaxSize( maxSize ),
   mKeepsMemory( false ),
   mAutoDelete( true ),
   mReadOnly( false ),
   mModified( false )
{
}

void ByteArrayModelPrivate::setData( Byte* data, int size, int rawSize, bool keepMemory )
{
    if( mAutoDelete )
        delete mData;
    const int oldSize = mSize;

    mData = data;
    mSize = size;
    mRawSize = (rawSize<size) ? size : rawSize;
    if( mMaxSize != -1 && mMaxSize < size )
        mMaxSize = size;
    mKeepsMemory = keepMemory;

    mModified = false;
    emit p->contentsChanged( ArrayChangeMetricsList::oneReplacement(0, oldSize, size) );
    emit p->modifiedChanged( false );
}


Size ByteArrayModelPrivate::insert( Address offset, const Byte* insertData, int insertLength )
{
    if( mReadOnly )
        return 0;
    // check all parameters
    if( insertLength == 0 )
      return 0;

    const bool wasModifiedBefore = mModified;

    // correct for appending
    if( offset > mSize )
        offset = mSize;

    insertLength = addSize( insertLength, offset, true );

    // copy new data to its place
    memcpy( &mData[offset], insertData, insertLength );

    const bool bookmarksModified = m_bookmarks.adjustToReplaced( offset, 0, insertLength );
    mModified = true;

    emit p->contentsChanged( ArrayChangeMetricsList::oneReplacement(offset, 0, insertLength) );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    if( ! wasModifiedBefore )
        emit p->modifiedChanged( true );

    return insertLength;
}


Size ByteArrayModelPrivate::remove( const AddressRange& _removeRange )
{
    if( mReadOnly )
        return 0;

    AddressRange removeRange( _removeRange );
    if( removeRange.startsBehind(mSize-1) || removeRange.width() == 0 )
        return 0;

    const bool wasModifiedBefore = mModified;

    removeRange.restrictEndTo( mSize-1 );

    const Address behindRemovePos = removeRange.nextBehindEnd();
    // move right data behind the input range
    memmove( &mData[removeRange.start()], &mData[behindRemovePos], mSize-behindRemovePos );

    // set new values
    mSize -= removeRange.width();

    const bool bookmarksModified = m_bookmarks.adjustToReplaced( removeRange.start(), removeRange.width(), 0 );
    mModified = true;

    emit p->contentsChanged( ArrayChangeMetricsList::oneReplacement(removeRange.start(), removeRange.width(), 0) );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    if( ! wasModifiedBefore )
        emit p->modifiedChanged( true );

    return removeRange.width();
}


Size ByteArrayModelPrivate::replace( const AddressRange& _removeRange, const Byte* insertData, int insertLength )
{
    if( mReadOnly )
        return 0;

    AddressRange removeRange( _removeRange );
    // check all parameters
    if( removeRange.start() >= mSize || (removeRange.width()==0 && insertLength==0) )
        return 0;

    const bool wasModifiedBefore = mModified;

    removeRange.restrictEndTo( mSize-1 );

    const Size sizeDiff = insertLength - removeRange.width();
    int newSize = mSize + sizeDiff;
    // check if buffer does not get to big TODO: make algo simplier and less if else
    if( mMaxSize != -1 && newSize > mMaxSize)
    {
        if( mSize == mMaxSize )
            return 0;
        insertLength -= newSize - mMaxSize;
        newSize = mMaxSize;
    }
    else if( mKeepsMemory && newSize > mRawSize )
    {
        if( mSize == mRawSize )
            return 0;
        insertLength -= newSize - mRawSize;
        newSize = mRawSize;
    }

    const Address behindInsertPos = removeRange.start() + insertLength;
    const Address behindRemovePos = removeRange.nextBehindEnd();

    // raw array not big enough?
    if( mRawSize < newSize )
    {
        // create new buffer
        Byte* newData = new Byte[newSize];
        if( newData == 0 )
            return 0;

        // move old data to its (new) places
        memcpy( newData, mData, removeRange.start() );
        memcpy( &newData[behindInsertPos], &mData[behindRemovePos], mSize-behindRemovePos );

        // remove old
        delete [] mData;
        // set new values
        mData = newData;
        mRawSize = newSize;
    }
    else
        // move old data to its (new) places
        memmove( &mData[behindInsertPos], &mData[behindRemovePos], mSize-behindRemovePos );

    // copy new data to its place
    memcpy( &mData[removeRange.start()], insertData, insertLength );

    // set new values
    mSize = newSize;

    const bool bookmarksModified = m_bookmarks.adjustToReplaced( removeRange.start(), removeRange.width(), insertLength );
    mModified = true;

    emit p->contentsChanged(
        ArrayChangeMetricsList::oneReplacement(removeRange.start(), removeRange.width(), insertLength) );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    if( ! wasModifiedBefore )
        emit p->modifiedChanged( true );
    return insertLength;
}


bool ByteArrayModelPrivate::swap( Address firstStart, const AddressRange& _secondRange )
{
    if( mReadOnly )
        return false;

    AddressRange secondRange( _secondRange );
    // check all parameters
    if( secondRange.start() >= mSize || secondRange.width() == 0
        || firstStart > mSize || secondRange.start() == firstStart )
        return false;

    const bool wasModifiedBefore = mModified;

    secondRange.restrictEndTo( mSize-1 );
    const bool toRight = firstStart > secondRange.start();
    const Size movedLength = secondRange.width();
    const Size displacedLength = toRight ?  firstStart - secondRange.end()-1 : secondRange.start() - firstStart;

    // find out section that is smaller
    Size smallPartLength, largePartLength, smallPartStart, largePartStart, smallPartDest, largePartDest;
    // moving part is smaller?
    if( movedLength < displacedLength )
    {
        smallPartStart = secondRange.start();
        smallPartLength = movedLength;
        largePartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            smallPartDest = firstStart - movedLength;
            largePartStart = secondRange.nextBehindEnd();
            largePartDest = secondRange.start();
        }
        else
        {
            smallPartDest = firstStart;
            largePartStart = firstStart;
            largePartDest = firstStart + movedLength;
        }
    }
    else
    {
        largePartStart = secondRange.start();
        largePartLength = movedLength;
        smallPartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            largePartDest = firstStart - movedLength;
            smallPartStart = secondRange.nextBehindEnd();
            smallPartDest = secondRange.start();
        }
        else
        {
            largePartDest = firstStart;
            smallPartStart = firstStart;
            smallPartDest = firstStart + movedLength;
        }
    }

    // copy smaller part to tempbuffer
    Byte* temp = new Byte[smallPartLength];
    memcpy( temp, &mData[smallPartStart], smallPartLength );

    // move the larger part
    memmove( &mData[largePartDest], &mData[largePartStart], largePartLength );

    // copy smaller part to its new dest
    memcpy( &mData[smallPartDest], temp, smallPartLength );
    delete [] temp;

    const bool bookmarksModified = toRight ?
        m_bookmarks.adjustToSwapped( secondRange.start(), secondRange.nextBehindEnd(), firstStart-secondRange.end()-1 ) :
        m_bookmarks.adjustToSwapped( firstStart, secondRange.start(),secondRange.width() );
    mModified = true;

    emit p->contentsChanged(
        ArrayChangeMetricsList::oneSwapping(firstStart,secondRange.start(),secondRange.width()) );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    if( ! wasModifiedBefore )
        emit p->modifiedChanged( true );
    return true;
}


Size ByteArrayModelPrivate::fill( Byte fillByte, Address offset, Size fillLength )
{
    if( mReadOnly )
        return 0;

    // nothing to fill
    if( offset >= mSize )
        return 0;

    const bool wasModifiedBefore = mModified;

    const Size lengthToEnd = mSize - offset;

    if( fillLength < 0 )
        fillLength = lengthToEnd;
    else if( fillLength > lengthToEnd )
        fillLength = addSize( fillLength, offset, false );

    memset( &mData[offset], fillByte, fillLength );
    mModified = true;

    emit p->contentsChanged( ArrayChangeMetricsList::oneReplacement(offset,fillLength,fillLength) );
    if( ! wasModifiedBefore )
        emit p->modifiedChanged( true );
    return fillLength;
}


int ByteArrayModelPrivate::addSize( int addSize, int splitPosition, bool saveUpperPart )
{
    if( mReadOnly )
        return 0;

    int newSize = mSize + addSize;
    // check if buffer does not get too big
    if( mMaxSize != -1 && newSize > mMaxSize )
    {
        if( mSize == mMaxSize )
            return 0;
        newSize = mMaxSize;
        addSize = newSize - mSize;
    }
    else if( mKeepsMemory && newSize > mRawSize )
    {
        if( mSize == mRawSize )
            return 0;
        newSize = mRawSize;
        addSize = newSize - mSize;
    }

    const int BehindSplitPos = splitPosition + addSize;
    // raw array not big enough?
    if( mRawSize < newSize )
    {
        // get new raw size
        int chunkSize = minChunkSize;
        // find chunk size where newsize fits into
        while( chunkSize < newSize )
            chunkSize <<= 1;
        // limit to max size
        if( chunkSize > maxChunkSize )
            chunkSize = maxChunkSize;
        // find add size
        int NewRawSize = chunkSize;
        while( NewRawSize<newSize )
            NewRawSize += chunkSize;
        // create new buffer
        Byte* newData = new Byte[NewRawSize];

        // move old data to its (new) places
        memcpy( newData, mData, splitPosition );
        if( saveUpperPart )
            memcpy( &newData[BehindSplitPos], &mData[splitPosition], mSize-splitPosition );

        // remove old
        delete [] mData;
        // set new values
        mData = newData;
        mRawSize = NewRawSize;
    }
    // old buffer kept
    else
    {
        if( saveUpperPart )
            // move old data to its (new) places
        memmove( &mData[BehindSplitPos], &mData[splitPosition], mSize-splitPosition );
    }

    // set new values
    mSize = newSize;

    return addSize;
}

}
