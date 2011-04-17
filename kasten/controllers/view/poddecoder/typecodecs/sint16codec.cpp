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

#include "sint16codec.h"

// tool
#include "../types/sint16.h"
#include "../poddata.h"
// KDE
#include <KLocale>


namespace Okteta
{

SInt16Codec::SInt16Codec()
  : AbstractTypeCodec( i18nc("@label:textbox","Signed 16-bit") )
{}

QVariant SInt16Codec::value( const PODData& data, int* byteCount ) const
{
    const qint16* pointer = (qint16*)data.pointer( 2 );

    *byteCount = pointer ? 2 : 0;
    return pointer ? QVariant::fromValue<SInt16>( SInt16(*pointer) ) : QVariant();
}

QByteArray SInt16Codec::valueToBytes( const QVariant& value ) const
{
    const qint16 number = value.value<SInt16>().value;

    return QByteArray( (const char*)&number, sizeof(qint16) );
}

bool SInt16Codec::areEqual( const QVariant& value, QVariant& otherValue ) const
{
    return ( value.value<SInt16>().value == otherValue.value<SInt16>().value );
}

SInt16Codec::~SInt16Codec() {}

}
