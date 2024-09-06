/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "char8stringvalidatortest.hpp"

// sut
#include <char8stringvalidator.hpp>
#include <char8stringparser.hpp>
// Okteta core
#include <Okteta/CharCodec>
// Qt
#include <QTest>


void Char8StringValidatorTest::testValidate_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("codecName");
    QTest::addColumn<QValidator::State>("expectedValidateResult");
    QTest::addColumn<QChar>("expectedParseResult");

    const QString iso8859_1 = QStringLiteral("ISO-8859-1");
    const QString usAscii = QStringLiteral("US-ASCII");
    const QChar dummyByte;

    QTest::newRow("emptystring")
        << QString() << iso8859_1 << QValidator::Intermediate << dummyByte;
    QTest::newRow("A-ISO-8859-1")
        << QStringLiteral("A") << iso8859_1 << QValidator::Acceptable << QChar::fromLatin1('A');
    QTest::newRow("z-ISO-8859-1")
        << QStringLiteral("z") << iso8859_1 << QValidator::Acceptable << QChar::fromLatin1('z');
    QTest::newRow("€-ISO-8859-1")
        << QStringLiteral("€") << iso8859_1 << QValidator::Invalid << dummyByte;
    QTest::newRow("AA-ISO-8859-1")
        << QStringLiteral("AA") << iso8859_1 << QValidator::Invalid << dummyByte;
    QTest::newRow("backslash-ISO-8859-1")
        << QStringLiteral("\\") << iso8859_1 << QValidator::Acceptable << QChar::fromLatin1('\\');
    QTest::newRow("newline-ISO-8859-1")
        << QStringLiteral("\\n") << iso8859_1 << QValidator::Acceptable << QChar::fromLatin1('\n');
    QTest::newRow("newline-n-ISO-8859-1")
        << QStringLiteral("\\nn") << iso8859_1 << QValidator::Invalid << dummyByte;
    QTest::newRow("escaped-A-ISO-8859-1")
        << QStringLiteral("\\A") << iso8859_1 << QValidator::Invalid << dummyByte;
    QTest::newRow("escaped-hex-x-US-ASCII")
        << QStringLiteral("\\x") << usAscii << QValidator::Intermediate << dummyByte;
    QTest::newRow("escaped-hex-x2-US-ASCII")
        << QStringLiteral("\\x2") << usAscii << QValidator::Acceptable << QChar::fromLatin1('\x02');
    QTest::newRow("escaped-hex-x02-US-ASCII")
        << QStringLiteral("\\x02") << usAscii << QValidator::Acceptable << QChar::fromLatin1('\x02');
    QTest::newRow("escaped-hex-x2g-US-ASCII")
        << QStringLiteral("\\x2g") << usAscii << QValidator::Invalid << dummyByte;
    QTest::newRow("escaped-hex-x20-US-ASCII")
        << QStringLiteral("\\x20") << usAscii << QValidator::Acceptable << QChar::fromLatin1(' ');
    QTest::newRow("escaped-hex-undefined-US-ASCII")
        << QStringLiteral("\\xFF") << usAscii << QValidator::Invalid << dummyByte;
    QTest::newRow("escaped-octal-0-US-ASCII")
        << QStringLiteral("\\0") << usAscii << QValidator::Acceptable << QChar::fromLatin1('\0');
    QTest::newRow("escaped-octal-40-US-ASCII")
        << QStringLiteral("\\40") << usAscii << QValidator::Acceptable << QChar::fromLatin1('\40');
    QTest::newRow("escaped-octal-040-US-ASCII")
        << QStringLiteral("\\040") << usAscii << QValidator::Acceptable << QChar::fromLatin1('\40');
    QTest::newRow("escaped-octal-undefined-US-ASCII")
        << QStringLiteral("\\377") << usAscii << QValidator::Invalid << dummyByte;
    QTest::newRow("escaped-octal-outofbounds-US-ASCII")
        << QStringLiteral("\\444") << usAscii << QValidator::Invalid << dummyByte;
}

void Char8StringValidatorTest::testValidate()
{
    QFETCH(const QString, string);
    QFETCH(QString, codecName);
    QFETCH(const QValidator::State, expectedValidateResult);
    QFETCH(QChar, expectedParseResult);

    auto codec = Okteta::CharCodec::createCodec(codecName);

    QVERIFY(codec != nullptr);
    QCOMPARE(codec->name(), codecName);

    Okteta::Char8StringParser parser(codec.get());
    Okteta::Char8StringValidator validator(&parser);

    int dummyCursorPos = 0;
    QString input(string);

    QValidator::State validateResult = validator.validate(input, dummyCursorPos);

    QCOMPARE(validateResult, expectedValidateResult);

    if (validateResult == QValidator::Acceptable) {
        QChar parseResult;
        parser.evaluate(&parseResult, string);

        QCOMPARE(parseResult, expectedParseResult);
    }
}

QTEST_GUILESS_MAIN(Char8StringValidatorTest)

#include "moc_char8stringvalidatortest.cpp"
