/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "valuecodectest.hpp"

// test object
#include <valuecodec.hpp>
// Qt
#include <QTest>
#include <QBitArray>

namespace Okteta {

//---------------------------------------------------------------------------- Tests -----

// keep in order with ValueCoding
struct ValueCodecDescription
{
    const char* name;
    int id;
    uint encodingWidth;
    const char* validDigits;
};

static constexpr ValueCodecDescription valueCodecDescriptions[] =
{
    {"HexadecimalByteCodec", HexadecimalCoding, 2, "0123456789ABCDEFabcdef"},
    {"DecimalByteCodec", DecimalCoding, 3, "0123456789"},
    {"OctalByteCodec", OctalCoding, 3, "01234567"},
    {"BinaryByteCodec", BinaryCoding, 8, "01"}
};
void ValueCodecTest::testCreateCodec_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<uint>("encodingWidth");

    for (auto& valueCodecDescription : valueCodecDescriptions) {
        QTest::newRow(valueCodecDescription.name)
            << valueCodecDescription.id
            << valueCodecDescription.encodingWidth;
    }
}

void ValueCodecTest::testCreateCodec()
{
    QFETCH(int, codecId);
    QFETCH(uint, encodingWidth);

    ValueCodec* codec = ValueCodec::createCodec((ValueCoding)codecId);

    QVERIFY(codec != nullptr);
    QCOMPARE(codec->encodingWidth(), encodingWidth);

    delete codec;
}

void ValueCodecTest::testEncodeDecode_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");

    for (auto& valueCodecDescription : valueCodecDescriptions) {
        for (int j = 0; j < 256; ++j) {
            const QString rowTitle =
                QLatin1String(valueCodecDescription.name) +
                QStringLiteral(" - %1").arg(j);

            QTest::newRow(rowTitle.toLatin1().constData())
                << valueCodecDescription.id
                << Byte(j);
        }
    }
}

void ValueCodecTest::testEncodeDecode()
{
    QFETCH(int, codecId);
    QFETCH(Byte, byte);

    ValueCodec* codec = ValueCodec::createCodec((ValueCoding)codecId);

    QString digits;
    codec->encode(&digits, 0, byte);
    QCOMPARE(digits.length(), (int)codec->encodingWidth());

    Byte decodedByte;
    const int usedDigits = codec->decode(&decodedByte, digits, 0);
    QCOMPARE(usedDigits, digits.length());
    QCOMPARE(decodedByte, byte);

    delete codec;
}

void ValueCodecTest::testEncodeShortDecode_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");

    for (auto& valueCodecDescription : valueCodecDescriptions) {
        for (int j = 0; j < 256; ++j) {
            const QString rowTitle =
                QLatin1String(valueCodecDescription.name) +
                QStringLiteral(" - %1").arg(j);

            QTest::newRow(rowTitle.toLatin1().constData())
                << valueCodecDescription.id
                << Byte(j);
        }
    }
}

void ValueCodecTest::testEncodeShortDecode()
{
    QFETCH(int, codecId);
    QFETCH(Byte, byte);

    ValueCodec* codec = ValueCodec::createCodec((ValueCoding)codecId);

    QString digits;
    codec->encodeShort(&digits, 0, byte);
    QVERIFY(!digits.isEmpty());
    QVERIFY(digits.length() <= (int)codec->encodingWidth());

    Byte decodedByte;
    const int usedDigits = codec->decode(&decodedByte, digits, 0);
    QCOMPARE(usedDigits, digits.length());
    QCOMPARE(decodedByte, byte);

    delete codec;
}

void ValueCodecTest::testAppendDigit_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");

    for (auto& valueCodecDescription : valueCodecDescriptions) {
        for (int j = 0; j < 256; ++j) {
            const QString rowTitle =
                QLatin1String(valueCodecDescription.name) +
                QStringLiteral(" - %1").arg(j);

            QTest::newRow(rowTitle.toLatin1().constData())
                << valueCodecDescription.id
                << Byte(j);
        }
    }
}

void ValueCodecTest::testAppendDigit()
{
    QFETCH(int, codecId);
    QFETCH(Byte, byte);

    ValueCodec* codec = ValueCodec::createCodec((ValueCoding)codecId);

    QString digits;
    codec->encode(&digits, 0, byte);

    Byte decodedByte = 0;
    for (auto d : qAsConst(digits)) {
        codec->appendDigit(&decodedByte, d.toLatin1());
    }

    QCOMPARE(decodedByte, byte);

    delete codec;
}

void ValueCodecTest::testRemoveLastDigit_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");
    QTest::addColumn<uint>("removedDigitCount");

    for (auto& valueCodecDescription : valueCodecDescriptions) {
        for (int b = 0; b < 256; ++b) {
            for (uint r = 1; r <= valueCodecDescription.encodingWidth; ++r) {
                const QString rowTitle =
                    QLatin1String(valueCodecDescription.name) +
                    QStringLiteral(" - %1 - removed last %2").arg(b).arg(r);

                QTest::newRow(rowTitle.toLatin1().constData())
                    << valueCodecDescription.id
                    << Byte(b)
                    << r;
            }
        }
    }
}

void ValueCodecTest::testRemoveLastDigit()
{
    QFETCH(int, codecId);
    QFETCH(Byte, byte);
    QFETCH(uint, removedDigitCount);

    ValueCodec* codec = ValueCodec::createCodec((ValueCoding)codecId);

    QString digits;
    codec->encode(&digits, 0, byte);

    Byte modifiedByte = byte;
    for (uint i = 0; i < removedDigitCount; ++i) {
        codec->removeLastDigit(&modifiedByte);
    }

    QString modifiedDigits;
    codec->encode(&modifiedDigits, 0, modifiedByte);

    QVERIFY(digits.startsWith(modifiedDigits.mid(removedDigitCount)));
    QVERIFY(modifiedDigits.startsWith(QString(removedDigitCount, QLatin1Char('0'))));

    delete codec;
}

void ValueCodecTest::testIsValidDigit_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<uchar>("digit");
    QTest::addColumn<bool>("isValid");

    constexpr int digitCount = 256;

    for (auto& valueCodecDescription : valueCodecDescriptions) {
        QBitArray validnessPerDigitField = QBitArray(digitCount, false);
        const QByteArray validDigits =
            QByteArray(valueCodecDescription.validDigits);

        for (int j = 0; j < validDigits.count(); ++j) {
            validnessPerDigitField.setBit(validDigits[j], true);
        }

        for (int j = 0; j < validnessPerDigitField.count(); ++j) {
            const uchar digit = uchar(j);
            const bool isValid = validnessPerDigitField.testBit(j);
            const QString rowTitle =
                QLatin1String(valueCodecDescription.name) +
                QStringLiteral(" - \"%1\" is ").arg(QLatin1Char(digit)) +
                (isValid ? QStringLiteral("valid") : QStringLiteral("invalid"));

            QTest::newRow(rowTitle.toLatin1().constData())
                << valueCodecDescription.id
                << digit
                << isValid;
        }
    }
}

void ValueCodecTest::testIsValidDigit()
{
    QFETCH(int, codecId);
    QFETCH(uchar, digit);
    QFETCH(bool, isValid);

    ValueCodec* codec = ValueCodec::createCodec((ValueCoding)codecId);

    QCOMPARE(codec->isValidDigit(digit), isValid);

    delete codec;
}

}

QTEST_GUILESS_MAIN(Okteta::ValueCodecTest)
