/*
 *  This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *  SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "view/structures/script/scriptengineinitializer.hpp"
#include "view/structures/allprimitivetypes.hpp"

#include <QTest>
#include <QString>
#include <QDebug>
#include <QScriptEngine>
#include "view/structures/parsers/scriptvalueconverter.hpp"
#include "view/structures/datatypes/datainformation.hpp"
#include "view/structures/datatypes/primitive/primitivedatainformation.hpp"
#include "view/structures/datatypes/primitive/enumdatainformation.hpp"
#include "view/structures/script/scriptlogger.hpp"
#include "view/structures/parsers/parserutils.hpp"

class ScriptValueConverterTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testPrimitives();
    void testPrimitives_data();
    void testParseEnum();
    void basicConverterTest();
    void testParseEnum_data();

private:
    DataInformation* convert(const QString& code);
    DataInformation* convert(const QScriptValue& value);
    QScriptValue evaluate(const char* code);
    void dumpLoggerOutput();
    QScopedPointer<QScriptEngine> engine;
    QScopedPointer<ScriptLogger> logger;
};

DataInformation* ScriptValueConverterTest::convert(const QString& code)
{
    QScriptValue value = engine->evaluate(code);
    return ScriptValueConverter::convert(value, QStringLiteral("value"), logger.data());
}

DataInformation* ScriptValueConverterTest::convert(const QScriptValue& value)
{
    return ScriptValueConverter::convert(value, QStringLiteral("value"), logger.data());
}

QScriptValue ScriptValueConverterTest::evaluate(const char* code)
{
    return engine->evaluate(QString::fromUtf8(code));
}

void ScriptValueConverterTest::initTestCase()
{
    engine.reset(ScriptEngineInitializer::newEngine());
    logger.reset(new ScriptLogger());
}

void ScriptValueConverterTest::basicConverterTest()
{
    logger->clear();
    // check that passing functions works
    QScriptValue sVal = evaluate("var foo = { value : uint8(),\n"
                                 " str : struct({first : uint8(), second : uint16()}),\n"
                                 " obj : array(uint32(), 10) \n}\n foo");
    QVector<DataInformation*> converted = ScriptValueConverter::convertValues(sVal, logger.data());
    QCOMPARE(converted.size(), 3);
    QVERIFY(converted[0]->isPrimitive());
    QCOMPARE(converted[0]->name(), QStringLiteral("value"));
    QVERIFY(converted[1]->isStruct());
    QCOMPARE(converted[1]->name(), QStringLiteral("str"));
    QCOMPARE(converted[1]->childCount(), 2u);
    QVERIFY(converted[2]->isArray());
    QCOMPARE(converted[2]->name(), QStringLiteral("obj"));
    QCOMPARE(converted[2]->childCount(), 10u);

    // test with an array now
    sVal = evaluate("var foo = [uint8(), uint16(), uint32()]; foo");
    qDeleteAll(converted);
    converted = ScriptValueConverter::convertValues(sVal, logger.data());
    QCOMPARE(converted.size(), 3);
    QVERIFY(converted[0]->isPrimitive());
    QVERIFY(converted[1]->isPrimitive());
    QVERIFY(converted[2]->isPrimitive());
    QVERIFY(converted[0]->asPrimitive()->type() == PrimitiveDataType::UInt8);
    QVERIFY(converted[1]->asPrimitive()->type() == PrimitiveDataType::UInt16);
    QVERIFY(converted[2]->asPrimitive()->type() == PrimitiveDataType::UInt32);
    QCOMPARE(converted[0]->name(), QStringLiteral("0"));
    QCOMPARE(converted[1]->name(), QStringLiteral("1"));
    QCOMPARE(converted[2]->name(), QStringLiteral("2"));

    // check number is not a valid object
    sVal = evaluate("1 + 2");
    QVERIFY(sVal.isNumber());
    QVERIFY2(!convert(sVal), " numbers should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 1);

    // should be exactly 1 error message
    sVal = evaluate("function foo() { return uint8(); }; foo");
    QVERIFY(sVal.isFunction());
    QVERIFY2(!convert(sVal), "functions should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 2);

    // should be exactly 2 error messages
    sVal = evaluate("var x = /.*/; x");
    QVERIFY(sVal.isRegExp());
    QVERIFY2(!convert(sVal), " regexp should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 3);

    sVal = evaluate("var obj = { x : 1 }; obj.x();");
    QVERIFY(sVal.isError());
    QVERIFY2(!convert(sVal), " error objects should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 4);

    sVal = evaluate("var x = [1, 2, 3]; x");
    QVERIFY(sVal.isArray());
    QVERIFY2(!convert(sVal), " array objects should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 5);

    sVal = evaluate("var x = new Date(); x");
    QVERIFY(sVal.isDate());
    QVERIFY2(!convert(sVal), " date objects should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 6);

    sVal = evaluate("var x = true; x");
    QVERIFY(sVal.isBool());
    QVERIFY2(!convert(sVal), " bool objects should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 7);

    sVal = evaluate("var x = null; x");
    QVERIFY(sVal.isNull());
    QVERIFY2(!convert(sVal), " null should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 8);

    sVal = evaluate("var x = undefined; x");
    QVERIFY(sVal.isUndefined());
    QVERIFY2(!convert(sVal), " undefined should not be valid!");
    QCOMPARE(logger->rowCount(QModelIndex()), 9);

    // object with invalid entry
    sVal = evaluate("var foo = { value : function() { return 1; },\n"
                    " str : struct({first : uint8(), second : uint16()}),\n"
                    " obj : array(uint32(), 10) \n}\n foo");
    qDeleteAll(converted);
    converted = ScriptValueConverter::convertValues(sVal, logger.data());
    QCOMPARE(converted.size(), 2);
    // first entry is invalid
    QCOMPARE(logger->rowCount(QModelIndex()), 11);
    // this should cause 2 error messages -> 11 now
    // qDebug() << logger->messages();
    qDeleteAll(converted);
}

void ScriptValueConverterTest::testPrimitives_data()
{
    QTest::addColumn<QString>("code");
    QTest::addColumn<QString>("code2");
    QTest::addColumn<QString>("typeString");
    QTest::addColumn<int>("expectedType");

    QTest::newRow("uint8") << "uint8()" << "new uint8()" << "UInt8" << (int) PrimitiveDataType::UInt8;
    QTest::newRow("uint16") << "uint16()" << "new uint16()" << "UInt16" << (int) PrimitiveDataType::UInt16;
    QTest::newRow("uint32") << "uint32()" << "new uint32()" << "UInt32" << (int) PrimitiveDataType::UInt32;
    QTest::newRow("uint64") << "uint64()" << "new uint64()" << "UInt64" << (int) PrimitiveDataType::UInt64;
    QTest::newRow("int8") << "int8()" << "new int8()" << "Int8" << (int) PrimitiveDataType::Int8;
    QTest::newRow("int16") << "int16()" << "new int16()" << "Int16" << (int) PrimitiveDataType::Int16;
    QTest::newRow("int32") << "int32()" << "new int32()" << "Int32" << (int) PrimitiveDataType::Int32;
    QTest::newRow("int64") << "int64()" << "new int64()" << "Int64" << (int) PrimitiveDataType::Int64;
    QTest::newRow("bool8") << "bool8()" << "new bool8()" << "Bool8" << (int) PrimitiveDataType::Bool8;
    QTest::newRow("bool16") << "bool16()" << "new bool16()" << "Bool16" << (int) PrimitiveDataType::Bool16;
    QTest::newRow("bool32") << "bool32()" << "new bool32()" << "Bool32" << (int) PrimitiveDataType::Bool32;
    QTest::newRow("bool64") << "bool64()" << "new bool64()" << "Bool64" << (int) PrimitiveDataType::Bool64;
    QTest::newRow("char") << "char()" << "new char()" << "Char" << (int) PrimitiveDataType::Char;
    QTest::newRow("float") << "float()" << "new float()" << "Float" << (int) PrimitiveDataType::Float;
    QTest::newRow("double") << "double()" << "new double()" << "Double" << (int) PrimitiveDataType::Double;
}

void ScriptValueConverterTest::testPrimitives()
{
    QFETCH(QString, code);
    QFETCH(QString, code2);
    QFETCH(QString, typeString);
    QFETCH(int, expectedType);
    logger->clear();
    auto type = static_cast<PrimitiveDataType>(expectedType);

    QScriptValue val1 = engine->evaluate(code);
    QScriptValue val2 = engine->evaluate(code2);
    QCOMPARE(val1.property(ParserStrings::PROPERTY_TYPE()).toString(), typeString);
    QCOMPARE(val2.property(ParserStrings::PROPERTY_TYPE()).toString(), typeString);
    QCOMPARE(val1.property(ParserStrings::PROPERTY_INTERNAL_TYPE()).toString(), ParserStrings::TYPE_PRIMITIVE());
    QCOMPARE(val2.property(ParserStrings::PROPERTY_INTERNAL_TYPE()).toString(), ParserStrings::TYPE_PRIMITIVE());

    if (type == PrimitiveDataType::Invalid) {
        return; // the cast will fail
    }
    QScopedPointer<DataInformation> data1(ScriptValueConverter::convert(val1, QStringLiteral("val1"),
                                                                        logger.data()));
    QScopedPointer<DataInformation> data2(ScriptValueConverter::convert(val2, QStringLiteral("val2"),
                                                                        logger.data()));
    QVERIFY(data1);
    QVERIFY(data2);
    PrimitiveDataInformation* p1 = data1->asPrimitive();
    PrimitiveDataInformation* p2 = data2->asPrimitive();
    QVERIFY(p1);
    QVERIFY(p2);
    QCOMPARE(p1->type(), type);
    QCOMPARE(p2->type(), type);
    if (type == PrimitiveDataType::Bitfield) {
        return; // the following tests don't work with bitfields
    }
    QScopedPointer<DataInformation> data3(convert(QStringLiteral("\"%1\"").arg(typeString)));
    QVERIFY(data3);
    PrimitiveDataInformation* p3 = data3->asPrimitive();
    QVERIFY(p3);
    QCOMPARE(p3->type(), type);
}

void ScriptValueConverterTest::testParseEnum()
{
    // QFETCH(QString, name);
    QFETCH(QString, code);
    QFETCH(int, expectedCount);

    QScriptValue val = engine->evaluate(code);

    QVERIFY(val.isValid());
    QVERIFY(!val.isNull());
    QVERIFY(!val.isUndefined());
    QVERIFY(val.isObject());
    QCOMPARE(val.property(ParserStrings::PROPERTY_INTERNAL_TYPE()).toString(), QStringLiteral("enum"));

    QScopedPointer<DataInformation> data(ScriptValueConverter::convert(val, QStringLiteral("val"), logger.data()));
    if (expectedCount > 0) {
        QVERIFY(data);
    } else {
        QVERIFY(!data);
        return;
    }
    EnumDataInformation* e = data->asEnum();
    QVERIFY(e);

    QMap<AllPrimitiveTypes, QString> enumVals = e->enumValues()->values();
    QCOMPARE(enumVals.size(), expectedCount);

    if (expectedCount != 0) {
        QFETCH(quint64, expectedValue);
        // to ensure it does not match when value is not found add 1 to the default
        AllPrimitiveTypes result = enumVals.key(QStringLiteral("value"), expectedValue + 1);
        QCOMPARE(result.value<quint64>(), expectedValue);
    }
}

namespace {
inline QString arg2(const QString& str, const char* arg_1, const char* arg_2)
{
    return str.arg(QString::fromUtf8(arg_1), QString::fromUtf8(arg_2));
}
}

void ScriptValueConverterTest::testParseEnum_data()
{
    QString baseStr = QStringLiteral("enumeration(\"someValues\", %1, { value : %2})");
    QTest::addColumn<QString>("code");
    QTest::addColumn<int>("expectedCount");
    QTest::addColumn<quint64>("expectedValue");

    QTest::newRow("invalid_type_struct") << arg2(baseStr, "struct({ val : uint8() })", "1234.234")
                                         << 0 << quint64(0);
    QTest::newRow("invalid_type_array") << arg2(baseStr, "array(uint8(), 1)", "1234.234") << 0
                                        << quint64(0);
    QTest::newRow("invalid_type_union") << arg2(baseStr, "union({ val : uint8() })", "1234.234")
                                        << 0 << quint64(0);
    QTest::newRow("invalid_type_double") << arg2(baseStr, "double()", "1234.234") << 0 << quint64(0);
    QTest::newRow("invalid_type_float") << arg2(baseStr, "float()", "1234.234") << 0 << quint64(0);
    QTest::newRow("invalid_type_string") << arg2(baseStr, "string()", "1234.234") << 0 << quint64(0);

    QTest::newRow("float2int8") << arg2(baseStr, "uint8()", "1.234") << 1 << quint64(1);
    QTest::newRow("float2int8_range") << arg2(baseStr, "uint8()", "1234.234") << 0 << quint64(0);
    QTest::newRow("float2int32") << arg2(baseStr, "uint32()", "1234.1234") << 1 << quint64(1234);
    QTest::newRow("float2int32_range") << arg2(baseStr, "uint32()", "5294967296.234") << 0 << quint64(0);
    QTest::newRow("float2int64") << arg2(baseStr, "uint64()", "5294967296.234") << 1
                                 << quint64(5294967296UL);
    QTest::newRow("double_overflow") << arg2(baseStr, "uint64()", "9007199254740993.0") << 0
                                     << quint64(9007199254740993UL); // only 992 and 994 can be represented as a double
    QTest::newRow("uint64_max_hex") << arg2(baseStr, "uint64()", "new String(\"0xFFFFFFFFFFFFFFFF\")") << 1
                                    << quint64(0xFFFFFFFFFFFFFFFFL);
    QTest::newRow("uint64_max") << arg2(baseStr, "uint64()", "new String(\"18446744073709551615\")") << 1
                                << quint64(18446744073709551615UL);
}

QTEST_GUILESS_MAIN(ScriptValueConverterTest)

#include "scriptvalueconvertertest.moc"
