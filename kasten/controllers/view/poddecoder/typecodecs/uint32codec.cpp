/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "uint32codec.h"

// tool
#include "../poddata.h"
// KDE
#include <KLocale>


namespace Okteta
{

UInt32Codec::UInt32Codec()
  : AbstractTypeCodec( i18nc("@label:textbox","Unsigned 32-bit") )
{}

QVariant UInt32Codec::value( const PODData& data, int* byteCount ) const
{
    const quint32* pointer = (quint32*)data.pointer( 4 );

    *byteCount = pointer ? 4 : 0;
    return ( pointer == 0 ) ? QString() :
           mAsHex ?           QString::fromLatin1( "0x%1" ).arg( *pointer, 8, 16, QChar::fromLatin1('0') ) :
                              QString::number( *pointer );
}

QByteArray UInt32Codec::valueToBytes( const QVariant& value ) const
{
    bool ok;

    const quint32 number = value.toString().toUInt( &ok, mAsHex ? 16 : 10 );

    return ok ? QByteArray( (const char*)&number, sizeof(quint32) ) : QByteArray();
}

UInt32Codec::~UInt32Codec() {}

}
