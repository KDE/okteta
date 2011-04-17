/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "fill.h"

// 
#include <abstractbytearraymodel.h>
// Qt
#include <QtCore/QByteArray>


using namespace Okteta;


// fills the buffer with char from b to e
void textureByteArrayModel( AbstractByteArrayModel *byteArrayModel,
                            unsigned char b, unsigned char e,
                            unsigned int From, int To )
{
    const Size size = byteArrayModel->size();

    // check
    if( To == -1 || To >= size )
      To = size-1;
    if( b > e )
      b = e;

    // do
    unsigned char c = b;
    for( int i=From; i<=To; ++i )
    {
        byteArrayModel->setByte(i, c );
        if( c == e )
            c = b;
        else
            c++;
    }
}


void textureByteArrayModel( AbstractByteArrayModel *byteArrayModel,
                            unsigned char b, unsigned char e,
                            const AddressRange& range )
{
    textureByteArrayModel( byteArrayModel, b, e, range.start(), range.end() );
}


void textureByteArray( QByteArray *byteArray,
                       unsigned char b, unsigned char e,
                       unsigned int From, int To )
{
    const int size = byteArray->size();

    // check
    if( To == -1 || To >= size )
      To = size-1;
    if( b > e )
      b = e;

    // do
    unsigned char c = b;
    for( int i=From; i<=To; ++i )
    {
        (*byteArray)[i] = c;
        if( c == e )
            c = b;
        else
            c++;
    }
}


void textureByteArray( QByteArray *byteArray,
                       unsigned char b, unsigned char e,
                       const AddressRange& range )
{
    textureByteArray( byteArray, b, e, range.start(), range.end() );
}

/*
static void list( AbstractByteArrayModel *byteArrayModel, const char* Name )
{
  unsigned int size = byteArrayModel->size();
  for( unsigned int i=0; i<size; ++i )
  {
    kdDebug() << Name<<":"<<i<<":"<<byteArrayModel->datum(i) << endl;
  }
}
*/
