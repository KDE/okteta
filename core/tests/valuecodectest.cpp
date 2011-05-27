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
// Qt
#include <QtCore/QBitArray>


namespace Okteta
{

//---------------------------------------------------------------------------- Tests -----

// keep in order with ValueCoding
struct ValueCodecDescription
{
    const char* name;
    int id;
    uint encodingWidth;
};

static const ValueCodecDescription valueCodecDescriptions[] =
{
    {"HexadecimalByteCodec", HexadecimalCoding, 2},
    {"DecimalByteCodec", DecimalCoding, 3},
    {"OctalByteCodec", OctalCoding, 3},
    {"BinaryByteCodec", BinaryCoding, 8}
};
static const int valueCodecDescriptionCount =
    sizeof(valueCodecDescriptions)/sizeof(valueCodecDescriptions[0]);


void ValueCodecTest::testCreateCodec_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<uint>("encodingWidth");

    for( int c = 0; c < valueCodecDescriptionCount; ++c )
    {
        const ValueCodecDescription& valueCodecDescription =
            valueCodecDescriptions[c];

        QTest::newRow(valueCodecDescription.name)
            << valueCodecDescription.id
            << valueCodecDescription.encodingWidth;
    }
}

void ValueCodecTest::testCreateCodec()
{
    QFETCH(int, codecId);
    QFETCH(uint, encodingWidth);

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QVERIFY( codec != 0 );
    QCOMPARE( codec->encodingWidth(), encodingWidth );

    delete codec;
}

void ValueCodecTest::testEncodeDecode_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");

    for( int c = 0; c < valueCodecDescriptionCount; ++c )
    {
        const ValueCodecDescription& valueCodecDescription =
            valueCodecDescriptions[c];
        for( int j = 0; j < 256; ++j )
        {
            const QString rowTitle =
                QLatin1String(valueCodecDescription.name) +
                QString::fromLatin1(" - %1").arg( j );

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

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QString digits;
    codec->encode( digits, 0, byte );
    QCOMPARE( digits.length(), (int)codec->encodingWidth() );

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

    for( int c = 0; c < valueCodecDescriptionCount; ++c )
    {
        const ValueCodecDescription& valueCodecDescription =
            valueCodecDescriptions[c];

        for( int j = 0; j < 256; ++j )
        {
            const QString rowTitle =
                QLatin1String(valueCodecDescription.name) +
                QString::fromLatin1(" - %1").arg(j);

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

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QString digits;
    codec->encodeShort( digits, 0, byte );
    QVERIFY( ! digits.isEmpty() );
    QVERIFY( digits.length() <= (int)codec->encodingWidth() );

    Byte decodedByte;
    const int usedDigits = codec->decode( &decodedByte, digits, 0 );
    QCOMPARE( usedDigits, digits.length() );
    QCOMPARE( decodedByte, byte );

    delete codec;
}

void ValueCodecTest::testAppendDigit_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");

    for( int c = 0; c < valueCodecDescriptionCount; ++c )
    {
        const ValueCodecDescription& valueCodecDescription =
            valueCodecDescriptions[c];

        for( int j = 0; j < 256; ++j )
        {
            const QString rowTitle =
                QLatin1String(valueCodecDescription.name) +
                QString::fromLatin1(" - %1").arg(j);

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

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QString digits;
    codec->encode( digits, 0, byte );

    Byte decodedByte = 0;
    for( int i = 0; i < digits.length(); ++i )
        codec->appendDigit( &decodedByte, digits[i].toLatin1() );

    QCOMPARE( decodedByte, byte );

    delete codec;
}

void ValueCodecTest::testRemoveLastDigit_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<Byte>("byte");
    QTest::addColumn<uint>("removedDigitCount");

    for( int c = 0; c < valueCodecDescriptionCount; ++c )
    {
        const ValueCodecDescription& valueCodecDescription =
            valueCodecDescriptions[c];

        for( int b = 0; b < 256; ++b )
        {
            for( uint r = 1; r <= valueCodecDescription.encodingWidth; ++r )
            {
                const QString rowTitle =
                    QLatin1String(valueCodecDescription.name) +
                    QString::fromLatin1(" - %1 - removed last %2").arg(b).arg(r);

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

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QString digits;
    codec->encode( digits, 0, byte );

    Byte modifiedByte = byte;
    for( uint i = 0; i < removedDigitCount; ++i )
        codec->removeLastDigit( &modifiedByte );

    QString modifiedDigits;
    codec->encode( modifiedDigits, 0, modifiedByte );

    QVERIFY( digits.startsWith(modifiedDigits.mid(removedDigitCount)) );
    QVERIFY( modifiedDigits.startsWith(QString(removedDigitCount,QLatin1Char('0'))) );

    delete codec;
}

// keep in order with ValueCoding
static const char * const validDigitsPerCodec[] =
{
    "0123456789ABCDEFabcdef",
    "0123456789",
    "01234567",
    "01"
};
static const int validDigitsPerCodecCount =
    sizeof(validDigitsPerCodec)/sizeof(validDigitsPerCodec[0]);

void ValueCodecTest::testIsValidDigit_data()
{
    QTest::addColumn<int>("codecId");
    QTest::addColumn<uchar>("digit");
    QTest::addColumn<bool>("isValid");

    static const int digitCount = 256;

    for( int c = 0; c < valueCodecDescriptionCount; ++c )
    {
        const ValueCodecDescription& valueCodecDescription =
            valueCodecDescriptions[c];

        QBitArray validnessPerDigitField = QBitArray( digitCount, false );
        const QByteArray validDigits =
            QByteArray(validDigitsPerCodec[c]);

        for( int j = 0; j < validDigits.count(); ++j )
            validnessPerDigitField.setBit( validDigits[j], true );

        for( int j = 0; j < validnessPerDigitField.count(); ++j )
        {
            const uchar digit = uchar( j );
            const bool isValid = validnessPerDigitField.testBit( j );
            const QString rowTitle =
                QLatin1String(valueCodecDescription.name) +
                QString::fromLatin1(" - \"%1\" is ").arg(QLatin1Char(digit)) +
                QLatin1String(isValid ? "valid" : "invalid");

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

    ValueCodec* codec = ValueCodec::createCodec( (ValueCoding)codecId);

    QCOMPARE( codec->isValidDigit(digit), isValid );

    delete codec;
}

}

QTEST_KDEMAIN_CORE( Okteta::ValueCodecTest )
