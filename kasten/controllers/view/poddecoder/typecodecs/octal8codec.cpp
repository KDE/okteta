/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "octal8codec.h"

// tool
#include "../types/octal8.h"
#include "../poddata.h"
// KDE
#include <KLocale>


namespace Okteta
{

Octal8Codec::Octal8Codec()
  : AbstractTypeCodec( i18nc("@label:textbox encoding of one byte as value in the octal format","Octal 8-bit") )
{}

QVariant Octal8Codec::value( const PODData& data, int* byteCount ) const
{
    const unsigned char* pointer = (unsigned char*)data.pointer( 1 );

    *byteCount = pointer ? 1 : 0;
    return pointer ? QVariant::fromValue<Octal8>( Octal8(*pointer) ) : QVariant();
}

QByteArray Octal8Codec::valueToBytes( const QVariant& value ) const
{
    const quint8 number = value.value<Octal8>().value;

    return QByteArray( (const char*)&number, sizeof(quint8) );
}

bool Octal8Codec::areEqual( const QVariant& value, QVariant& otherValue ) const
{
    return ( value.value<Octal8>().value == otherValue.value<Octal8>().value );
}

Octal8Codec::~Octal8Codec() {}

}
