/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include <QtTest>
#include <QScriptValue>
#include <QScriptEngine>
#include <limits>

#include "view/structures/parsers/parserutils.h"

Q_DECLARE_METATYPE(QScriptValue)

class CommonParserTest : public QObject
{
Q_OBJECT

private Q_SLOTS:
    void testIntFromString();
    void testIntFromString_data();
    void testUIntFromString();
    void testUIntFromString_data();

    void testIntFromScriptValue();
    void testIntFromScriptValue_data();
    void testUIntFromScriptValue();
    void testUIntFromScriptValue_data();

    void testToStringEncoding();
    void testToStringEncoding_data();
};

void CommonParserTest::testIntFromString_data()
{
    QTest::addColumn<QString>("str");
    QTest::addColumn<bool>("okay");
    QTest::addColumn<int>("value");

    QTest::newRow("-1") << "-1" << true << -1;
    QTest::newRow("hex with minus") << "-0xff" << true << -0xff;
    QTest::newRow("0xff") << "0xff" << true << 0xff;

    int maxInt = 0x7fffffff;
    QTEST_ASSERT(maxInt == std::numeric_limits<int>::max());
    QTest::newRow("max decimal") << "2147483647" << true << maxInt;
    QTest::newRow("max hex") << "0x7fffffff" << true << maxInt;
    QTest::newRow("max hex no leading 0x") << "7fffffff" << false;

    QTest::newRow("max+1 decimal") << "2147483648" << false;
    QTest::newRow("max+1 hex") << "0x80000000" << false;
    QTest::newRow("max-1 decimal") << "2147483646" << true << 0x7ffffffe;
    QTest::newRow("max-1 hex") << "7ffffffe" << false << 2147483646;

    int minInt = -0x80000000;
    QTEST_ASSERT(minInt == std::numeric_limits<int>::min());
    QTest::newRow("min decimal") << "-2147483648" << true << minInt;
    QTest::newRow("min hex") << "-0x80000000" << true << minInt;

    QTest::newRow("min+1 decimal") << "-2147483647" << true << -0x7fffffff;
    QTest::newRow("min+1 hex") << "-0x7fffffff" << true << -2147483647;
    QTest::newRow("min-1 decimal") << "-2147483649" << false;
    QTest::newRow("min-1 hex") << "-0x80000001" << false;

}

void CommonParserTest::testIntFromString()
{
    QFETCH(QString, str);
    QFETCH(bool, okay);

    ParsedNumber<int> result = ParserUtils::intFromString(str);
    QCOMPARE(result.string, str);
    QCOMPARE(result.isValid, okay);
    if (result.isValid)
    {
        QFETCH(int, value);
        QCOMPARE(result.value, value);
    }
}

void CommonParserTest::testUIntFromString_data()
{
    QTest::addColumn<QString>("str");
    QTest::addColumn<bool>("okay");
    QTest::addColumn<uint>("value");

    QTest::newRow("-1") << "-1" << false;
    QTest::newRow("0xff") << "0xff" << true << 0xffu;
    QTest::newRow("1234") << "1234" << true << 1234u;

    uint maxUint = 0xffffffff;
    QTEST_ASSERT(maxUint == std::numeric_limits<uint>::max());
    QTest::newRow("max decimal") << "4294967295" << true << maxUint;
    QTest::newRow("max hex") << "0xffffffff" << true << maxUint;
    QTest::newRow("max hex no leading 0x") << "ffffffff" << false;

    QTest::newRow("max+1 decimal") << "4294967296" << false;
    QTest::newRow("max+1 hex") << "0x100000000" << false;

}

void CommonParserTest::testUIntFromString()
{
    QFETCH(QString, str);
    QFETCH(bool, okay);

    ParsedNumber<uint> result = ParserUtils::uintFromString(str);
    QCOMPARE(result.string, str);
    QCOMPARE(result.isValid, okay);
    if (result.isValid)
    {
        QFETCH(uint, value);
        QCOMPARE(result.value, value);
    }
}

void CommonParserTest::testIntFromScriptValue_data()
{
    QTest::addColumn<QScriptValue>("scriptValue");
    QTest::addColumn<bool>("okay");
    QTest::addColumn<int>("value");

    QScriptEngine* engine = new QScriptEngine(this);
    QTest::newRow("invalid script value") << QScriptValue() << false;
    QTest::newRow("boolean true") << QScriptValue(true) << false;
    QTest::newRow("boolean false") << QScriptValue(false) << false;
    QTest::newRow("null script value") << engine->nullValue() << false;
    QTest::newRow("undefined script value") << engine->undefinedValue() << false;
    QTest::newRow("array") << engine->newArray(5) << false;
    QTest::newRow("object") << engine->newObject() << false;
    QTest::newRow("float") << QScriptValue(1234.5) << false;

    QTest::newRow("-1234 number") << QScriptValue(-1234) << true << -1234;
    QTest::newRow("-1234 string") << QScriptValue(QLatin1String("-1234")) << true << -1234;
    QTest::newRow("1234 number") << QScriptValue(1234) << true << 1234;
    QTest::newRow("1234 string") << QScriptValue(QLatin1String("1234")) << true << 1234;
    QTest::newRow("0xff number") << QScriptValue(0xff) << true << 0xff;
    QTest::newRow("0xff string") << QScriptValue(QLatin1String("0xff")) << true << 0xff;
    QTest::newRow("-0xff number") << QScriptValue(-0xff) << true << -0xff;
    QTest::newRow("-0xff string") << QScriptValue(QLatin1String("-0xff")) << true << -0xff;

    int maxInt = 0x7fffffff;
    QTEST_ASSERT(maxInt == std::numeric_limits<int>::max());
    QTest::newRow("max int") << QScriptValue(maxInt) << true << maxInt;
    QTest::newRow("max double") << QScriptValue(double(maxInt)) << true << maxInt;
    QTest::newRow("max string") << QScriptValue(QLatin1String("2147483647")) << true << maxInt;
    QTest::newRow("max hex string") << QScriptValue(QLatin1String("0x7fffffff")) << true << maxInt;
    //to int32 wraps around
    QTest::newRow("max+1") << QScriptValue(double(maxInt) + 1) << false;
    QTest::newRow("max+1 string") << QScriptValue(QLatin1String("2147483648")) << false;
    QTest::newRow("max+1 hex string") << QScriptValue(QLatin1String("0x80000000")) << false;
    QTest::newRow("max+2") << QScriptValue(double(maxInt) + 2) << false;

    int minInt = -0x80000000;
    QTEST_ASSERT(minInt == std::numeric_limits<int>::min());
    QTest::newRow("min int") << QScriptValue(minInt) << true << minInt;
    QTest::newRow("min double") << QScriptValue(double(minInt)) << true << minInt;
    QTest::newRow("min string") << QScriptValue(QLatin1String("-2147483648")) << true << minInt;
    QTest::newRow("min hex string") << QScriptValue(QLatin1String("-0x80000000")) << true << minInt;
    QTest::newRow("min-1") << QScriptValue(double(minInt) - 1) << false;
    QTest::newRow("large number") << QScriptValue(double(0x123456789a)) << false;

}

void CommonParserTest::testIntFromScriptValue()
{
    QFETCH(QScriptValue, scriptValue);
    QFETCH(bool, okay);

    ParsedNumber<int> result = ParserUtils::intFromScriptValue(scriptValue);
    QCOMPARE(result.string, scriptValue.toString());
    if (result.isValid != okay)
    {
        qDebug() << scriptValue.toString() << ", int =" << scriptValue.toInt32() << ", double = "
                << scriptValue.toNumber();
    }
    QCOMPARE(result.isValid, okay);
    if (result.isValid)
    {
        QFETCH(int, value);
        QCOMPARE(result.value, value);
    }
}

void CommonParserTest::testUIntFromScriptValue_data()
{
    QTest::addColumn<QScriptValue>("scriptValue");
    QTest::addColumn<bool>("okay");
    QTest::addColumn<uint>("value");

    QScriptEngine* engine = new QScriptEngine(this);
    QTest::newRow("invalid script value") << QScriptValue() << false;
    QTest::newRow("boolean true") << QScriptValue(true) << false;
    QTest::newRow("boolean false") << QScriptValue(false) << false;
    QTest::newRow("null script value") << engine->nullValue() << false;
    QTest::newRow("undefined script value") << engine->undefinedValue() << false;
    QTest::newRow("array") << engine->newArray(5) << false;
    QTest::newRow("object") << engine->newObject() << false;
    QTest::newRow("float") << QScriptValue(1234.5) << false;

    QTest::newRow("1234 number") << QScriptValue(1234) << true << 1234u;
    QTest::newRow("1234 string") << QScriptValue(QLatin1String("1234")) << true << 1234u;
    QTest::newRow("0xff number") << QScriptValue(0xff) << true << 0xffu;
    QTest::newRow("0xff string") << QScriptValue(QLatin1String("0xff")) << true << 0xffu;

    QTest::newRow("negative string") << QScriptValue(QLatin1String("-1")) << false;
    QTest::newRow("negative number") << QScriptValue(-1) << false;

    uint maxInt = 0xffffffff;
    QTEST_ASSERT(maxInt == std::numeric_limits<uint>::max());
    QTest::newRow("max") << QScriptValue(double(maxInt)) << true << maxInt;
    QTest::newRow("max int") << QScriptValue(maxInt) << true << maxInt;
    QTest::newRow("max string") << QScriptValue(QLatin1String("4294967295")) << true << maxInt;
    QTest::newRow("max hex string") << QScriptValue(QLatin1String("0xffffffff")) << true << maxInt;
    QTest::newRow("max+1") << QScriptValue(double(maxInt) + 1) << false;
    QTest::newRow("max+1 string") << QScriptValue(QLatin1String("4294967296")) << false;
    QTest::newRow("max+1 hex string") << QScriptValue(QLatin1String("0x100000000")) << false;
    QTest::newRow("large number") << QScriptValue(double(0x123456789a)) << false;
}

void CommonParserTest::testUIntFromScriptValue()
{
    QFETCH(QScriptValue, scriptValue);
    QFETCH(bool, okay);

    ParsedNumber<uint> result = ParserUtils::uintFromScriptValue(scriptValue);
    QCOMPARE(result.string, scriptValue.toString());
    if (result.isValid != okay)
    {
        qDebug() << scriptValue.toString() << ", uint =" << scriptValue.toUInt32() << ", double = "
                << scriptValue.toNumber();
    }
    QCOMPARE(result.isValid, okay);
    if (result.isValid)
    {
        QFETCH(uint, value);
        QCOMPARE(result.value, value);
    }
}

inline void CommonParserTest::testToStringEncoding()
{
    QFETCH(QString, str);
    QFETCH(int, expected);
    QScopedPointer<ScriptLogger> logger(new ScriptLogger());
    StringDataInformation::StringType type =
            ParserUtils::toStringEncoding(str, LoggerWithContext(logger.data(), QString()));
    QCOMPARE((int)type, expected);
}

inline void CommonParserTest::testToStringEncoding_data()
{
    QTest::addColumn<QString>("str");
    QTest::addColumn<int>("expected");

    QTest::newRow("ascii") << "ascii" << (int)StringDataInformation::ASCII;
    QTest::newRow("ascii upper") << "ASCII" << (int)StringDataInformation::ASCII;
    QTest::newRow("excess char") << "asciii" << (int)StringDataInformation::InvalidEncoding;

    QTest::newRow("latin1") << "latin1" << (int)StringDataInformation::Latin1;
    QTest::newRow("latin-1") << "latin-1" << (int)StringDataInformation::Latin1;
    QTest::newRow("latin") << "latin" << (int)StringDataInformation::InvalidEncoding;

    QTest::newRow("utf8") << "utf8" << (int)StringDataInformation::UTF8;
    QTest::newRow("utf-8") << "utf-8" << (int)StringDataInformation::UTF8;
    QTest::newRow("utf--8") << "utf--8" << (int)StringDataInformation::InvalidEncoding;

    QTest::newRow("utf16") << "utf16" << (int)StringDataInformation::UTF16_LE;
    QTest::newRow("utf-16") << "utf-16" << (int)StringDataInformation::UTF16_LE;
    QTest::newRow("utf-16le") << "utf-16le" << (int)StringDataInformation::UTF16_LE;
    QTest::newRow("utf-16-le") << "utf-16-le" << (int)StringDataInformation::UTF16_LE;
    QTest::newRow("utf-16--le") << "utf-16--le" << (int)StringDataInformation::InvalidEncoding;

    QTest::newRow("utf-16be") << "utf-16be" << (int)StringDataInformation::UTF16_BE;
    QTest::newRow("utf-16-be") << "utf-16-be" << (int)StringDataInformation::UTF16_BE;
    QTest::newRow("utf-16-abe") << "utf-16-abe" << (int)StringDataInformation::InvalidEncoding;

    QTest::newRow("utf32") << "utf32" << (int)StringDataInformation::UTF32_LE;
    QTest::newRow("utf-32") << "utf-32" << (int)StringDataInformation::UTF32_LE;
    QTest::newRow("utf-32le") << "utf-32le" << (int)StringDataInformation::UTF32_LE;
    QTest::newRow("utf-32-le") << "utf-32-le" << (int)StringDataInformation::UTF32_LE;
    QTest::newRow("utf-32--le") << "utf-32--le" << (int)StringDataInformation::InvalidEncoding;

    QTest::newRow("utf-32be") << "utf-32be" << (int)StringDataInformation::UTF32_BE;
    QTest::newRow("utf-32-be") << "utf-32-be" << (int)StringDataInformation::UTF32_BE;
    QTest::newRow("utf-32-abe") << "utf-32-abe" << (int)StringDataInformation::InvalidEncoding;
}

QTEST_MAIN(CommonParserTest)


#include "commonparsertest.moc"
