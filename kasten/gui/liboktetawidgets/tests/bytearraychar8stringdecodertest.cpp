/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraychar8stringdecodertest.hpp"

// test object
#include <bytearraychar8stringdecoder.hpp>
// Okteta core
#include <Okteta/CharCodec>
// Qt
#include <QTest>
#include <QByteArray>

Q_DECLARE_METATYPE(Okteta::ByteArrayChar8StringDecoder::CodeState)

namespace Okteta {

char* toString(ByteArrayChar8StringDecoder::CodeState state)
{
    const char* text;
    switch (state) {
    case ByteArrayChar8StringDecoder::CodeInvalid: text = "CodeInvalid"; break;
    case ByteArrayChar8StringDecoder::CodeIntermediate: text = "CodeIntermediate"; break;
    case ByteArrayChar8StringDecoder::CodeAcceptable: text = "CodeAcceptable"; break;
    }
    return QTest::toString(text);
}

void ByteArrayChar8StringDecoderTest::testDecode_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("codecName");
    QTest::addColumn<Okteta::ByteArrayChar8StringDecoder::CodeState>("expectedDecodeStateResult");
    QTest::addColumn<QByteArray>("expectedDecodeResult");

    const QString iso8859_1 = QStringLiteral("ISO-8859-1");
    const QString usAscii = QStringLiteral("US-ASCII");

    QTest::newRow("emptystring")
        << QString() << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArray();
    QTest::newRow("A-ISO-8859-1")
        << QStringLiteral("A") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("A");
    QTest::newRow("z-ISO-8859-1")
        << QStringLiteral("z") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("z");
    QTest::newRow("€-ISO-8859-1")
        << QStringLiteral("€") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeInvalid << QByteArray();
    QTest::newRow("AA-ISO-8859-1")
        << QStringLiteral("AA") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("AA");
    QTest::newRow("singlebackslash-ISO-8859-1")
        << QStringLiteral("\\") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeIntermediate << QByteArrayLiteral("\0");
    QTest::newRow("escapedbackslash-ISO-8859-1")
        << QStringLiteral("\\\\") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\\");
    QTest::newRow("newline-ISO-8859-1")
        << QStringLiteral("\\n") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\n");
    QTest::newRow("newline-n-ISO-8859-1")
        << QStringLiteral("\\nn") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\nn");
    QTest::newRow("escaped-A-ISO-8859-1")
        << QStringLiteral("\\A") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeIntermediate << QByteArrayLiteral("\0A");
    QTest::newRow("all-escaped-controls-ISO-8859-1")
        << QStringLiteral("\\\\\\n\\r\\t\\v\\f\\b\\a") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\\\n\r\t\v\f\b\a");
    QTest::newRow("escaped-hex-x-US-ASCII")
        << QStringLiteral("\\x") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeIntermediate << QByteArrayLiteral("\0");
    QTest::newRow("escaped-hex-x2-US-ASCII")
        << QStringLiteral("\\x2") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeIntermediate << QByteArrayLiteral("\x02");
    QTest::newRow("escaped-hex-x02-US-ASCII")
        << QStringLiteral("\\x02") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\x02");
    QTest::newRow("escaped-hex-x2g-US-ASCII")
        << QStringLiteral("\\x2g") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeIntermediate << QByteArrayLiteral("\x02g");
    QTest::newRow("escaped-hex-x20-US-ASCII")
        << QStringLiteral("\\x20") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral(" ");
    QTest::newRow("escaped-hex-x00-US-ASCII")
        << QStringLiteral("\\x00") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\0");
    QTest::newRow("escaped-hex-undefined-US-ASCII")
        << QStringLiteral("\\xFF") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\xFF");
    QTest::newRow("escaped-octal-0-US-ASCII")
        << QStringLiteral("\\0") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeIntermediate << QByteArrayLiteral("\0");
    QTest::newRow("escaped-octal-40-US-ASCII")
        << QStringLiteral("\\40") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\40");
    QTest::newRow("escaped-octal-040-US-ASCII")
        << QStringLiteral("\\040") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\40");
    QTest::newRow("escaped-octal-undefined-US-ASCII")
        << QStringLiteral("\\377") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\377");
    QTest::newRow("escaped-octal-outofbounds-US-ASCII")
        << QStringLiteral("\\444") << usAscii
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\0444");
    QTest::newRow("mixture-ISO-8859-1")
        << QStringLiteral("A\\\\Text\\nABC\\xffABC\\a\\012Z") << iso8859_1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable
        << QByteArrayLiteral("A\\Text\nABC\xff""ABC\a\012Z");
}

void ByteArrayChar8StringDecoderTest::testDecode()
{
    QFETCH(const QString, string);
    QFETCH(const QString, codecName);
    QFETCH(const Okteta::ByteArrayChar8StringDecoder::CodeState, expectedDecodeStateResult);
    QFETCH(const QByteArray, expectedDecodeResult);

    Okteta::ByteArrayChar8StringDecoder decoder;
    decoder.setCharCodec(codecName);

    QByteArray decodeResult;
    int usedStringSize = -1;

    // with target bytearray
    Okteta::ByteArrayChar8StringDecoder::CodeState decodeState = decoder.decode(&decodeResult, string, 0, -1, &usedStringSize);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    QCOMPARE(decodeResult, expectedDecodeResult);
    if (decodeState != Okteta::ByteArrayChar8StringDecoder::CodeInvalid) {
        QCOMPARE(usedStringSize, string.size());
    }

    // without target bytearray
    decodeState = decoder.decode(nullptr, string, 0, -1, &usedStringSize);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    if (decodeState != Okteta::ByteArrayChar8StringDecoder::CodeInvalid) {
        QCOMPARE(usedStringSize, string.size());
    }
}

void ByteArrayChar8StringDecoderTest::testDecodeMaxSize_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<int>("minResultSize");
    QTest::addColumn<int>("maxResultSize");
    QTest::addColumn<int>("expectedUsedStringSize");
    QTest::addColumn<Okteta::ByteArrayChar8StringDecoder::CodeState>("expectedDecodeStateResult");
    QTest::addColumn<QByteArray>("expectedDecodeResult");

    QTest::newRow("emptystring-0-maxsize")
        << QString() << 0 << 0
        << 0
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArray();
    QTest::newRow("somestring-0-maxsize")
        << QStringLiteral("ABC") << 0 << 0
        << 0
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArray();
    QTest::newRow("emptystring-1-minsize")
        << QString() << 1 << 1
        << 0
        << Okteta::ByteArrayChar8StringDecoder::CodeIntermediate<< QByteArray();
    QTest::newRow("somestring-1-maxsize")
        << QStringLiteral("ABC") << 0 << 1
        << 1
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("A");
    QTest::newRow("somestring-2-maxsize")
        << QStringLiteral("ABC") << 0 << 2
        << 2
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("AB");
    QTest::newRow("somestring-all-maxsize")
        << QStringLiteral("ABC") << 0 << 3
        << 3
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("ABC");
    QTest::newRow("someescapedstring-1-maxsize")
        << QStringLiteral("\\x01\\x02\\x03") << 0 << 1
        << 4
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\x01");
    QTest::newRow("someescapedstring-1-maxsize")
        << QStringLiteral("\\x01\\x02\\x03") << 0 << 2
        << 8
        << Okteta::ByteArrayChar8StringDecoder::CodeAcceptable << QByteArrayLiteral("\x01\x02");
}

void ByteArrayChar8StringDecoderTest::testDecodeMaxSize()
{
    QFETCH(const QString, string);
    QFETCH(const int, minResultSize);
    QFETCH(const int, maxResultSize);
    QFETCH(const int, expectedUsedStringSize);
    QFETCH(const Okteta::ByteArrayChar8StringDecoder::CodeState, expectedDecodeStateResult);
    QFETCH(const QByteArray, expectedDecodeResult);

    const QString usAscii = QStringLiteral("US-ASCII");

    Okteta::ByteArrayChar8StringDecoder decoder;
    decoder.setCharCodec(usAscii);

    QByteArray decodeResult;
    int usedStringSize = -1;

    // with target bytearray
    Okteta::ByteArrayChar8StringDecoder::CodeState decodeState = decoder.decode(&decodeResult, string, minResultSize, maxResultSize, &usedStringSize);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    QCOMPARE(decodeResult, expectedDecodeResult);
    if (decodeState != Okteta::ByteArrayChar8StringDecoder::CodeInvalid) {
        QCOMPARE(usedStringSize, expectedUsedStringSize);
    }

    // without target bytearray
    decodeState = decoder.decode(nullptr, string, minResultSize, maxResultSize, &usedStringSize);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    if (decodeState != Okteta::ByteArrayChar8StringDecoder::CodeInvalid) {
        QCOMPARE(usedStringSize, expectedUsedStringSize);
    }
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayChar8StringDecoderTest)

#include "moc_bytearraychar8stringdecodertest.cpp"
