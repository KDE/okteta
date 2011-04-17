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

#include "float32codec.h"

// tool
#include "../types/float32.h"
#include "../poddata.h"
// KDE
#include <KLocale>


namespace Okteta
{

Float32Codec::Float32Codec()
  : AbstractTypeCodec( i18nc("@label:textbox","Float 32-bit") )
{}

QVariant Float32Codec::value( const PODData& data, int* byteCount ) const
{
    const float* pointer = (float*)data.pointer( 4 );

    *byteCount = pointer ? 4 : 0;
    return pointer ? QVariant::fromValue<Float32>( Float32(*pointer) ) : QVariant();
}

QByteArray Float32Codec::valueToBytes( const QVariant& value ) const
{
    const float number = value.value<Float32>().value;

    return QByteArray( (const char*)&number, sizeof(float) );
}

bool Float32Codec::areEqual( const QVariant& value, QVariant& otherValue ) const
{
    return ( value.value<Float32>().value == otherValue.value<Float32>().value );
}

Float32Codec::~Float32Codec() {}

}
