/*
 *  This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *  SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QTest>
#include <QString>
#include <QScriptEngine>
#include <QScriptValue>
#include "view/structures/parsers/osdparser.hpp"
#include "view/structures/datatypes/primitive/primitivedatainformation.hpp"
#include "view/structures/datatypes/topleveldatainformation.hpp"

class OsdParserTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testPrimitive();
    void testPrimitive_data();
    void testScriptFuntion();
};

namespace {
inline QString arg(const QString& str, const char* argument)
{
    return str.arg(QString::fromUtf8(argument));
}
}
void OsdParserTest::testPrimitive_data()
{
    QTest::addColumn<QString>("xml");
    QTest::addColumn<QString>("secondXml");
    QTest::addColumn<int>("expectedType");
    QString base(
        QStringLiteral(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?><data><primitive name=\"foo\" type=\"%1\" /></data>"));
    QTest::newRow("uint8") << arg(base, "uint8") << arg(base, "UInt8") << (int) PrimitiveDataType::UInt8;
    QTest::newRow("uint16") << arg(base, "uint16") << arg(base, "UInt16") << (int) PrimitiveDataType::UInt16;
    QTest::newRow("uint32") << arg(base, "uint32") << arg(base, "UInt32") << (int) PrimitiveDataType::UInt32;
    QTest::newRow("uint64") << arg(base, "uint64") << arg(base, "UInt64") << (int) PrimitiveDataType::UInt64;
    QTest::newRow("int8") << arg(base, "int8") << arg(base, "Int8") << (int) PrimitiveDataType::Int8;
    QTest::newRow("int16") << arg(base, "int16") << arg(base, "Int16") << (int) PrimitiveDataType::Int16;
    QTest::newRow("int32") << arg(base, "int32") << arg(base, "Int32") << (int) PrimitiveDataType::Int32;
    QTest::newRow("int64") << arg(base, "int64") << arg(base, "Int64") << (int) PrimitiveDataType::Int64;
    QTest::newRow("bool8") << arg(base, "bool8") << arg(base, "Bool8") << (int) PrimitiveDataType::Bool8;
    QTest::newRow("bool16") << arg(base, "bool16") << arg(base, "Bool16") << (int) PrimitiveDataType::Bool16;
    QTest::newRow("bool32") << arg(base, "bool32") << arg(base, "Bool32") << (int) PrimitiveDataType::Bool32;
    QTest::newRow("bool64") << arg(base, "bool64") << arg(base, "Bool64") << (int) PrimitiveDataType::Bool64;
    QTest::newRow("char") << arg(base, "char") << arg(base, "Char") << (int) PrimitiveDataType::Char;
    QTest::newRow("float") << arg(base, "float") << arg(base, "Float") << (int) PrimitiveDataType::Float;
    QTest::newRow("double") << arg(base, "double") << arg(base, "Double") << (int) PrimitiveDataType::Double;
}

void OsdParserTest::testPrimitive()
{
    QFETCH(QString, xml);
    QFETCH(QString, secondXml);
    QFETCH(int, expectedType);
    PrimitiveDataType type(static_cast<PrimitiveDataType>(expectedType));

    OsdParser parser(xml);
    QVector<TopLevelDataInformation*> tds = parser.parseStructures();
    QCOMPARE(tds.size(), 1);
    const TopLevelDataInformation* td = tds.at(0);
    DataInformation* data = td->actualDataInformation();
    QCOMPARE(data->name(), QStringLiteral("foo"));
    PrimitiveDataInformation* prim = data->asPrimitive();
    QVERIFY(prim);
    QCOMPARE(prim->type(), type);
    qDeleteAll(tds);

    // just to ensure comparison is case insensitive
    OsdParser parser2(secondXml);
    QVector<TopLevelDataInformation*> tds2 = parser2.parseStructures();
    QCOMPARE(tds2.size(), 1);
    const TopLevelDataInformation* td2 = tds2.at(0);
    DataInformation* data2 = td2->actualDataInformation();
    QCOMPARE(data2->name(), QStringLiteral("foo"));
    PrimitiveDataInformation* prim2 = data2->asPrimitive();
    QVERIFY(prim2);
    QCOMPARE(prim2->type(), type);
    qDeleteAll(tds2);
}

void OsdParserTest::testScriptFuntion()
{
    QScriptEngine engine;
    QScriptValue functionWrong1 = engine.evaluate(QStringLiteral("function x() { return 2; }"));
    QVERIFY(functionWrong1.isUndefined());
    QScriptValue functionWrong2 = engine.evaluate(QStringLiteral("function() { return 2; }"));
    QVERIFY(functionWrong2.isError());
    QScriptValue function = engine.evaluate(QStringLiteral("x = function() { return 2; }"));
    QVERIFY(function.isFunction());
    QCOMPARE(function.toString(), QStringLiteral("function () { return 2; }"));
    // must wrap in parentheses, see https://bugreports.qt-project.org/browse/QTBUG-5757
    QScriptValue betterFunction = engine.evaluate(QStringLiteral("(function() { return 2; })"));
    QVERIFY(betterFunction.isFunction());
    QCOMPARE(betterFunction.toString(), QStringLiteral("function () { return 2; }"));
}

QTEST_GUILESS_MAIN(OsdParserTest)

#include "osdparsertest.moc"
