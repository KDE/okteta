/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2006,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "charcodeciftest.h"

// test object
#include <charcodec.h>
// lib
#include <character.h>
// Qt
#include <QtTest/QtTest>


namespace Okteta
{

void CharCodecIfTest::init()
{
    mCharCodec = createCodec();
}

void CharCodecIfTest::cleanup()
{
    deleteCodec( mCharCodec );
}


void CharCodecIfTest::testEncodeDecode_data()
{
    QTest::addColumn<int>("byteValue");

    for( int i = 0; i < 256; ++i )
    {
        const QString rowTitle = QString::fromLatin1( "byte %1" ).arg( i );
        QTest::newRow(rowTitle.toLatin1().constData()) << i;
    }
}

// TODO: is duplication with what is on CharCodecTest
// (though that is complete, TextCharCodecCharCodecIfTest ATM misses to test all TextCharCodecs)
void CharCodecIfTest::testEncodeDecode()
{
    QFETCH(int, byteValue);

    // current assumption: the mapping of chars to byte values is biunique for all used charsets
    const Byte byte = Byte( byteValue );
    Character character = mCharCodec->decode( byte );
    if( ! character.isUndefined() )
    {
        QVERIFY( mCharCodec->canEncode(character) );

        Byte encodedByte;
        const bool encodeSuccess = mCharCodec->encode( &encodedByte, character );
        QVERIFY( encodeSuccess );
        QCOMPARE( encodedByte, byte );
    }
}

}
