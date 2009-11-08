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

#include "float32codec.h"

// tool
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
    return pointer ? QString::number( *pointer, 'e', 8 ) : QString();
}

QByteArray Float32Codec::valueToBytes( const QVariant& value ) const
{
    bool ok;

    const float number = value.toString().toFloat( &ok );

    return ok ? QByteArray( (const char*)&number, sizeof(float) ) : QByteArray();
}

Float32Codec::~Float32Codec() {}

}
