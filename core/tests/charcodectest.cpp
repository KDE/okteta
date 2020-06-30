/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charcodectest.hpp"

// test object
#include <charcodec.hpp>
// lib
#include <character.hpp>
// KF
#include <QTest>

namespace Okteta {

//---------------------------------------------------------------------------- Tests -----

void CharCodecTest::testCreateCodec_data()
{
    QTest::addColumn<QString>("codecName");

    for (const QString& codecName : CharCodec::codecNames()) {
        QTest::newRow(codecName.toLatin1().constData()) << codecName;
    }
}

void CharCodecTest::testCreateCodec()
{
    QFETCH(QString, codecName);

    CharCodec* codec = CharCodec::createCodec(codecName);

    QVERIFY(codec != nullptr);
    QCOMPARE(codec->name(), codecName);

    delete codec;
}

void CharCodecTest::testEncodeDecode_data()
{
    QTest::addColumn<QString>("codecName");
    QTest::addColumn<int>("byteValue");

    for (const QString& codecName : CharCodec::codecNames()) {
        for (int i = 0; i < 256; ++i) {
            const QString rowTitle = codecName + QStringLiteral(" - %1").arg(i);
            QTest::newRow(rowTitle.toLatin1().constData()) << codecName << i;
        }
    }
}

void CharCodecTest::testEncodeDecode()
{
    QFETCH(QString, codecName);
    QFETCH(int, byteValue);

    CharCodec* codec = CharCodec::createCodec(codecName);

    // current assumption: the mapping of chars to byte values is biunique for all used charsets
    const Byte byte = Byte(byteValue);
    Character character = codec->decode(byte);
    if (!character.isUndefined()) {
        QVERIFY(codec->canEncode(character));

        Byte encodedByte;
        const bool encodeSuccess = codec->encode(&encodedByte, character);
        QVERIFY(encodeSuccess);
        QCOMPARE(encodedByte, byte);
    }

    delete codec;
}

}

QTEST_GUILESS_MAIN(Okteta::CharCodecTest)
