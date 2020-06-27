/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *    SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <KLocalizedString>

#include <QTest>
#include <QtGlobal>
#include <QLocale>

#include <limits>
#include <Okteta/ByteArrayModel>

#include "view/structures/datatypes/primitive/primitivetemplateinfo.hpp"
#include "view/structures/datatypes/primitive/primitivedatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/signedbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/boolbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitivefactory.hpp"
#include "structureviewpreferences.h"

inline uint qHash(PrimitiveDataType type)
{
    return ::qHash(static_cast<int>(type));
}

class PrimitiveDataInformationTest : public QObject
{
    Q_OBJECT

private:
    template <typename s, typename u>
    void addRowsGetAndSetSigned(PrimitiveDataType type, const char* name);
    template <typename T>
    void addRowsGetAndSetUnsigned(PrimitiveDataType type, const char* name);
    void checkSignedDisplayBase(int expected);
    void checkUnsignedDisplayBase(int expected);
    int minimumSignedBits(qint64 value); // get the least number of bits this can be represented in
    int minimumUnsignedBits(quint64 value); // get the least number of bits this can be represented in

private Q_SLOTS:
    void initTestCase();
    void testValueStringFloat();
    void testValueStringDouble();
    void testValueStringChar();
    void testValueStringInt();
    void testValueStringInt_data();
    void testValueStringUIntAndBool();
    void testValueStringUIntAndBool_data();
    void testToAndFromVariant();
    void testGetAndSetValue();
    void testGetAndSetValue_data();
    void testFromVariant();
    void cleanupTestCase();

private:
    QHash<PrimitiveDataType, PrimitiveDataInformation*> basic;
    SignedBitfieldDataInformation* signedBitfield;
    UnsignedBitfieldDataInformation* unsignedBitfield;
    BoolBitfieldDataInformation* boolBitfield;
    // TODO enum
//     Okteta::Byte* data;
//     QScopedPointer<Okteta::ByteArrayModel> model;
};

Q_DECLARE_METATYPE(PrimitiveDataInformation*)
Q_DECLARE_METATYPE(AllPrimitiveTypes)

int PrimitiveDataInformationTest::minimumSignedBits(qint64 value)
{
    if (value == 0) {
        return 1;
    }
    if (value >= 0) {
        return minimumUnsignedBits(quint64(value)) + 1;
    }
    if (value == -1) { // has no zero in it -> can't determine size
        return 1;
    }
    // find the first zero
    for (int i = 63; i >= 0; --i) {
        if ((value & (qint64(1) << i)) == 0) {
            return i + 2; // found first zero, we need one bit more
        }
    }

    QTEST_ASSERT(false);
    return 65;
}

int PrimitiveDataInformationTest::minimumUnsignedBits(quint64 value)
{
    if (value == 0) {
        return 1;
    }
    for (int i = 63; i >= 0; --i) {
        if ((value & (quint64(1) << i)) == (quint64(1) << i)) {
            return i + 1;
        }
    }

    QTEST_ASSERT(false);
    return 65;
}

void PrimitiveDataInformationTest::initTestCase()
{
    KLocalizedString::setLanguages(QStringList() << QStringLiteral("C"));
    Kasten::StructureViewPreferences::setLocaleAwareDecimalFormatting(false); // this could mess with our expected results
    Kasten::StructureViewPreferences::setLocaleAwareFloatFormatting(false); // this could mess with our expected results
    QLocale locale(QLocale::C);
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    QLocale::setDefault(locale);

    LoggerWithContext lwc(nullptr, QString());

    PrimitiveDataType type = PrimitiveDataType::START;
    while (type < PrimitiveDataType::Bitfield) {
        basic.insert(type, PrimitiveFactory::newInstance(QStringLiteral("prim"), type, lwc));
        type = static_cast<PrimitiveDataType>(static_cast<int>(type) + 1);
    }
    boolBitfield = new BoolBitfieldDataInformation(QStringLiteral("bitfield"), 24);
    unsignedBitfield = new UnsignedBitfieldDataInformation(QStringLiteral("bitfield"), 24);
    signedBitfield = new SignedBitfieldDataInformation(QStringLiteral("bitfield"), 24);
}

namespace {
template <PrimitiveDataType Type>
void valueCompareHelper(typename PrimitiveInfo<Type>::valueType value, QString bin,
                        QString hex, QString dec, QString oct)
{
    QCOMPARE(PrimitiveInfo<Type>::Methods::staticValueString(value, 2), bin);
    QCOMPARE(PrimitiveInfo<Type>::Methods::staticValueString(value, 16), hex);
    QCOMPARE(PrimitiveInfo<Type>::Methods::staticValueString(value, 10), dec);
    QCOMPARE(PrimitiveInfo<Type>::Methods::staticValueString(value, 8), oct);
}

template <PrimitiveDataType first, PrimitiveDataType second>
void valueCompareHelperUnsigned(typename PrimitiveInfo<first>::valueType value, const QString& bin,
                                const QString& hex, const QString& dec, const QString& oct, const QString& boolBase)
{
    QCOMPARE(PrimitiveInfo<first>::Methods::staticValueString(value, 2), bin);
    QCOMPARE(PrimitiveInfo<first>::Methods::staticValueString(value, 16), hex);
    QCOMPARE(PrimitiveInfo<first>::Methods::staticValueString(value, 10), dec);
    QCOMPARE(PrimitiveInfo<first>::Methods::staticValueString(value, 8), oct);

    QCOMPARE(PrimitiveInfo<second>::Methods::staticValueString(value, 2),
             value > 1 ? boolBase.arg(bin) : boolBase);
    QCOMPARE(PrimitiveInfo<second>::Methods::staticValueString(value, 16),
             value > 1 ? boolBase.arg(hex) : boolBase);
    QCOMPARE(PrimitiveInfo<second>::Methods::staticValueString(value, 10),
             value > 1 ? boolBase.arg(dec) : boolBase);
    QCOMPARE(PrimitiveInfo<second>::Methods::staticValueString(value, 8),
             value > 1 ? boolBase.arg(oct) : boolBase);
}

}

void PrimitiveDataInformationTest::testValueStringInt()
{
    QFETCH(qint64, value);
    QFETCH(QString, binStr);
    QFETCH(QString, hexStr);
    QFETCH(QString, decStr);
    QFETCH(QString, octStr);

    int minSize = minimumSignedBits(value);
    // qDebug() << "need" << minSize << "bit to represent" << value;
    // run once with locale aware, and once without
    for (int i = 0; i <= 1; ++i) {
        Kasten::StructureViewPreferences::setLocaleAwareDecimalFormatting(bool(i));

        if (minSize <= 8) {
            valueCompareHelper<PrimitiveDataType::Int8>(qint8(value), binStr, hexStr, decStr, octStr);
        }
        if (minSize <= 16) {
            valueCompareHelper<PrimitiveDataType::Int16>(qint16(value), binStr, hexStr, decStr, octStr);
        }
        if (minSize <= 32) {
            valueCompareHelper<PrimitiveDataType::Int32>(qint32(value), binStr, hexStr, decStr, octStr);
        }
        if (minSize <= 64) {
            valueCompareHelper<PrimitiveDataType::Int64>(qint64(value), binStr, hexStr, decStr, octStr);
        }

        // check bitfield now
        SignedBitfieldDataInformation bitfield(QStringLiteral("signed"), minSize);
        bitfield.setValue(value);
        for (uint width = minSize; width <= 64u; ++width) {
            bitfield.setWidth(width);
            bitfield.mWasAbleToRead = true;
            Kasten::StructureViewPreferences::setSignedDisplayBase(2);
            QCOMPARE(bitfield.valueString(), binStr);
            Kasten::StructureViewPreferences::setSignedDisplayBase(16);
            QCOMPARE(bitfield.valueString(), hexStr);
            Kasten::StructureViewPreferences::setSignedDisplayBase(10);
            QCOMPARE(bitfield.valueString(), decStr);
            Kasten::StructureViewPreferences::setSignedDisplayBase(8);
            QCOMPARE(bitfield.valueString(), octStr);
        }
    }
}

void PrimitiveDataInformationTest::testValueStringInt_data()
{
    QTest::addColumn<qint64>("value");
    QTest::addColumn<QString>("binStr");
    QTest::addColumn<QString>("hexStr");
    QTest::addColumn<QString>("decStr");
    QTest::addColumn<QString>("octStr");

    QTest::newRow("0") << qint64(0) << "0b0" << "0x0" << "0" << "0o0";
    QTest::newRow("1") << qint64(1) << "0b1" << "0x1" << "1" << "0o1";
    QTest::newRow("2") << qint64(2) << "0b10" << "0x2" << "2" << "0o2";
    QTest::newRow("32") << qint64(32) << "0b100000" << "0x20" << "32" << "0o40";
    QTest::newRow("79") << qint64(79) << "0b1001111" << "0x4f" << "79" << "0o117";
    QTest::newRow("172") << qint64(172) << "0b10101100" // 8 chars -> no space
                         << "0xac" << "172" << "0o254";
    QTest::newRow("259") << qint64(259) << "0b1 00000011" // 9 chars -> space
                         << "0x103" << "259" << "0o403";
    QTest::newRow("50448") << qint64(50448) << "0b11000101 00010000" // 16 chars -> no space
                           << "0xc510" << "50448" << "0o142420";
    QTest::newRow("126832") << qint64(126832) << "0b1 11101111 01110000" // 17 chars -> space
                            << "0x1ef70" << "126832" << "0o367560";
    // maximum
    QTest::newRow("qint8::max()") << qint64(std::numeric_limits<qint8>::max())
                                  << "0b1111111" << "0x7f" << "127" << "0o177";
    QTest::newRow("qint16::max()") << qint64(std::numeric_limits<qint16>::max())
                                   << "0b1111111 11111111" << "0x7fff" << "32767" << "0o77777";
    QTest::newRow("qint32::max()") << qint64(std::numeric_limits<qint32>::max())
                                   << "0b1111111 11111111 11111111 11111111" << "0x7fffffff" << "2147483647"
                                   << "0o177 77777777";
    QTest::newRow("qint64::max()") << qint64(std::numeric_limits<qint64>::max())
                                   << "0b1111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111"
                                   << "0x7fffffff ffffffff" << "9223372036854775807" << "0o77777 77777777 77777777";
    // negative
    QTest::newRow("-1") << qint64(-1) << "-0b1" << "-0x1" << "-1" << "-0o1";
    QTest::newRow("-2") << qint64(-2) << "-0b10" << "-0x2" << "-2" << "-0o2";
    QTest::newRow("-32") << qint64(-32) << "-0b100000" << "-0x20" << "-32" << "-0o40";
    QTest::newRow("-79") << qint64(-79) << "-0b1001111" << "-0x4f" << "-79" << "-0o117";
    QTest::newRow("-172") << qint64(-172) << "-0b10101100" << "-0xac" << "-172" << "-0o254";
    QTest::newRow("-259") << qint64(-259) << "-0b1 00000011" << "-0x103" << "-259" << "-0o403";
    ;
    QTest::newRow("-50448") << qint64(-50448) << "-0b11000101 00010000"
                            << "-0xc510" << "-50448" << "-0o142420";
    QTest::newRow("-126832") << qint64(-126832) << "-0b1 11101111 01110000"
                             << "-0x1ef70" << "-126832" << "-0o367560";
    // -1 * postive maximum
    QTest::newRow("-qint8::max()") << qint64(-std::numeric_limits<qint8>::max())
                                   << "-0b1111111" << "-0x7f" << "-127" << "-0o177";
    QTest::newRow("-qint16::max()") << qint64(-std::numeric_limits<qint16>::max())
                                    << "-0b1111111 11111111" << "-0x7fff" << "-32767" << "-0o77777";
    ;
    QTest::newRow("-qint32::max()") << qint64(-std::numeric_limits<qint32>::max())
                                    << "-0b1111111 11111111 11111111 11111111" << "-0x7fffffff" << "-2147483647"
                                    << "-0o177 77777777";
    QTest::newRow("-qint64::max()") << qint64(-std::numeric_limits<qint64>::max())
                                    << "-0b1111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111"
                                    << "-0x7fffffff ffffffff" << "-9223372036854775807" << "-0o77777 77777777 77777777";

    // negative minimum
    QTest::newRow("qint8::min()") << qint64(std::numeric_limits<qint8>::min())
                                  << "-0b10000000" << "-0x80" << "-128" << "-0o200";
    QTest::newRow("qint16::min()") << qint64(std::numeric_limits<qint16>::min())
                                   << "-0b10000000 00000000" << "-0x8000" << "-32768" << "-0o100000";
    QTest::newRow("qint32::min()") << qint64(std::numeric_limits<qint32>::min())
                                   << "-0b10000000 00000000 00000000 00000000" << "-0x80000000" << "-2147483648"
                                   << "-0o200 00000000";
    QTest::newRow("qint64::min()") << qint64(std::numeric_limits<qint64>::min())
                                   << "-0b10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000"
                                   << "-0x80000000 00000000" << "-9223372036854775808" << "-0o100000 00000000 00000000";
}

void PrimitiveDataInformationTest::testFromVariant()
{
    bool ok = false;
    FloatDataInformationMethods::fromVariant(QVariant(float(42.0f)), &ok);
    QCOMPARE(ok, true); // float in range
    FloatDataInformationMethods::fromVariant(QVariant(double(42.0)), &ok);
    QCOMPARE(ok, true); // double should be fine too
    FloatDataInformationMethods::fromVariant(QVariant(double(std::numeric_limits<float>::max())), &ok);
    QCOMPARE(ok, true); // float max as a double should work too
    FloatDataInformationMethods::fromVariant(QVariant(std::numeric_limits<float>::quiet_NaN()), &ok);
    QCOMPARE(ok, true); // nan should be fine too
    FloatDataInformationMethods::fromVariant(QVariant(std::numeric_limits<double>::quiet_NaN()), &ok);
    QCOMPARE(ok, true); // double nan gets mapped to float nan
    FloatDataInformationMethods::fromVariant(QVariant(QStringLiteral("abc")), &ok);
    QCOMPARE(ok, false); // bad data type
    FloatDataInformationMethods::fromVariant(QVariant(std::numeric_limits<double>::max()), &ok);
    QCOMPARE(ok, false); // out of range

    DoubleDataInformationMethods::fromVariant(QVariant(float(42.0f)), &ok);
    QCOMPARE(ok, true); // float should be fine too QVariant::type() == Float
    DoubleDataInformationMethods::fromVariant(QVariant(std::numeric_limits<double>::max()), &ok);
    QCOMPARE(ok, true); // double
    DoubleDataInformationMethods::fromVariant(QVariant(std::numeric_limits<float>::quiet_NaN()), &ok);
    QCOMPARE(ok, true); // nan should be fine too
    DoubleDataInformationMethods::fromVariant(QVariant(std::numeric_limits<double>::quiet_NaN()), &ok);
    QCOMPARE(ok, true); // double nan gets mapped to float nan
    DoubleDataInformationMethods::fromVariant(QVariant(QStringLiteral("abc")), &ok);
    QCOMPARE(ok, false); // bad data type
    // TODO test other types!

}

void PrimitiveDataInformationTest::testValueStringUIntAndBool()
{
    // this test will fail if translations are loaded
    QFETCH(quint64, value);
    QFETCH(QString, binStr);
    QFETCH(QString, hexStr);
    QFETCH(QString, decStr);
    QFETCH(QString, octStr);

    int minSize = minimumUnsignedBits(value);
    // qDebug() << "need" << minSize << "bit to represent" << value;
    QString boolBase;
    if (value == 0) {
        boolBase = QStringLiteral("false");
    } else if (value == 1) {
        boolBase = QStringLiteral("true");
    } else {
        boolBase = QStringLiteral("true (%1)");
    }
    // run once with locale aware, and once without
    for (int i = 0; i <= 1; ++i) {
        Kasten::StructureViewPreferences::setLocaleAwareDecimalFormatting(bool(i));

        if (minSize <= 8) {
            valueCompareHelperUnsigned<PrimitiveDataType::UInt8, PrimitiveDataType::Bool8>(quint8(value), binStr, hexStr,
                                                                                           decStr, octStr, boolBase);
        }
        if (minSize <= 16) {
            valueCompareHelperUnsigned<PrimitiveDataType::UInt16, PrimitiveDataType::Bool16>(quint16(value), binStr, hexStr,
                                                                                             decStr, octStr, boolBase);
        }
        if (minSize <= 32) {
            valueCompareHelperUnsigned<PrimitiveDataType::UInt32, PrimitiveDataType::Bool32>(quint32(value), binStr, hexStr,
                                                                                             decStr, octStr, boolBase);
        }
        if (minSize <= 64) {
            valueCompareHelperUnsigned<PrimitiveDataType::UInt64, PrimitiveDataType::Bool64>(quint64(value), binStr, hexStr,
                                                                                             decStr, octStr, boolBase);
        }

        // check bitfield now
        UnsignedBitfieldDataInformation bitfield(QStringLiteral("unsigned"), minSize);
        BoolBitfieldDataInformation boolBitfield(QStringLiteral("bool"), minSize);
        bitfield.setValue(value);
        boolBitfield.setValue(value);
        bitfield.mWasAbleToRead = true;
        boolBitfield.mWasAbleToRead = true;
        for (uint width = minSize; width <= 64u; ++width) {
            bitfield.setWidth(width);
            bitfield.setWidth(width);
            Kasten::StructureViewPreferences::setUnsignedDisplayBase(2);
            QCOMPARE(bitfield.valueString(), binStr);
            QCOMPARE(boolBitfield.valueString(), value > 1 ? boolBase.arg(binStr) : boolBase);
            Kasten::StructureViewPreferences::setUnsignedDisplayBase(16);
            QCOMPARE(bitfield.valueString(), hexStr);
            QCOMPARE(boolBitfield.valueString(), value > 1 ? boolBase.arg(hexStr) : boolBase);
            Kasten::StructureViewPreferences::setUnsignedDisplayBase(10);
            QCOMPARE(bitfield.valueString(), decStr);
            QCOMPARE(boolBitfield.valueString(), value > 1 ? boolBase.arg(decStr) : boolBase);
            Kasten::StructureViewPreferences::setUnsignedDisplayBase(8);
            QCOMPARE(bitfield.valueString(), octStr);
            QCOMPARE(boolBitfield.valueString(), value > 1 ? boolBase.arg(octStr) : boolBase);
        }
    }
}

void PrimitiveDataInformationTest::testValueStringUIntAndBool_data()
{
    QTest::addColumn<quint64>("value");
    QTest::addColumn<QString>("binStr");
    QTest::addColumn<QString>("hexStr");
    QTest::addColumn<QString>("decStr");
    QTest::addColumn<QString>("octStr");

    QTest::newRow("0") << quint64(0) << "0b0" << "0x0" << "0" << "0o0";
    QTest::newRow("1") << quint64(1) << "0b1" << "0x1" << "1" << "0o1";
    QTest::newRow("2") << quint64(2) << "0b10" << "0x2" << "2" << "0o2";
    QTest::newRow("32") << quint64(32) << "0b100000" << "0x20" << "32" << "0o40";
    QTest::newRow("79") << quint64(79) << "0b1001111" << "0x4f" << "79" << "0o117";
    QTest::newRow("172") << quint64(172) << "0b10101100" // 8 chars -> no space
                         << "0xac" << "172" << "0o254";
    QTest::newRow("259") << quint64(259) << "0b1 00000011" // 9 chars -> space
                         << "0x103" << "259" << "0o403";
    QTest::newRow("50448") << quint64(50448) << "0b11000101 00010000" // 16 chars -> no space
                           << "0xc510" << "50448" << "0o142420";
    QTest::newRow("126832") << quint64(126832) << "0b1 11101111 01110000" // 17 chars -> space
                            << "0x1ef70" << "126832" << "0o367560";
    // maximum
    QTest::newRow("quint8::max()") << quint64(std::numeric_limits<quint8>::max())
                                   << "0b11111111" << "0xff" << "255" << "0o377";
    QTest::newRow("quint16::max()") << quint64(std::numeric_limits<quint16>::max())
                                    << "0b11111111 11111111" << "0xffff" << "65535" << "0o177777";
    QTest::newRow("quint32::max()") << quint64(std::numeric_limits<quint32>::max())
                                    << "0b11111111 11111111 11111111 11111111" << "0xffffffff" << "4294967295"
                                    << "0o377 77777777";
    QTest::newRow("quint64::max()") << quint64(std::numeric_limits<quint64>::max())
                                    << "0b11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111"
                                    << "0xffffffff ffffffff" << "18446744073709551615" << "0o177777 77777777 77777777";
}

namespace {
QString charString(quint32 i)
{
    QString charString;
    if (i == '\n') {
        charString = QStringLiteral("\\n");
    } else if (i == '\t') {
        charString = QStringLiteral("\\t");
    } else if (i == '\r') {
        charString = QStringLiteral("\\r");
    } else if (i == '\f') {
        charString = QStringLiteral("\\f");
    } else if (i == '\0') {
        charString = QStringLiteral("\\0");
    } else if (i == '\v') {
        charString = QStringLiteral("\\v");
    } else if (i == '\b') {
        charString = QStringLiteral("\\b");
    } else if (i == '\a') {
        charString = QStringLiteral("\\a");
    } else {
        QChar unicode(i);
        if (unicode.isPrint()) {
            charString = QString(unicode);
        } else {
            charString = QString(QChar::ReplacementCharacter);
        }
    }
    // qDebug doesn't output unicode!
    // printf("The string for char %#x is: %s\n", i, charString.toUtf8().data());
    return charString;
}
}

void PrimitiveDataInformationTest::testValueStringChar()
{
    Kasten::StructureViewPreferences::setShowCharNumericalValue(false);
    Kasten::StructureViewPreferences::setLocaleAwareDecimalFormatting(false);
    // we don't want the numeric value now
    for (int i = 0; i < 256; ++i) {
        QString expected = QStringLiteral("'%1'").arg(charString(i));
        QCOMPARE(CharDataInformationMethods::staticValueString(i), expected);
    }

    Kasten::StructureViewPreferences::setShowCharNumericalValue(true);
    Kasten::StructureViewPreferences::setCharDisplayBase(16);
    for (int i = 0; i < 256; ++i) {
        QString expected = QStringLiteral("'%1' (0x%2)").arg(charString(i),
                                                             QString::number(i, 16));
        QCOMPARE(CharDataInformationMethods::staticValueString(i), expected);
    }

    Kasten::StructureViewPreferences::setCharDisplayBase(10);
    for (int i = 0; i < 256; ++i) {
        QString expected = QStringLiteral("'%1' (%2)").arg(charString(i),
                                                           QString::number(i, 10));
        QCOMPARE(CharDataInformationMethods::staticValueString(i), expected);
    }

    Kasten::StructureViewPreferences::setCharDisplayBase(2);
    for (int i = 0; i < 256; ++i) {
        QString expected = QStringLiteral("'%1' (0b%2)").arg(charString(i),
                                                             QString::number(i, 2));
        QCOMPARE(CharDataInformationMethods::staticValueString(i), expected);
    }

    Kasten::StructureViewPreferences::setCharDisplayBase(8);
    for (int i = 0; i < 256; ++i) {
        QString expected = QStringLiteral("'%1' (0o%2)").arg(charString(i),
                                                             QString::number(i, 8));
        QCOMPARE(CharDataInformationMethods::staticValueString(i), expected);
    }

    // TODO octal
}

void PrimitiveDataInformationTest::testValueStringDouble()
{
    // TODO implement
}

void PrimitiveDataInformationTest::testValueStringFloat()
{
    // TODO implement
}

void PrimitiveDataInformationTest::testToAndFromVariant()
{
    // TODO implement
}

void PrimitiveDataInformationTest::testGetAndSetValue()
{
    QFETCH(PrimitiveDataInformation*, data);
    QFETCH(AllPrimitiveTypes, newVal);
    QFETCH(AllPrimitiveTypes, expected);

    QScopedPointer<PrimitiveDataInformation> clone(data->clone());
    clone->setValue(newVal);
    QCOMPARE(clone->value(), expected);
}

template <typename signedType, typename unsignedType>
void PrimitiveDataInformationTest::addRowsGetAndSetSigned(PrimitiveDataType type, const char* name)
{
    QString msg = QString::fromUtf8(name);
    QTest::newRow(msg.arg(QStringLiteral("-325")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(-325) << AllPrimitiveTypes(signedType(-325));
    QTest::newRow(msg.arg(QStringLiteral("0")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(0) << AllPrimitiveTypes(signedType(0));
    QTest::newRow(msg.arg(QStringLiteral("-1")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(-1) << AllPrimitiveTypes(signedType(-1));
    QTest::newRow(msg.arg(QStringLiteral("357891")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(357891) << AllPrimitiveTypes(signedType(357891));

    QTest::newRow(msg.arg(QStringLiteral("max")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(std::numeric_limits<signedType>::max())
        << AllPrimitiveTypes(signedType(std::numeric_limits<signedType>::max()));
    QTest::newRow(msg.arg(QStringLiteral("min")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(std::numeric_limits<signedType>::min())
        << AllPrimitiveTypes(signedType(std::numeric_limits<signedType>::min()));
    QTest::newRow(msg.arg(QStringLiteral("u_max")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(std::numeric_limits<unsignedType>::max())
        << AllPrimitiveTypes(signedType(std::numeric_limits<unsignedType>::max()));
    QTest::newRow(msg.arg(QStringLiteral("u_min")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(std::numeric_limits<unsignedType>::min())
        << AllPrimitiveTypes(signedType(std::numeric_limits<unsignedType>::min()));
}

template <typename signedType>
void PrimitiveDataInformationTest::addRowsGetAndSetUnsigned(PrimitiveDataType type, const char* name)
{
    QString msg = QString::fromUtf8(name);
    QTest::newRow(msg.arg(QStringLiteral("-325")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(-325) << AllPrimitiveTypes(signedType(-325));
    QTest::newRow(msg.arg(QStringLiteral("0")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(0) << AllPrimitiveTypes(signedType(0));
    QTest::newRow(msg.arg(QStringLiteral("-1")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(-1) << AllPrimitiveTypes(signedType(-1));
    QTest::newRow(msg.arg(QStringLiteral("357891")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(357891) << AllPrimitiveTypes(signedType(357891));

    QTest::newRow(msg.arg(QStringLiteral("max")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(std::numeric_limits<signedType>::max())
        << AllPrimitiveTypes(signedType(std::numeric_limits<signedType>::max()));
    QTest::newRow(msg.arg(QStringLiteral("min")).toUtf8().constData())
        << basic[type] << AllPrimitiveTypes(std::numeric_limits<signedType>::min())
        << AllPrimitiveTypes(signedType(std::numeric_limits<signedType>::min()));
}

void PrimitiveDataInformationTest::testGetAndSetValue_data()
{
    QTest::addColumn<PrimitiveDataInformation*>("data");
    QTest::addColumn<AllPrimitiveTypes>("newVal");
    QTest::addColumn<AllPrimitiveTypes>("expected");

    addRowsGetAndSetSigned<qint8, quint8>(PrimitiveDataType::Int8, "int8: %1");
    addRowsGetAndSetSigned<qint16, quint16>(PrimitiveDataType::Int16, "int8: %1");
    addRowsGetAndSetSigned<qint32, quint32>(PrimitiveDataType::Int32, "int8: %1");
    addRowsGetAndSetSigned<qint64, quint64>(PrimitiveDataType::Int64, "int8: %1");
}

void PrimitiveDataInformationTest::cleanupTestCase()
{
    qDeleteAll(basic);
    delete signedBitfield;
    delete unsignedBitfield;
    delete boolBitfield;
}

QTEST_GUILESS_MAIN(PrimitiveDataInformationTest)

#include "primitivedatainformationtest.moc"
