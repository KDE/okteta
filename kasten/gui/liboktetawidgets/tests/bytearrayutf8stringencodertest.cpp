/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayutf8stringencodertest.hpp"

// test object
#include <bytearrayutf8stringencoder.hpp>
// Qt
#include <QTest>
#include <QByteArray>
#include <QTextCodec>

namespace Okteta {

void ByteArrayUtf8StringEncoderTest::testEncode_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<QString>("expectedEncodeResult");

    QTest::newRow("emptydata")
        << QByteArray() << QString();

    QTest::newRow("A")
        << QByteArrayLiteral("A") << QStringLiteral("A");
    QTest::newRow("z")
        << QByteArrayLiteral("z") << QStringLiteral("z");
    QTest::newRow("xA4")
        << QByteArrayLiteral("\xA4") << QStringLiteral("\\xa4");
    QTest::newRow("backslash")
        << QByteArrayLiteral("\\") << QStringLiteral("\\\\");
    QTest::newRow("newline")
        << QByteArrayLiteral("\n") << QStringLiteral("\\n");
    QTest::newRow("x02")
        << QByteArrayLiteral("\x02")  << QStringLiteral("\\x02");
    QTest::newRow("¤")
        << QByteArrayLiteral("\xC2\xA4") << QStringLiteral("¤");
    QTest::newRow("€")
        << QByteArrayLiteral("\xE2\x82\xAC") << QStringLiteral("€");

    QTest::newRow("combined")
        << QByteArrayLiteral("Az\xA4\\\n\x02\xC2\xA4\xE2\x82\xAC") << QStringLiteral("Az\\xa4\\\\\\n\\x02¤€");
}

void ByteArrayUtf8StringEncoderTest::testEncode()
{
    QFETCH(const QByteArray, data);
    QFETCH(const QString, expectedEncodeResult);

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");

    QVERIFY(codec != nullptr);

    Okteta::ByteArrayUtf8StringEncoder encoder;

    const QString encodeResult = encoder.encodeAsString(data, codec);

    QCOMPARE(encodeResult, expectedEncodeResult);
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayUtf8StringEncoderTest)

#include "moc_bytearrayutf8stringencodertest.cpp"
