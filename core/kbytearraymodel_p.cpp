/***************************************************************************
                          kbytearraymodel_p.cpp  -  description
                             -------------------
    copyright            : (C) 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "kbytearraymodel_p.h"

// C
#include <string.h>
#include <stdlib.h>


static const unsigned int minChunkSize = 512;
static const unsigned int maxChunkSize = 1024*10; // TODO: get max. memory page size

// TODO: think about realloc & Co.

namespace KHECore {


KByteArrayModelPrivate::KByteArrayModelPrivate( KByteArrayModel *parent,
                                                char *data, unsigned int size, int rawSize, bool keepsMemory )
 : p( parent ),
   m_data( data ),
   m_size( size ),
   m_rawSize( rawSize<(int)size?size:rawSize ),
   m_maxSize( -1 ),
   m_keepsMemory( keepsMemory ),
   m_autoDelete( false ),
   m_readOnly( true ),
   m_modified( false )
{
}

KByteArrayModelPrivate::KByteArrayModelPrivate( KByteArrayModel *parent,
                                                const char *data, unsigned int size )
 : p( parent ),
   m_data( (char *)data ),
   m_size( size ),
   m_rawSize( size ),
   m_maxSize( -1 ),
   m_keepsMemory( true ),
   m_autoDelete( false ),
   m_readOnly( true ),
   m_modified( false )
{
}

KByteArrayModelPrivate::KByteArrayModelPrivate( KByteArrayModel *parent,
                                                int size, int maxSize )
 : p( parent ),
   m_data( (size>0) ? new char[size] : 0 ),
   m_size( size ),
   m_rawSize( size ),
   m_maxSize( maxSize ),
   m_keepsMemory( false ),
   m_autoDelete( true ),
   m_readOnly( true ),
   m_modified( false )
{
}


int KByteArrayModelPrivate::insert( int position, const char* data, int length )
{
    // check all parameters
    if( length == 0 )
      return 0;

    // correct for appending
    if( position > (int)m_size )
        position = m_size;

    length = addSize( length, position, true );

    // copy new data to its place
    memcpy( &m_data[position], data, length );

    m_modified = true;

    emit p->contentsReplaced( position, 0, length );
    emit p->contentsChanged( position, m_size-1 );
    return length;
}


int KByteArrayModelPrivate::remove( const KSection &section )
{
    KSection removeSection( section );
    if( removeSection.startsBehind(m_size-1) || removeSection.width() == 0 )
        return 0;

    removeSection.restrictEndTo( m_size-1 );

    const unsigned int behindRemovePos = removeSection.end() + 1;
    // move right data behind the input range
    memmove( &m_data[removeSection.start()], &m_data[behindRemovePos], m_size-behindRemovePos );

    // set new values
    const int oldSize = m_size;
    m_size -= removeSection.width();

    m_modified = true;

    emit p->contentsReplaced( removeSection.start(), removeSection.width(), 0 );
    emit p->contentsChanged( removeSection.start(), oldSize-1 );
    return removeSection.width();
}


unsigned int KByteArrayModelPrivate::replace( const KSection &section, const char* data, unsigned int inputLength )
{
    KSection removeSection( section );
    // check all parameters
    if( removeSection.start() >= (int)m_size || (removeSection.width()==0 && inputLength==0) )
        return 0;

    removeSection.restrictEndTo( m_size-1 );

    const int sizeDiff = inputLength - removeSection.width();
    unsigned int newSize = m_size + sizeDiff;
    // check if buffer does not get to big TODO: make algo simplier and less if else
    if( m_maxSize != -1 && (int)newSize > m_maxSize)
    {
        if( (int)m_size == m_maxSize )
            return 0;
        inputLength -= newSize - m_maxSize;
        newSize = m_maxSize;
    }
    else if( m_keepsMemory && newSize > m_rawSize )
    {
        if( m_size == m_rawSize )
            return 0;
        inputLength -= newSize - m_rawSize;
        newSize = m_rawSize;
    }

    const int behindInsertPos = removeSection.start() + inputLength;
    const int behindRemovePos = removeSection.end()+1;

    // raw array not big enough?
    if( m_rawSize < newSize )
    {
        // create new buffer
        char *newData = new char[newSize];
        if( newData == 0 )
            return 0;

        // move old data to its (new) places
        memcpy( newData, m_data, removeSection.start() );
        memcpy( &newData[behindInsertPos], &m_data[behindRemovePos], m_size-behindRemovePos );

        // remove old
        delete [] m_data;
        // set new values
        m_data = newData;
        m_rawSize = newSize;
    }
    else
        // move old data to its (new) places
        memmove( &m_data[behindInsertPos], &m_data[behindRemovePos], m_size-behindRemovePos );

    // copy new data to its place
    memcpy( &m_data[removeSection.start()], data, inputLength );

    // set new values
    const int oldSize = m_size;
    m_size = newSize;

    m_modified = true;

    emit p->contentsReplaced( removeSection.start(), removeSection.width(), inputLength );
    emit p->contentsChanged( removeSection.start(), sizeDiff==0?removeSection.end():((sizeDiff>0?m_size:oldSize)-1) );
    return inputLength;
}


int KByteArrayModelPrivate::move( int destinationStart, const KSection &ss )
{
    KSection sourceSection( ss );
    // check all parameters
    if( sourceSection.start() >= (int)m_size || sourceSection.width() == 0
        || destinationStart > (int)m_size || sourceSection.start() == destinationStart )
        return sourceSection.start();

    sourceSection.restrictEndTo( m_size-1 );
    const bool toRight = destinationStart > sourceSection.start();
    const int movedLength = sourceSection.width();
    const int displacedLength = toRight ?  destinationStart - sourceSection.end()-1 : sourceSection.start() - destinationStart;

    // find out section that is smaller
    int smallPartLength, largePartLength, smallPartStart, largePartStart, smallPartDest, largePartDest;
    // moving part is smaller?
    if( movedLength < displacedLength )
    {
        smallPartStart = sourceSection.start();
        smallPartLength = movedLength;
        largePartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            smallPartDest = destinationStart - movedLength;
            largePartStart = sourceSection.end()+1;
            largePartDest = sourceSection.start();
        }
        else
        {
            smallPartDest = destinationStart;
            largePartStart = destinationStart;
            largePartDest = destinationStart + movedLength;
        }
    }
    else
    {
        largePartStart = sourceSection.start();
        largePartLength = movedLength;
        smallPartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            largePartDest = destinationStart - movedLength;
            smallPartStart = sourceSection.end()+1;
            smallPartDest = sourceSection.start();
        }
        else
        {
            largePartDest = destinationStart;
            smallPartStart = destinationStart;
            smallPartDest = destinationStart + movedLength;
        }
    }

    // copy smaller part to tempbuffer
    char *temp = new char[smallPartLength];
    memcpy( temp, &m_data[smallPartStart], smallPartLength );

    // move the larger part
    memmove( &m_data[largePartDest], &m_data[largePartStart], largePartLength );

    // copy smaller part to its new dest
    memcpy( &m_data[smallPartDest], temp, smallPartLength );
    delete [] temp;

    m_modified = true;

    emit p->contentsMoved( destinationStart, sourceSection.start(),sourceSection.width()  );
    emit p->contentsChanged( toRight?sourceSection.start():destinationStart, toRight?destinationStart:sourceSection.end() );
    return (movedLength<displacedLength) ? smallPartDest : largePartDest;
}


int KByteArrayModelPrivate::fill( const char fillDatum, unsigned int position, int fillLength )
{
    // nothing to fill
    if( position >= m_size )
        return 0;

    const int lengthToEnd = m_size - position;

    if( fillLength < 0 )
        fillLength = lengthToEnd;
    else if( fillLength > lengthToEnd )
        fillLength = addSize( fillLength, position, false );

    memset( &m_data[position], fillDatum, fillLength );
    m_modified = true;

    emit p->contentsReplaced( position, fillLength, fillLength );
    emit p->contentsChanged( position, position+fillLength-1 );
    return fillLength;
}


int KByteArrayModelPrivate::indexOf( const char *searchString, int length, int from ) const
{
    int result = -1;

    const int lastFrom = m_size - length;

    for( int i=from; i<=lastFrom ; ++i )
    {
        int c = memcmp( &m_data[i], searchString, length );

        if( c == 0 )
        {
            result = i;
            break;
        }
    }

    return result;
}

int KByteArrayModelPrivate::lastIndexOf( const char *searchString, int length, int from ) const
{
    int result = -1;

    const int lastFrom = size() - length;

    if( from < 0 )
        from = lastFrom + 1 + from;
    else if( from > lastFrom )
        from = lastFrom;

    for( int i=from; i>=0 ; --i )
    {
        int c = memcmp( &m_data[i], searchString, length );

        if( c == 0 )
        {
            result = i;
            break;
        }
    }

    return result;
}


int KByteArrayModelPrivate::addSize( int addSize, int splitPosition, bool saveUpperPart )
{
    unsigned int newSize = m_size + addSize;
    // check if buffer does not get too big
    if( m_maxSize != -1 && (int)newSize > m_maxSize )
    {
        if( (int)m_size == m_maxSize )
            return 0;
        newSize = m_maxSize;
        addSize = newSize - m_size;
    }
    else if( m_keepsMemory && newSize > m_rawSize )
    {
        if( m_size == m_rawSize )
            return 0;
        newSize = m_rawSize;
        addSize = newSize - m_size;
    }

    const int BehindSplitPos = splitPosition + addSize;
    // raw array not big enough?
    if( m_rawSize < newSize )
    {
        // get new raw size
        unsigned int chunkSize = minChunkSize;
        // find chunk size where newsize fits into
        while( chunkSize < newSize )
            chunkSize <<= 1;
        // limit to max size
        if( chunkSize > maxChunkSize )
            chunkSize = maxChunkSize;
        // find add size
        unsigned int NewRawSize = chunkSize;
        while( NewRawSize<newSize )
            NewRawSize += chunkSize;
        // create new buffer
        char *newData = new char[NewRawSize];

        // move old data to its (new) places
        memcpy( newData, m_data, splitPosition );
        if( saveUpperPart )
            memcpy( &newData[BehindSplitPos], &m_data[splitPosition], m_size-splitPosition );

        // remove old
        delete [] m_data;
        // set new values
        m_data = newData;
        m_rawSize = NewRawSize;
    }
    // old buffer kept
    else
    {
        if( saveUpperPart )
            // move old data to its (new) places
        memmove( &m_data[BehindSplitPos], &m_data[splitPosition], m_size-splitPosition );
    }

    // set new values
    m_size = newSize;

    return addSize;
}

}
