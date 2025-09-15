/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraychar8stringencodertest.hpp"

// test object
#include <bytearraychar8stringencoder.hpp>
// Okteta core
#include <Okteta/CharCodec>
// Qt
#include <QTest>
#include <QByteArray>

namespace Okteta {

void ByteArrayChar8StringEncoderTest::testEncode_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<QString>("codecName");
    QTest::addColumn<QString>("expectedEncodeResult");

    const QString iso8859_1 = QStringLiteral("ISO-8859-1");
    const QString iso8859_15 = QStringLiteral("ISO-8859-15");
    const QString usAscii = QStringLiteral("US-ASCII");

    QTest::newRow("emptydata")
        << QByteArray() << iso8859_1 << QString();

    QTest::newRow("A-ISO-8859-1")
        << QByteArrayLiteral("A") << iso8859_1 << QStringLiteral("A");
    QTest::newRow("z-ISO-8859-1")
        << QByteArrayLiteral("z") << iso8859_1 << QStringLiteral("z");
    QTest::newRow("xA4-ISO-8859-1")
        << QByteArrayLiteral("\xA4") << iso8859_1 << QStringLiteral("¤");
    QTest::newRow("xA4-ISO-8859-15")
        << QByteArrayLiteral("\xA4") << iso8859_15 << QStringLiteral("€");
    QTest::newRow("xA4-US-ASCII")
        << QByteArrayLiteral("\xA4") << usAscii << QStringLiteral("\\xa4");
    QTest::newRow("backslash-ISO-8859-1")
        << QByteArrayLiteral("\\") << iso8859_1 << QStringLiteral("\\\\");
    QTest::newRow("newline-ISO-8859-1")
        << QByteArrayLiteral("\n") << iso8859_1 << QStringLiteral("\\n");
    QTest::newRow("x02-US-ASCII")
        << QByteArrayLiteral("\x02")  << usAscii << QStringLiteral("\\x02");

    QTest::newRow("ISO-8859-1")
        << QByteArrayLiteral("Az\xA4\\\n\x02") << iso8859_1 << QStringLiteral("Az¤\\\\\\n\\x02");
    QTest::newRow("US-ASCII")
        << QByteArrayLiteral("Az\xA4\\\n\x02") << usAscii << QStringLiteral("Az\\xa4\\\\\\n\\x02");
}

void ByteArrayChar8StringEncoderTest::testEncode()
{
    QFETCH(const QByteArray, data);
    QFETCH(const QString, codecName);
    QFETCH(const QString, expectedEncodeResult);

    auto codec = Okteta::CharCodec::createCodec(codecName);

    QVERIFY(codec != nullptr);
    QCOMPARE(codec->name(), codecName);

    Okteta::ByteArrayChar8StringEncoder encoder;

    const QString encodeResult = encoder.encodeAsString(data, codec.get());

    QCOMPARE(encodeResult, expectedEncodeResult);
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayChar8StringEncoderTest)

#include "moc_bytearraychar8stringencodertest.cpp"
