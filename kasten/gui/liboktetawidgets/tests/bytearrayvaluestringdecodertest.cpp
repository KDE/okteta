/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayvaluestringdecodertest.hpp"

// test object
#include <bytearrayvaluestringdecoder.hpp>
// Okteta core
#include <Okteta/ValueCodec>
// Qt
#include <QTest>
#include <QByteArray>

Q_DECLARE_METATYPE(Okteta::ByteArrayValueStringDecoder::CodeState)
Q_DECLARE_METATYPE(Okteta::ValueCoding)

namespace Okteta {

char* toString(ByteArrayValueStringDecoder::CodeState state)
{
    const char* text;
    switch (state) {
    case ByteArrayValueStringDecoder::CodeInvalid: text = "CodeInvalid"; break;
    case ByteArrayValueStringDecoder::CodeIntermediate: text = "CodeIntermediate"; break;
    case ByteArrayValueStringDecoder::CodeAcceptable: text = "CodeAcceptable"; break;
    }
    return QTest::toString(text);
}

void ByteArrayValueStringDecoderTest::testDecode_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<Okteta::ValueCoding>("valueCoding");
    QTest::addColumn<Okteta::ByteArrayValueStringDecoder::CodeState>("expectedDecodeStateResult");
    QTest::addColumn<QByteArray>("expectedDecodeResult");

    QTest::newRow("emptystring-hex")
        << QString() << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArray();
    QTest::newRow("0-hex")
        << QStringLiteral("0") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0");
    QTest::newRow("0-binary")
        << QStringLiteral("0") << Okteta::BinaryCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0");
    QTest::newRow("00-hex")
        << QStringLiteral("00") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0");
    QTest::newRow("F-hex")
        << QStringLiteral("F") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\x0f");
    QTest::newRow("F-hex")
        << QStringLiteral("F") << Okteta::DecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeInvalid << QByteArray();
    QTest::newRow("FF-hex")
        << QStringLiteral("FF") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\xff");
    QTest::newRow("Z-hex")
        << QStringLiteral("Z") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeInvalid << QByteArray();
    QTest::newRow("0Z-hex")
        << QStringLiteral("0Z") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeInvalid << QByteArrayLiteral("\0");
    QTest::newRow("00FFF0-hex")
        << QStringLiteral("00FFF0") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0\xff\xf0");
    QTest::newRow("000102030405-hex-spaced-fullvalues")
        << QStringLiteral("00 01\t02\r03\n04\t\r\n 05") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0\x01\x02\x03\x04\x05");
    QTest::newRow("000102030405-decimal-spaced-mixedfullvalues")
        << QStringLiteral("000 1\t02\r003\n4\t\r\n 05") << Okteta::DecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0\x01\x02\x03\x04\x05");
    QTest::newRow("00FFF0-hex-badlyspaced")
        << QStringLiteral("00FF F0") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeInvalid << QByteArrayLiteral("\0\xff");
    QTest::newRow("00FFF0-hex-leadingspaces")
        << QStringLiteral("\t\r\n 00FFF0") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0\xff\xf0");
    QTest::newRow("00FFF0-hex-trailingspaces")
        << QStringLiteral("00FFF0\t\r\n ") << Okteta::HexadecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0\xff\xf0");
    QTest::newRow("000102030405-decimal-leadingtrailinginternalspaced-mixedfullvalues")
        << QStringLiteral("\t\r\n 000 1\t02\r003\n4\t\r\n 05\t\r\n ") << Okteta::DecimalCoding
        << Okteta::ByteArrayValueStringDecoder::CodeAcceptable << QByteArrayLiteral("\0\x01\x02\x03\x04\x05");
}

void ByteArrayValueStringDecoderTest::testDecode()
{
    QFETCH(const QString, string);
    QFETCH(const Okteta::ValueCoding, valueCoding);
    QFETCH(const Okteta::ByteArrayValueStringDecoder::CodeState, expectedDecodeStateResult);
    QFETCH(const QByteArray, expectedDecodeResult);

    auto valueCodec = Okteta::ValueCodec::createCodec(valueCoding);
    Okteta::ByteArrayValueStringDecoder decoder(valueCodec.get());

    QByteArray decodeResult;

    Okteta::ByteArrayValueStringDecoder::CodeState decodeState = decoder.decode(&decodeResult, string);

    QCOMPARE(decodeState, expectedDecodeStateResult);
    QCOMPARE(decodeResult, expectedDecodeResult);
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayValueStringDecoderTest)

#include "moc_bytearrayvaluestringdecodertest.cpp"
