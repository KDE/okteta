/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kabstractbytearraymodel.h"

// C
#include <ctype.h>


namespace KHECore {

KAbstractByteArrayModel::KAbstractByteArrayModel() {}

bool KAbstractByteArrayModel::isReadOnly() const { return false; }

void KAbstractByteArrayModel::setReadOnly( bool isReadOnly )
{
    Q_UNUSED( isReadOnly )
}

int KAbstractByteArrayModel::insert( int offset, const char *insertData, int insertLength )
{
    return replace( offset, 0, insertData, insertLength );
}


int KAbstractByteArrayModel::remove( const KSection &removeSection )
{
    replace( removeSection, 0, 0 );
    return removeSection.width(); // TODO: check if this is true
}


int KAbstractByteArrayModel::copyTo( char *dest, const KSection &cS ) const
{
    KSection copySection( cS );
    copySection.restrictEndTo( size()-1 );

    for( int i=copySection.start(); i<=copySection.end(); ++i )
        *dest++ = datum( i );

    return copySection.width();
}


int KAbstractByteArrayModel::indexOf( const char *pattern, int patternLength, int fromOffset  ) const
{
    int result = -1;

    const int lastFrom = size() - patternLength;

    for( int i=fromOffset; i<=lastFrom ; ++i )
    {
        int c = 0;
        for( ; c<patternLength; ++c )
            if( pattern[c] != datum(i+c) )
                break;
        if( c == patternLength )
        {
            result = i;
            break;
        }
    }

    return result;
}

int KAbstractByteArrayModel::lastIndexOf( const char *pattern, int patternLength, int fromOffset ) const
{
    int result = -1;

    const int lastFrom = size() - patternLength;

    if( fromOffset < 0 )
        fromOffset = lastFrom + 1 + fromOffset;
    else if( fromOffset > lastFrom )
        fromOffset = lastFrom;

    for( int i=fromOffset; i>=0 ; --i )
    {
        int c = 0;
        for( ; c<patternLength; ++c )
            if( pattern[c] != datum(i+c) )
                break;
        if( c == patternLength )
        {
            result = i;
            break;
        }
    }

    return result;
}

KAbstractByteArrayModel::~KAbstractByteArrayModel() {}

}
