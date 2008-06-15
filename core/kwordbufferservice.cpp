/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2005,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kwordbufferservice.h"

// lib
#include "kabstractbytearraymodel.h"
#include "khechar.h"
#include "kcharcodec.h"


namespace KHECore
{

KWordBufferService::KWordBufferService( KAbstractByteArrayModel *byteArrayModel, KCharCodec *charCodec )
 : mByteArrayModel( byteArrayModel ), mCharCodec( charCodec )
{}

KHE::KSection KWordBufferService::wordSection( unsigned int index ) const
{
    return isWordChar(index) ?
            KHE::KSection( indexOfWordStart(index), indexOfWordEnd(index) ) :
            KHE::KSection();
}


bool KWordBufferService::isWordChar( unsigned int index ) const
{
    const KChar decodedChar = mCharCodec->decode( mByteArrayModel->datum(index) );
    return !decodedChar.isUndefined() && decodedChar.isLetterOrNumber();
}


int KWordBufferService::indexOfPreviousWordStart( unsigned int index ) const
{
    const unsigned int size = mByteArrayModel->size();
    // already at the start or can the result only be 0?
    if( index == 0 || size < 3 )
        return 0;

    // search in two rounds: first for the next char, than for the next nonchar
    // after that return the index of the one before
    bool lookingForFirstWordChar = false;
    for( ; index>0; --index )
    {
        if( !isWordChar(index-1) )
        {
            if( !lookingForFirstWordChar )
                continue;
            return( index );
        }
        else if( !lookingForFirstWordChar )
            lookingForFirstWordChar = true;
    }
    return 0;
}


int KWordBufferService::indexOfNextWordStart( unsigned int index ) const
{
    const unsigned int size = mByteArrayModel->size();
    bool lookingForFirstWordChar = false;
    for( ; index<size; ++index )
    {
        if( isWordChar(index) )
        {
            if( !lookingForFirstWordChar )
                continue;
            return index;
        }
        else if( !lookingForFirstWordChar )
            lookingForFirstWordChar = true;
    }
    // if no more word found, go to the end
    return size;
}


int KWordBufferService::indexOfBeforeNextWordStart( unsigned int index ) const
{
    const unsigned int size = mByteArrayModel->size();
    bool lookingForFirstWordChar = false;
    for( ; index<size; ++index )
    {
        if( isWordChar(index) )
        {
            if( !lookingForFirstWordChar )
                continue;
            return index-1;
        }
        else if( !lookingForFirstWordChar )
            lookingForFirstWordChar = true;
    }
    // if no more word found, go to the end
    return size-1;
}


int KWordBufferService::indexOfWordStart( unsigned int index ) const
{
    for( ; index > 0; --index )
    {
        if( !isWordChar(index-1) )
            return( index );
    }
    return 0;
}


int KWordBufferService::indexOfWordEnd( unsigned int index ) const
{
    const unsigned int size = mByteArrayModel->size();
    for( ++index; index<size; ++index )
    {
        if( !isWordChar(index) )
            return index-1;
    }
    // word reaches the end
    return size-1;
}


int KWordBufferService::indexOfLeftWordSelect( unsigned int index ) const
{
    // word at index?
    if( isWordChar(index) )
    {
        // search for word start to the left
        for( ; index>0; --index )
        {
            if( !isWordChar(index-1) )
                return index;
        }
        // reached start, so return it
        return 0;
    }
    else
    {
        const unsigned int size = mByteArrayModel->size();
        // search for word start to the right
        for( ++index; index<size; ++index )
        {
            if( isWordChar(index) )
                return index;
        }
        // word reaches the end, so step behind
        return size;
    }
}


int KWordBufferService::indexOfRightWordSelect( unsigned int index ) const
{
    // TODO: should this check be here or with the caller?
    // the later would need another function to search the previous word end
    const unsigned int size = mByteArrayModel->size();
    bool searchToLeft;
    if( index >= size )
    {
        index = size;
        searchToLeft = true;
    }
    else
        searchToLeft = !isWordChar(index);
    // no word at index?
    if(  searchToLeft )
    {
        // search for word end to the left
        for( ; index>0; --index )
        {
            if( isWordChar(index-1) )
                return index;
        }
        // reached start, so return it
        return 0;
    }
    else
    {
        for( ++index; index<size; ++index )
        {
            // search for word end to the right
            if( !isWordChar(index) )
                return index;
        }
        // word reaches the end, so step behind
        return size;
    }
}

/*
int KWordBufferService::indexOfBehindWordEnd( unsigned int index ) const
{
  // no word at index?
  return !::isWordChar(datum(index)) ? indexOfBehindLeftWordEnd(index) : indexOfBehindRightWordEnd(index+1)
}


int KWordBufferService::indexOfBehindRightWordEnd( unsigned int index ) const
{
  for( ; index<size(); ++index )
{
    if( !::isWordChar(datum(index)) )
      return index;
}
  // word reaches the end, so step behind
  return size();
}


int KWordBufferService::indexOfBehindLeftWordEnd( unsigned int index ) const
{
  for( --index; index>=0; --index )
{
    if( ::isWordChar(datum(index)) )
      return index+1;
}
  // word reaches the end, so step behind
  return 0;
}
*/

KWordBufferService::~KWordBufferService() {}

}
