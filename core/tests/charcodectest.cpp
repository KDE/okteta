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

#include "charcodectest.h"

// test object
#include <charcodec.h>
// lib
#include <character.h>
// KDE
#include <qtest_kde.h>


namespace Okteta
{

//---------------------------------------------------------------------------- Tests -----

void CharCodecTest::testCreateCodec_data()
{
    QTest::addColumn<QString>("codecName");

    foreach( const QString& codecName, CharCodec::codecNames() )
        QTest::newRow(codecName.toLatin1().constData()) << codecName;
}

void CharCodecTest::testCreateCodec()
{
    QFETCH(QString, codecName);

    CharCodec* codec = CharCodec::createCodec( codecName );

    QVERIFY( codec != 0 );
    QCOMPARE( codec->name(), codecName );

    delete codec;
}

void CharCodecTest::testEncodeDecode_data()
{
    QTest::addColumn<QString>("codecName");
    QTest::addColumn<int>("byteValue");

    foreach( const QString& codecName, CharCodec::codecNames() )
        for( int i = 0; i < 256; ++i )
        {
            const QString rowTitle = codecName+QString::fromLatin1(" - %1").arg(i);
            QTest::newRow(rowTitle.toLatin1().constData()) << codecName << i;
        }
}

void CharCodecTest::testEncodeDecode()
{
    QFETCH(QString, codecName);
    QFETCH(int, byteValue);

    CharCodec* codec = CharCodec::createCodec( codecName );

    // current assumption: the mapping of chars to byte values is biunique for all used charsets
    const Byte byte = Byte( byteValue );
    Character character = codec->decode( byte );
    if( ! character.isUndefined() )
    {
        QVERIFY( codec->canEncode(character) );

        Byte encodedByte;
        const bool encodeSuccess = codec->encode( &encodedByte, character );
        QVERIFY( encodeSuccess );
        QCOMPARE( encodedByte, byte );
    }

    delete codec;
}

}

QTEST_KDEMAIN_CORE( Okteta::CharCodecTest )
