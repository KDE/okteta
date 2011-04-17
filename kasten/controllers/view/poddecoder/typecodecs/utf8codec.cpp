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

#include "utf8codec.h"

// tool
#include "../types/utf8.h"
#include "../poddata.h"
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextCodec>


namespace Okteta
{
static const unsigned char PrimitivesDefaultUndefinedChar = '?';


Utf8Codec::Utf8Codec()
  : AbstractTypeCodec( i18nc("@label:textbox","UTF-8") ),
    mUtf8Codec( QTextCodec::codecForName( "UTF-8" ) )
{}

QVariant Utf8Codec::value( const PODData& data, int* byteCount ) const
{
    // UTF-8
    // interpreted as a sequence of bytes, there is no endian problem
    // source: http://unicode.org/faq/utf_bom.html#3
    const QChar replacementChar( QChar::ReplacementCharacter );
    const char* originalData = (const char*)data.originalData();
    const int maxUtf8DataSize = data.size();

    QString utf8;
    bool isUtf8 = false;
    *byteCount = 0;
    for( int i=1; i<=maxUtf8DataSize; ++i )
    {
        utf8 = mUtf8Codec->toUnicode( originalData, i );
        if( utf8.size() == 1 && utf8[0] != replacementChar )
        {
            isUtf8 = true;
            *byteCount = i;
            break;
        }
    }

    return isUtf8 ? QVariant::fromValue<Utf8>( Utf8(utf8[0]) ) : QVariant();
}

QByteArray Utf8Codec::valueToBytes( const QVariant& value ) const
{
    const QChar valueChar = value.value<Utf8>().value;

    return mUtf8Codec->fromUnicode( valueChar );
}

bool Utf8Codec::areEqual( const QVariant& value, QVariant& otherValue ) const
{
    return ( value.value<Utf8>().value == otherValue.value<Utf8>().value );
}

Utf8Codec::~Utf8Codec() {}

}
