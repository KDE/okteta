/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractbytearraymodel.h"


namespace KHECore
{

static const int SearchedByteCountSignalLimit = 10000;

AbstractByteArrayModel::AbstractByteArrayModel() {}

bool AbstractByteArrayModel::isReadOnly() const { return false; }

void AbstractByteArrayModel::setReadOnly( bool isReadOnly )
{
    Q_UNUSED( isReadOnly )
}

int AbstractByteArrayModel::insert( int offset, const char *insertData, int insertLength )
{
    return replace( offset, 0, insertData, insertLength );
}


int AbstractByteArrayModel::remove( const Section &removeSection )
{
    replace( removeSection, 0, 0 );
    return removeSection.width(); // TODO: check if this is true
}


int AbstractByteArrayModel::copyTo( char *dest, const Section &cS ) const
{
    Section copySection( cS );
    copySection.restrictEndTo( size()-1 );

    for( int i=copySection.start(); i<=copySection.end(); ++i )
        *dest++ = datum( i );

    return copySection.width();
}


int AbstractByteArrayModel::indexOf( const char *pattern, int patternLength, int fromOffset  ) const
{
    int result = -1;

    const int lastFrom = size() - patternLength;
    int searchedBytesCount = 0;

    for( int i=fromOffset; i<=lastFrom ; ++i )
    {
        ++searchedBytesCount;
        int c = 0;
        for( ; c<patternLength; ++c )
            if( pattern[c] != datum(i+c) )
                break;

        if( searchedBytesCount >= SearchedByteCountSignalLimit )
        {
            searchedBytesCount = 0;
            emit searchedBytes( i-fromOffset+1 );
        }

        if( c == patternLength )
        {
            result = i;
            break;
        }
    }

    return result;
}

int AbstractByteArrayModel::lastIndexOf( const char *pattern, int patternLength, int fromOffset ) const
{
    int result = -1;

    const int lastFrom = size() - patternLength;
    int searchedBytesCount = 0;

    if( fromOffset < 0 )
        fromOffset = lastFrom + 1 + fromOffset;
    else if( fromOffset > lastFrom )
        fromOffset = lastFrom;

    for( int i=fromOffset; i>=0 ; --i )
    {
        ++searchedBytesCount;
        int c = 0;
        for( ; c<patternLength; ++c )
            if( pattern[c] != datum(i+c) )
                break;

        if( searchedBytesCount >= SearchedByteCountSignalLimit )
        {
            searchedBytesCount = 0;
            emit searchedBytes( i-fromOffset+1 );
        }

        if( c == patternLength )
        {
            result = i;
            break;
        }
    }

    return result;
}

AbstractByteArrayModel::~AbstractByteArrayModel() {}

}

#include "abstractbytearraymodel.moc"
