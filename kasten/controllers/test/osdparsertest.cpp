/*
 *  This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *  Copyright 2011, 2012  Alex Richardson <alex.richardson@gmx.de>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <QtTest>
#include <QString>
#include <QScriptEngine>
#include <QScriptValue>
#include "view/structures/parsers/osdparser.h"
#include "view/structures/datatypes/primitive/primitivedatainformation.h"
#include "view/structures/datatypes/topleveldatainformation.h"

class OsdParserTest : public QObject
{
Q_OBJECT
    private Q_SLOTS:
    void testPrimitive();
    void testPrimitive_data();
    void testScriptFuntion();
};

namespace
{
inline QString arg(const QString& str, const char* argument)
{
    return str.arg(QLatin1String(argument));
}
}
void OsdParserTest::testPrimitive_data()
{
    QTest::addColumn<QString>("xml");
    QTest::addColumn<QString>("secondXml");
    QTest::addColumn<int>("expectedType");
    QString base(
            QLatin1String(
                    "<?xml version=\"1.0\" encoding=\"UTF-8\"?><data><primitive name=\"foo\" type=\"%1\" /></data>"));
    QTest::newRow("uint8") << arg(base, "uint8") << arg(base, "UInt8") << (int) Type_UInt8;
    QTest::newRow("uint16") << arg(base, "uint16") << arg(base, "UInt16") << (int) Type_UInt16;
    QTest::newRow("uint32") << arg(base, "uint32") << arg(base, "UInt32") << (int) Type_UInt32;
    QTest::newRow("uint64") << arg(base, "uint64") << arg(base, "UInt64") << (int) Type_UInt64;
    QTest::newRow("int8") << arg(base, "int8") << arg(base, "Int8") << (int) Type_Int8;
    QTest::newRow("int16") << arg(base, "int16") << arg(base, "Int16") << (int) Type_Int16;
    QTest::newRow("int32") << arg(base, "int32") << arg(base, "Int32") << (int) Type_Int32;
    QTest::newRow("int64") << arg(base, "int64") << arg(base, "Int64") << (int) Type_Int64;
    QTest::newRow("bool8") << arg(base, "bool8") << arg(base, "Bool8") << (int) Type_Bool8;
    QTest::newRow("bool16") << arg(base, "bool16") << arg(base, "Bool16") << (int) Type_Bool16;
    QTest::newRow("bool32") << arg(base, "bool32") << arg(base, "Bool32") << (int) Type_Bool32;
    QTest::newRow("bool64") << arg(base, "bool64") << arg(base, "Bool64") << (int) Type_Bool64;
    QTest::newRow("char") << arg(base, "char") << arg(base, "Char") << (int) Type_Char;
    QTest::newRow("float") << arg(base, "float") << arg(base, "Float") << (int) Type_Float;
    QTest::newRow("double") << arg(base, "double") << arg(base, "Double") << (int) Type_Double;
}

void OsdParserTest::testPrimitive()
{
    QFETCH(QString, xml);
    QFETCH(QString, secondXml);
    QFETCH(int, expectedType);
    PrimitiveDataType type(static_cast<PrimitiveDataTypeEnum>(expectedType));

    OsdParser parser(xml);
    QVector<TopLevelDataInformation*> tds = parser.parseStructures();
    QCOMPARE(tds.size(), 1);
    const TopLevelDataInformation* td = tds.at(0);
    DataInformation* data = td->actualDataInformation();
    QCOMPARE(data->name(), QLatin1String("foo"));
    PrimitiveDataInformation* prim = data->asPrimitive();
    QVERIFY(prim);
    QCOMPARE(prim->type(), type);
    qDeleteAll(tds);

    //just to ensure comparison is case insensitive
    OsdParser parser2(secondXml);
    QVector<TopLevelDataInformation*> tds2 = parser2.parseStructures();
    QCOMPARE(tds2.size(), 1);
    const TopLevelDataInformation* td2 = tds2.at(0);
    DataInformation* data2 = td2->actualDataInformation();
    QCOMPARE(data2->name(), QString(QLatin1String("foo")));
    PrimitiveDataInformation* prim2 = data2->asPrimitive();
    QVERIFY(prim2);
    QCOMPARE(prim2->type(), type);
    qDeleteAll(tds2);
}

void OsdParserTest::testScriptFuntion()
{
    QScriptEngine engine;
    QScriptValue functionWrong1 = engine.evaluate(QLatin1String("function x() { return 2; }"));
    QVERIFY(functionWrong1.isUndefined());
    QScriptValue functionWrong2 = engine.evaluate(QLatin1String("function() { return 2; }"));
    QVERIFY(functionWrong2.isError());
    QScriptValue function = engine.evaluate(QLatin1String("x = function() { return 2; }"));
    QVERIFY(function.isFunction());
    QCOMPARE(function.toString(), QString(QLatin1String("function () { return 2; }")));
    //must wrap in parentheses, see https://bugreports.qt-project.org/browse/QTBUG-5757
    QScriptValue betterFunction = engine.evaluate(QLatin1String("(function() { return 2; })"));
    QVERIFY(betterFunction.isFunction());
    QCOMPARE(betterFunction.toString(), QString(QLatin1String("function () { return 2; }")));
}

QTEST_MAIN(OsdParserTest)

#include "osdparsertest.moc"
