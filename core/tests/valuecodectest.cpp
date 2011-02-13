/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "valuecodectest.h"

// test object
#include <valuecodec.h>
// KDE
#include <qtest_kde.h>


namespace Okteta
{

//---------------------------------------------------------------------------- Tests -----

static const struct {const char* name; int id; } valueCodecs[] =
{
    {"HexadecimalByteCodec", HexadecimalCoding},
    {"DecimalByteCodec", DecimalCoding},
    {"OctalByteCodec", OctalCoding},
    {"BinaryByteCodec", BinaryCoding}
};
static const int valueCodecCount = sizeof(valueCodecs)/sizeof(valueCodecs[0]);


void ValueCodecTest::testCreateCodec_data()
{
    QTest::addColumn<int>("codecId");

    for( int i = 0; i < valueCodecCount; ++i )
        QTest::newRow(valueCodecs[i].name) << valueCodecs[i].id;
}

void ValueCodecTest::testCreateCodec()
{
    QFETCH(int, codecId);

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QVERIFY( codec != 0 );

    delete codec;
}

void ValueCodecTest::testEncodeDecode_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");

    for( int i = 0; i < valueCodecCount; ++i )
        for( int j = 0; j < 256; ++j )
        {
            const QString rowTitle = valueCodecs[i].name+QString::fromLatin1(" - %1").arg(j);
            QTest::newRow(rowTitle.toLatin1().constData()) << valueCodecs[i].id << Byte(j);
        }
}

void ValueCodecTest::testEncodeDecode()
{
    QFETCH(int, codecId);
    QFETCH(Byte, byte);

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QString digits;
    codec->encode( digits, 0, byte );
    QVERIFY( ! digits.isEmpty() );

    Byte decodedByte;
    const int usedDigits = codec->decode( &decodedByte, digits, 0 );
    QCOMPARE( usedDigits, digits.length() );
    QCOMPARE( decodedByte, byte );

    delete codec;
}

void ValueCodecTest::testEncodeShortDecode_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");

    for( int i = 0; i < valueCodecCount; ++i )
        for( int j = 0; j < 256; ++j )
        {
            const QString rowTitle = valueCodecs[i].name+QString::fromLatin1(" - %1").arg(j);
            QTest::newRow(rowTitle.toLatin1().constData()) << valueCodecs[i].id << Byte(j);
        }
}

void ValueCodecTest::testEncodeShortDecode()
{
    QFETCH(int, codecId);
    QFETCH(Byte, byte);

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QString digits;
    codec->encodeShort( digits, 0, byte );
    QVERIFY( ! digits.isEmpty() );

    Byte decodedByte;
    const int usedDigits = codec->decode( &decodedByte, digits, 0 );
    QCOMPARE( usedDigits, digits.length() );
    QCOMPARE( decodedByte, byte );

    delete codec;
}

}

QTEST_KDEMAIN_CORE( Okteta::ValueCodecTest )
