/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayutf8stringdecodertest.hpp"

// test object
#include <bytearrayutf8stringdecoder.hpp>
// Qt
#include <QTest>
#include <QByteArray>
#include <QTextCodec>

Q_DECLARE_METATYPE(Okteta::ByteArrayUtf8StringDecoder::CodeState)

namespace Okteta {

char *toString(ByteArrayUtf8StringDecoder::CodeState state)
{
    const char* text;
    switch (state) {
    case ByteArrayUtf8StringDecoder::CodeInvalid: text = "CodeInvalid"; break;
    case ByteArrayUtf8StringDecoder::CodeIntermediate: text = "CodeIntermediate"; break;
    case ByteArrayUtf8StringDecoder::CodeAcceptable: text = "CodeAcceptable"; break;
    }
    return QTest::toString(text);
}

void ByteArrayUtf8StringDecoderTest::testDecode_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<Okteta::ByteArrayUtf8StringDecoder::CodeState>("expectedDecodeStateResult");
    QTest::addColumn<QByteArray>("expectedDecodeResult");

    QTest::newRow("emptystring")
        << QString()
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArray();
    QTest::newRow("A")
        << QStringLiteral("A")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("A");
    QTest::newRow("z")
        << QStringLiteral("z")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("z");
    QTest::newRow("AA")
        << QStringLiteral("AA")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("AA");
    QTest::newRow("singlebackslash")
        << QStringLiteral("\\")
        << Okteta::ByteArrayUtf8StringDecoder::CodeIntermediate << QByteArrayLiteral("\0");
    QTest::newRow("escapedbackslash")
        << QStringLiteral("\\\\")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\\");
    QTest::newRow("newline")
        << QStringLiteral("\\n")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\n");
    QTest::newRow("newline-n")
        << QStringLiteral("\\nn")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\nn");
    QTest::newRow("escaped-A")
        << QStringLiteral("\\A")
        << Okteta::ByteArrayUtf8StringDecoder::CodeIntermediate << QByteArrayLiteral("\0A");
    QTest::newRow("all-escaped-controls")
        << QStringLiteral("\\\\\\n\\r\\t\\v\\f\\b\\a")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\\\n\r\t\v\f\b\a");
    QTest::newRow("escaped-hex-x")
        << QStringLiteral("\\x")
        << Okteta::ByteArrayUtf8StringDecoder::CodeIntermediate << QByteArrayLiteral("\0");
    QTest::newRow("escaped-hex-x2")
        << QStringLiteral("\\x2")
        << Okteta::ByteArrayUtf8StringDecoder::CodeIntermediate << QByteArrayLiteral("\x02");
    QTest::newRow("escaped-hex-x02")
        << QStringLiteral("\\x02")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\x02");
    QTest::newRow("escaped-hex-x2g")
        << QStringLiteral("\\x2g")
        << Okteta::ByteArrayUtf8StringDecoder::CodeIntermediate << QByteArrayLiteral("\x02g");
    QTest::newRow("escaped-hex-x20")
        << QStringLiteral("\\x20")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral(" ");
    QTest::newRow("escaped-hex-x00")
        << QStringLiteral("\\x00")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\0");
    QTest::newRow("escaped-hex-undefined")
        << QStringLiteral("\\xFF")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\xFF");
    QTest::newRow("escaped-octal-0")
        << QStringLiteral("\\0")
        << Okteta::ByteArrayUtf8StringDecoder::CodeIntermediate << QByteArrayLiteral("\0");
    QTest::newRow("escaped-octal-40")
        << QStringLiteral("\\40")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\40");
    QTest::newRow("escaped-octal-040")
        << QStringLiteral("\\040")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\40");
    QTest::newRow("escaped-octal-undefined")
        << QStringLiteral("\\377")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\377");
    QTest::newRow("escaped-octal-outofbounds")
        << QStringLiteral("\\444")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\0444");
    QTest::newRow("€")
        << QStringLiteral("€")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\xE2\x82\xAC");
    QTest::newRow("mixture")
        << QStringLiteral("A\\\\Text\\nABC\\xffABC\\a\\012Z")
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable
        << QByteArrayLiteral("A\\Text\nABC\xff""ABC\a\012Z");
}

void ByteArrayUtf8StringDecoderTest::testDecode()
{
    QFETCH(const QString, string);
    QFETCH(const Okteta::ByteArrayUtf8StringDecoder::CodeState, expectedDecodeStateResult);
    QFETCH(const QByteArray, expectedDecodeResult);

    Okteta::ByteArrayUtf8StringDecoder decoder;

    QByteArray decodeResult;
    int usedStringSize = -1;

    // with target bytearray
    Okteta::ByteArrayUtf8StringDecoder::CodeState decodeState = decoder.decode(&decodeResult, string, 0, -1, &usedStringSize);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    QCOMPARE(decodeResult, expectedDecodeResult);
    if (decodeState != Okteta::ByteArrayUtf8StringDecoder::CodeInvalid) {
        QCOMPARE(usedStringSize, string.size());
    }

    // without target bytearray
    decodeState = decoder.decode(nullptr, string, 0, -1, &usedStringSize);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    if (decodeState != Okteta::ByteArrayUtf8StringDecoder::CodeInvalid) {
        QCOMPARE(usedStringSize, string.size());
    }
}

void ByteArrayUtf8StringDecoderTest::testDecodeMaxSize_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<int>("minResultSize");
    QTest::addColumn<int>("maxResultSize");
    QTest::addColumn<int>("expectedUsedStringSize");
    QTest::addColumn<Okteta::ByteArrayUtf8StringDecoder::CodeState>("expectedDecodeStateResult");
    QTest::addColumn<QByteArray>("expectedDecodeResult");

    QTest::newRow("emptystring-0-maxsize")
        << QString() << 0 << 0
        << 0
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArray();
    QTest::newRow("somestring-0-maxsize")
        << QStringLiteral("ABC") << 0 << 0
        << 0
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArray();
    QTest::newRow("emptystring-1-minsize")
        << QString() << 1 << 1
        << 0
        << Okteta::ByteArrayUtf8StringDecoder::CodeIntermediate<< QByteArray();
    QTest::newRow("somestring-1-maxsize")
        << QStringLiteral("ABC") << 0 << 1
        << 1
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("A");
    QTest::newRow("somestring-2-maxsize")
        << QStringLiteral("ABC") << 0 << 2
        << 2
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("AB");
    QTest::newRow("somestring-all-maxsize")
        << QStringLiteral("ABC") << 0 << 3
        << 3
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("ABC");
    QTest::newRow("someescapedstring-1-maxsize")
        << QStringLiteral("\\x01\\x02\\x03") << 0 << 1
        << 4
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\x01");
    QTest::newRow("someescapedstring-1-maxsize")
        << QStringLiteral("\\x01\\x02\\x03") << 0 << 2
        << 8
        << Okteta::ByteArrayUtf8StringDecoder::CodeAcceptable << QByteArrayLiteral("\x01\x02");
}

void ByteArrayUtf8StringDecoderTest::testDecodeMaxSize()
{
    QFETCH(const QString, string);
    QFETCH(const int, minResultSize);
    QFETCH(const int, maxResultSize);
    QFETCH(const int, expectedUsedStringSize);
    QFETCH(const Okteta::ByteArrayUtf8StringDecoder::CodeState, expectedDecodeStateResult);
    QFETCH(const QByteArray, expectedDecodeResult);

    Okteta::ByteArrayUtf8StringDecoder decoder;

    QByteArray decodeResult;
    int usedStringSize = -1;

    // with target bytearray
    Okteta::ByteArrayUtf8StringDecoder::CodeState decodeState = decoder.decode(&decodeResult, string, minResultSize, maxResultSize, &usedStringSize);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    QCOMPARE(decodeResult, expectedDecodeResult);
    if (decodeState != Okteta::ByteArrayUtf8StringDecoder::CodeInvalid) {
        QCOMPARE(usedStringSize, expectedUsedStringSize);
    }

    // without target bytearray
    decodeState = decoder.decode(nullptr, string, minResultSize, maxResultSize, &usedStringSize);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    if (decodeState != Okteta::ByteArrayUtf8StringDecoder::CodeInvalid) {
        QCOMPARE(usedStringSize, expectedUsedStringSize);
    }
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayUtf8StringDecoderTest)

#include "moc_bytearrayutf8stringdecodertest.cpp"
