/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charcodeciftest.hpp"

// test object
#include <charcodec.hpp>
// lib
#include <character.hpp>
// Qt
#include <QTest>

namespace Okteta {

void CharCodecIfTest::init()
{
    mCharCodec = createCodec();
}

void CharCodecIfTest::cleanup()
{
    deleteCodec(mCharCodec);
}

void CharCodecIfTest::testEncodeDecode_data()
{
    QTest::addColumn<int>("byteValue");

    for (int i = 0; i < 256; ++i) {
        const QString rowTitle = QStringLiteral("byte %1").arg(i);
        QTest::newRow(rowTitle.toLatin1().constData()) << i;
    }
}

// TODO: is duplication with what is on CharCodecTest
// (though that is complete, TextCharCodecCharCodecIfTest ATM misses to test all TextCharCodecs)
void CharCodecIfTest::testEncodeDecode()
{
    QFETCH(int, byteValue);

    // current assumption: the mapping of chars to byte values is biunique for all used charsets
    const Byte byte = Byte(byteValue);
    Character character = mCharCodec->decode(byte);
    if (!character.isUndefined()) {
        QVERIFY(mCharCodec->canEncode(character));

        Byte encodedByte;
        const bool encodeSuccess = mCharCodec->encode(&encodedByte, character);
        QVERIFY(encodeSuccess);
        QCOMPARE(encodedByte, byte);
    }
}

}
