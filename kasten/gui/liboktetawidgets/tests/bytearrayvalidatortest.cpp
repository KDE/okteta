/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayvalidatortest.hpp"

// Qt
#include <QTest>
// Std
#include <array>

Q_DECLARE_METATYPE(Okteta::ByteArrayValidator::Coding)

namespace Okteta {

const char* toString(ByteArrayValidator::Coding coding)
{
    const char* text;
    switch (coding) {
    case ByteArrayValidator::HexadecimalCoding: text = "HexadecimalCoding"; break;
    case ByteArrayValidator::DecimalCoding:     text = "DecimalCoding"; break;
    case ByteArrayValidator::OctalCoding:       text = "OctalCoding"; break;
    case ByteArrayValidator::BinaryCoding:      text = "BinaryCoding"; break;
    case ByteArrayValidator::CharCoding:        text = "CharCoding"; break;
    case ByteArrayValidator::Utf8Coding:        text = "Utf8Coding"; break;
    default: text = nullptr; break;
    }
    return text;
}

inline QString hexString(int size)
{
    QString result;
    for (int value = 0; value < size; ++value) {
        result += QStringLiteral("%1").arg(value, 2, 16, QLatin1Char('0'));
    }
    return result;
}
inline QString hexBadString(int size)
{
    return QStringLiteral("G.H-I,J;K:L_M+N*O#PQRSTUVWXY").left(size);
}
inline QString decimalString(int size)
{
    QString result;
    for (int value = 0; value < size; ++value) {
        result += QStringLiteral("%1").arg(value, 3, 10, QLatin1Char('0'));
    }
    return result;
}
inline QString decimalBadString(int size)
{
    return QStringLiteral("ABCG.H-I,J;K:L_M+N*O#PQRSTUVWXY").left(size);
}
inline QString octalString(int size)
{
    QString result;
    for (int value = 0; value < size; ++value) {
        result += QStringLiteral("%1").arg(value, 3, 8, QLatin1Char('0'));
    }
    return result;
}
inline QString octalBadString(int size)
{
    return QStringLiteral("89ABCG.H-I,J;K:L_M+N*O#PQRSTUVWXY").left(size);
}
inline QString binaryString(int size)
{
    QString result;
    for (int value = 0; value < size; ++value) {
        result += QStringLiteral("%1").arg(value, 8, 2, QLatin1Char('0'));
    }
    return result;
}
inline QString binaryBadString(int size)
{
    return QStringLiteral("23456789ABCG.H-I,J;K:L_M+N*O#PQRSTUVWXY").left(size);
}
inline QString charString(int size)
{
    QString result;
    for (int value = 0; value < size; ++value) {
        switch (value) {
        case 7:   result += QLatin1String("\\a"); break;
        case 8:   result += QLatin1String("\\b"); break;
        case 9:   result += QLatin1String("\\t"); break;
        case 10:  result += QLatin1String("\\n"); break;
        case 11:  result += QLatin1String("\\v"); break;
        case 12:  result += QLatin1String("\\f"); break;
        case 13:  result += QLatin1String("\\r"); break;
        default:  result += QStringLiteral("\\x%1").arg(value, 2, 16, QLatin1Char('0'));
        }
    }
    return result;
}
inline QString charBadString(int size, const QString& charCodecName)
{
    if (charCodecName == QLatin1String("US-ASCII"))
        return QStringLiteral("¡¢£€¥Š§š©ª«¬.®¯°±¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ").left(size);
    // if (charCodecName == QLatin1String("ISO-8859-15"))
    // if (charCodecName == QLatin1String("EBCDIC 1047"))
    return QStringLiteral("░▒▓│┤╣║╗╝┐└┴┬├─┼╚╔╩╦╠═╬¤ı┘┌█▄¦").left(size);
}

QString ByteArrayValidatorTest::createString(ByteArrayValidator::Coding coding, int size)
{
    QString result;
    switch (coding) {
    case ByteArrayValidator::HexadecimalCoding: result = hexString(size); break;
    case ByteArrayValidator::DecimalCoding:     result = decimalString(size); break;
    case ByteArrayValidator::OctalCoding:       result = octalString(size); break;
    case ByteArrayValidator::BinaryCoding:      result = binaryString(size); break;
    case ByteArrayValidator::CharCoding:        result = charString(size); break;
    case ByteArrayValidator::Utf8Coding:        result = charString(size); break;
    default: break;
    }
    return result;
}

QString ByteArrayValidatorTest::createBadString(ByteArrayValidator::Coding coding, int size, const QString& charCodecName)
{
    QString result;
    switch (coding) {
    case ByteArrayValidator::HexadecimalCoding: result = hexBadString(size); break;
    case ByteArrayValidator::DecimalCoding:     result = decimalBadString(size); break;
    case ByteArrayValidator::OctalCoding:       result = octalBadString(size); break;
    case ByteArrayValidator::BinaryCoding:      result = binaryBadString(size); break;
    case ByteArrayValidator::CharCoding:        result = charBadString(size, charCodecName); break;
    default: break;
    }
    return result;
}

QByteArray ByteArrayValidatorTest::createFullByteSequence()
{
    static QByteArray sequence;
    if (sequence.isEmpty()) {
        sequence.resize(256);
        for (int i = 0; i < 256; ++i) {
            sequence[i] = static_cast<char>(i);
        }
    }
    return sequence;
}

void ByteArrayValidatorTest::testValidate_data()
{
    QTest::addColumn<Okteta::ByteArrayValidator::Coding>("coding");
    QTest::addColumn<QString>("charCodecName");
    QTest::addColumn<int>("minLength");
    QTest::addColumn<int>("maxLength");
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("expectedString");
    QTest::addColumn<QValidator::State>("expectedValidateResult");

    const std::array<ByteArrayValidator::Coding, 6> codings = {
        ByteArrayValidator::HexadecimalCoding,
        ByteArrayValidator::DecimalCoding,
        ByteArrayValidator::OctalCoding,
        ByteArrayValidator::BinaryCoding,
        ByteArrayValidator::CharCoding,
        ByteArrayValidator::Utf8Coding,
    };

    for (auto coding: codings) {
        const QStringList charCodecNames = (coding == ByteArrayValidator::CharCoding) ?
            QStringList { QStringLiteral("US-ASCII"), QStringLiteral("ISO-8859-15"), QStringLiteral("EBCDIC 1047") } :
            QStringList { QString() };
        for (const QString& charCodecName : charCodecNames) {
            QString codingString = QString::fromLatin1(toString(coding));
            if (!charCodecName.isEmpty()) {
                codingString += QLatin1Char('-') + QString(charCodecName).replace(QLatin1Char(' '), QLatin1Char('_'));
            }

            QTest::newRow(QStringLiteral("empty-%1-0-10").arg(codingString).toLatin1().constData())
                << coding << charCodecName << 0 << 10
                << QString() << QString() << QValidator::Acceptable;
            QTest::newRow(QStringLiteral("empty-%1-1-10").arg(codingString).toLatin1().constData())
                << coding << charCodecName << 1 << 10
                << QString() << QString() << QValidator::Intermediate;
            QTest::newRow(QStringLiteral("nullbyte-%1-1-10").arg(codingString).toLatin1().constData())
                << coding << charCodecName << 1 << 10
                << createString(coding, 1) << createString(coding, 1) << QValidator::Acceptable;
            QTest::newRow(QStringLiteral("10-bytes-%1-1-10").arg(codingString).toLatin1().constData())
                << coding << charCodecName << 1 << 10
                << createString(coding, 10) << createString(coding, 10) << QValidator::Acceptable;
            QTest::newRow(QStringLiteral("16-bytes-%1-1-10").arg(codingString).toLatin1().constData())
                << coding << charCodecName << 1 << 10
                << createString(coding, 16) << createString(coding, 16) << QValidator::Invalid;

            if (coding != ByteArrayValidator::Utf8Coding) {
                const QString invalidString = createBadString(coding, 1, charCodecName);
                QTest::newRow(QStringLiteral("invalid-bytes-%1-0-10").arg(codingString).toLatin1().constData())
                    << coding << charCodecName << 0 << 10
                    << invalidString << invalidString << QValidator::Invalid;
                const QString invalidAtBeginString = createBadString(coding, 1, charCodecName) + createString(coding, 1);
                QTest::newRow(QStringLiteral("invalidatbegin-bytes-%1-0-10").arg(codingString).toLatin1().constData())
                    << coding << charCodecName << 0 << 10
                    << invalidAtBeginString << invalidAtBeginString << QValidator::Invalid;
                const QString invalidAtEndString = createString(coding, 1) + createBadString(coding, 1, charCodecName);
                QTest::newRow(QStringLiteral("invalidatend-bytes-%1-0-10").arg(codingString).toLatin1().constData())
                    << coding << charCodecName << 0 << 10
                    << invalidAtEndString << invalidAtEndString << QValidator::Invalid;
            }
        }
    };
}

void ByteArrayValidatorTest::testValidate()
{
    QFETCH(const Okteta::ByteArrayValidator::Coding, coding);
    QFETCH(const QString, charCodecName);
    QFETCH(const int, minLength);
    QFETCH(const int, maxLength);
    QFETCH(const QString, string);
    QFETCH(const QString, expectedString);
    QFETCH(const QValidator::State, expectedValidateResult);

    Okteta::ByteArrayValidator validator;

    validator.setCodec(coding);
    if (coding == ByteArrayValidator::CharCoding) {
        validator.setCharCodec(charCodecName);
    }
    validator.setMinLength(minLength);
    validator.setMaxLength(maxLength);

    int dummyCursorPos = 0;
    QString input(string);

    QValidator::State validateResult = validator.validate(input, dummyCursorPos);

    QCOMPARE(validateResult, expectedValidateResult);
    QCOMPARE(input, expectedString);
}

void ByteArrayValidatorTest::testKeepMinMaxBetweenCodecs()
{
    const std::array<ByteArrayValidator::Coding, 6> codings = {
        ByteArrayValidator::DecimalCoding,
        ByteArrayValidator::OctalCoding,
        ByteArrayValidator::BinaryCoding,
        ByteArrayValidator::CharCoding,
        ByteArrayValidator::Utf8Coding,
        ByteArrayValidator::HexadecimalCoding, // back to initial at last
    };
    constexpr int minLength = 3;
    constexpr int maxLength = 8;

    Okteta::ByteArrayValidator validator;

    validator.setMinLength(minLength);
    validator.setMaxLength(maxLength);

    for (auto coding: codings) {
        validator.setCodec(coding);

        QCOMPARE(validator.minLength(), minLength);
        QCOMPARE(validator.maxLength(), maxLength);
    }
}

void ByteArrayValidatorTest::testToByteArraay_data()
{
    QTest::addColumn<Okteta::ByteArrayValidator::Coding>("coding");
    QTest::addColumn<int>("minLength");
    QTest::addColumn<int>("maxLength");
    QTest::addColumn<QString>("string");
    QTest::addColumn<QByteArray>("expectedByteArray");

    const std::array<ByteArrayValidator::Coding, 6> codings = {
        ByteArrayValidator::HexadecimalCoding,
        ByteArrayValidator::DecimalCoding,
        ByteArrayValidator::OctalCoding,
        ByteArrayValidator::BinaryCoding,
        ByteArrayValidator::CharCoding,
        ByteArrayValidator::Utf8Coding,
    };

    for (auto coding: codings) {
        const QString codingString = QString::fromLatin1(toString(coding));
        QTest::newRow(QStringLiteral("empty-%1-0-10").arg(codingString).toLatin1().constData())
            << coding << 0 << 10
            << QString() << QByteArray();
        QTest::newRow(QStringLiteral("empty-%1-1-10").arg(codingString).toLatin1().constData())
            << coding << 1 << 10
            << QString() << QByteArrayLiteral("\0");
        QTest::newRow(QStringLiteral("nullbyte-%1-1-10").arg(codingString).toLatin1().constData())
            << coding << 1 << 10
             << createString(coding, 1) << QByteArrayLiteral("\0");
        QTest::newRow(QStringLiteral("10-bytes-%1-1-10").arg(codingString).toLatin1().constData())
            << coding << 1 << 10
            << createString(coding, 10)
            << QByteArrayLiteral("\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9");
        QTest::newRow(QStringLiteral("16-bytes-%1-1-10").arg(codingString).toLatin1().constData())
            << coding << 1 << 10
            << createString(coding, 16)
            << QByteArrayLiteral("\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9");
    }
}

void ByteArrayValidatorTest::testToByteArraay()
{
    QFETCH(const Okteta::ByteArrayValidator::Coding, coding);
    QFETCH(const int, minLength);
    QFETCH(const int, maxLength);
    QFETCH(const QString, string);
    QFETCH(const QByteArray, expectedByteArray);

    Okteta::ByteArrayValidator validator;

    validator.setCodec(coding);
    validator.setMinLength(minLength);
    validator.setMaxLength(maxLength);

    const QByteArray byteArray = validator.toByteArray(string);

    QCOMPARE(byteArray, expectedByteArray);
}

void ByteArrayValidatorTest::testToString_data()
{
    QTest::addColumn<Okteta::ByteArrayValidator::Coding>("coding");
    QTest::addColumn<int>("minLength");
    QTest::addColumn<int>("maxLength");
    QTest::addColumn<QByteArray>("byteArray");
    QTest::addColumn<QString>("expectedString");

    const std::array<ByteArrayValidator::Coding, 6> codings = {
        ByteArrayValidator::HexadecimalCoding,
        ByteArrayValidator::DecimalCoding,
        ByteArrayValidator::OctalCoding,
        ByteArrayValidator::BinaryCoding,
        ByteArrayValidator::CharCoding,
        ByteArrayValidator::Utf8Coding,
    };

    for (auto coding: codings) {
        const QString codingString = QString::fromLatin1(toString(coding));
        QTest::newRow(QStringLiteral("empty-%1-0-10").arg(codingString).toLatin1().constData())
            << coding << 0 << 10
            << QByteArray() << QString();
        QTest::newRow(QStringLiteral("empty-%1-1-10").arg(codingString).toLatin1().constData())
            << coding << 1 << 10
            << QByteArray() << createString(coding, 1);
        QTest::newRow(QStringLiteral("nullbyte-%1-1-10").arg(codingString).toLatin1().constData())
            << coding << 1 << 10
            << QByteArrayLiteral("\0") << createString(coding, 1);
        QTest::newRow(QStringLiteral("10-bytes-%1-1-10").arg(codingString).toLatin1().constData())
            << coding << 1 << 10
            << QByteArrayLiteral("\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9")
            << createString(coding, 10);
        QTest::newRow(QStringLiteral("16-bytes-%1-1-10").arg(codingString).toLatin1().constData())
            << coding << 1 << 10
            << QByteArrayLiteral("\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC\xD\xE\xF")
            << createString(coding, 10);
    };
}

void ByteArrayValidatorTest::testToString()
{
    QFETCH(const Okteta::ByteArrayValidator::Coding, coding);
    QFETCH(const int, minLength);
    QFETCH(const int, maxLength);
    QFETCH(const QByteArray, byteArray);
    QFETCH(const QString, expectedString);

    Okteta::ByteArrayValidator validator;

    validator.setCodec(coding);
    validator.setMinLength(minLength);
    validator.setMaxLength(maxLength);

    const QString string = validator.toString(byteArray);

    QCOMPARE(string, expectedString);
}

void ByteArrayValidatorTest::testRoundtrip_data()
{
    QTest::addColumn<Okteta::ByteArrayValidator::Coding>("coding");

    const std::array<ByteArrayValidator::Coding, 6> codings = {
        ByteArrayValidator::HexadecimalCoding,
        ByteArrayValidator::DecimalCoding,
        ByteArrayValidator::OctalCoding,
        ByteArrayValidator::BinaryCoding,
        ByteArrayValidator::CharCoding,
        ByteArrayValidator::Utf8Coding,
    };

    for (auto coding: codings) {
        QTest::newRow(toString(coding)) << coding;
    }
}

void ByteArrayValidatorTest::testRoundtrip()
{
    QFETCH(const Okteta::ByteArrayValidator::Coding, coding);

    const QByteArray sequence = createFullByteSequence();

    Okteta::ByteArrayValidator validator;

    validator.setCodec(coding);

    const QString string = validator.toString(sequence);
    const QByteArray byteArray = validator.toByteArray(string);

    QCOMPARE(byteArray, sequence);
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayValidatorTest)

#include "moc_bytearrayvalidatortest.cpp"
