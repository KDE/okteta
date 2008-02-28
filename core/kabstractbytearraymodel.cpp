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

void KAbstractByteArrayModel::setReadOnly( bool isReadOnly )
{
    Q_UNUSED( isReadOnly )
}

int KAbstractByteArrayModel::insert( int Pos, const char* D, int Length )
{
  return replace( Pos,0,D,Length );
}


int KAbstractByteArrayModel::remove( const KSection &Remove )
{
  replace( Remove, 0, 0 );
  return Remove.width(); // TODO: check if this is true
}

int KAbstractByteArrayModel::copyTo( char* Dest, int Pos, int Length ) const
{
  return copyTo( Dest, KSection::fromWidth(Pos,Length) );
}


int KAbstractByteArrayModel::copyTo( char* Dest, const KSection &S ) const
{
  KSection Source( S );
  Source.restrictEndTo( size()-1 );
  for( int i=Source.start(); i<=Source.end(); ++i )
    *Dest++ = datum( i );
  return Source.width();
}


int KAbstractByteArrayModel::indexOf( const char* Data, int Length, int From ) const
{
    int Result = -1;

    const int LastFrom = size() - Length;

    for( int i=From; i<=LastFrom ; ++i )
    {
        int c = 0;
        for( ; c<Length; ++c )
            if( Data[c] != datum(i+c) )
                break;
        if( c == Length )
        {
            Result = i;
            break;
        }
    }

    return Result;
}

int KAbstractByteArrayModel::lastIndexOf( const char* Data, int Length, int From ) const
{
    int Result = -1;

    const int LastFrom = size() - Length;

    if( From < 0 )
        From = LastFrom + 1 + From;
    else if( From > LastFrom )
        From = LastFrom;

    for( int i=From; i>=0 ; --i )
    {
        int c = 0;
        for( ; c<Length; ++c )
            if( Data[c] != datum(i+c) )
                break;
        if( c == Length )
        {
            Result = i;
            break;
        }
    }

    return Result;
}

KAbstractByteArrayModel::~KAbstractByteArrayModel() {}

}
