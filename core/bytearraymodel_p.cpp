/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2003,2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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


static const unsigned int minChunkSize = 512;
static const unsigned int maxChunkSize = 1024*10; // TODO: get max. memory page size

// TODO: think about realloc & Co.

namespace Okteta
{


ByteArrayModelPrivate::ByteArrayModelPrivate( ByteArrayModel *parent,
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

ByteArrayModelPrivate::ByteArrayModelPrivate( ByteArrayModel *parent,
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

ByteArrayModelPrivate::ByteArrayModelPrivate( ByteArrayModel *parent,
                                                int size, int maxSize )
 : p( parent ),
   m_data( (size>0) ? new char[size] : 0 ),
   m_size( size ),
   m_rawSize( size ),
   m_maxSize( maxSize ),
   m_keepsMemory( false ),
   m_autoDelete( true ),
   m_readOnly( false ),
   m_modified( false )
{
}

void ByteArrayModelPrivate::setData( char *data, unsigned int size, int rawSize, bool keepMemory )
{
    if( m_autoDelete )
        delete m_data;
    const int oldSize = m_size;

    m_data = data;
    m_size = size;
    m_rawSize = (rawSize<(int)size) ? size : rawSize;
    if( m_maxSize != -1 && m_maxSize < (int)size )
        m_maxSize = size;
    m_keepsMemory = keepMemory;

    m_modified = false;
    emit p->contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(0, oldSize, size) );
    emit p->modificationChanged( false );
}


int ByteArrayModelPrivate::insert( int position, const char* data, int length )
{
    if( m_readOnly )
        return 0;
    // check all parameters
    if( length == 0 )
      return 0;

    // correct for appending
    if( position > (int)m_size )
        position = m_size;

    length = addSize( length, position, true );

    // copy new data to its place
    memcpy( &m_data[position], data, length );

    const bool bookmarksModified = m_bookmarks.adjustToReplaced( position, 0, length );
    m_modified = true;

    emit p->contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(position, 0, length) );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    emit p->modificationChanged( true );
    return length;
}


int ByteArrayModelPrivate::remove( const KDE::Section& section )
{
    if( m_readOnly )
        return 0;

    KDE::Section removeSection( section );
    if( removeSection.startsBehind(m_size-1) || removeSection.width() == 0 )
        return 0;

    removeSection.restrictEndTo( m_size-1 );

    const unsigned int behindRemovePos = removeSection.nextBehindEnd();
    // move right data behind the input range
    memmove( &m_data[removeSection.start()], &m_data[behindRemovePos], m_size-behindRemovePos );

    // set new values
    m_size -= removeSection.width();

    const bool bookmarksModified = m_bookmarks.adjustToReplaced( removeSection.start(), removeSection.width(), 0 );
    m_modified = true;

    emit p->contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(removeSection.start(), removeSection.width(), 0) );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    emit p->modificationChanged( true );
    return removeSection.width();
}


unsigned int ByteArrayModelPrivate::replace( const KDE::Section& section, const char* data, unsigned int inputLength )
{
    if( m_readOnly )
        return 0;

    KDE::Section removeSection( section );
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
    const int behindRemovePos = removeSection.nextBehindEnd();

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
    m_size = newSize;

    const bool bookmarksModified = m_bookmarks.adjustToReplaced( removeSection.start(), removeSection.width(), inputLength );
    m_modified = true;

    emit p->contentsChanged(
        KDE::ArrayChangeMetricsList::oneReplacement(removeSection.start(), removeSection.width(), inputLength) );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    emit p->modificationChanged( true );
    return inputLength;
}


bool ByteArrayModelPrivate::swap( int firstStart, const KDE::Section& secondSection )
{
    if( m_readOnly )
        return false;

    KDE::Section sourceSection( secondSection );
    // check all parameters
    if( sourceSection.start() >= (int)m_size || sourceSection.width() == 0
        || firstStart > (int)m_size || sourceSection.start() == firstStart )
        return false;

    sourceSection.restrictEndTo( m_size-1 );
    const bool toRight = firstStart > sourceSection.start();
    const int movedLength = sourceSection.width();
    const int displacedLength = toRight ?  firstStart - sourceSection.end()-1 : sourceSection.start() - firstStart;

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
            smallPartDest = firstStart - movedLength;
            largePartStart = sourceSection.nextBehindEnd();
            largePartDest = sourceSection.start();
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
        largePartStart = sourceSection.start();
        largePartLength = movedLength;
        smallPartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            largePartDest = firstStart - movedLength;
            smallPartStart = sourceSection.nextBehindEnd();
            smallPartDest = sourceSection.start();
        }
        else
        {
            largePartDest = firstStart;
            smallPartStart = firstStart;
            smallPartDest = firstStart + movedLength;
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

    const bool bookmarksModified = toRight ?
        m_bookmarks.adjustToSwapped( sourceSection.start(), sourceSection.nextBehindEnd(), firstStart-sourceSection.end()-1 ) :
        m_bookmarks.adjustToSwapped( firstStart, sourceSection.start(),sourceSection.width() );
    m_modified = true;

    emit p->contentsChanged(
        KDE::ArrayChangeMetricsList::oneSwapping(firstStart,sourceSection.start(),sourceSection.width()) );
    if( bookmarksModified ) emit p->bookmarksModified( true );
    emit p->modificationChanged( true );
    return true;
}


int ByteArrayModelPrivate::fill( const char fillDatum, unsigned int position, int fillLength )
{
    if( m_readOnly )
        return 0;

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

    emit p->contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(position,fillLength,fillLength) );
    emit p->modificationChanged( true );
    return fillLength;
}


int ByteArrayModelPrivate::indexOf( const char *searchString, int length, int from ) const
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

int ByteArrayModelPrivate::lastIndexOf( const char *searchString, int length, int from ) const
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


int ByteArrayModelPrivate::addSize( int addSize, int splitPosition, bool saveUpperPart )
{
    if( m_readOnly )
        return 0;

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
