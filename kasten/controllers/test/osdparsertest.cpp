/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011  Alex Richardson <alex.richardson@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#undef QT_NO_CAST_FROM_ASCII //being lazy

#include <QtTest>
#include <QtCore/QString>
#include <QtCore/QDate>
#include "view/structures/parsers/osdparser.h"
#include "view/structures/datatypes/primitive/primitivedatainformation.h"
#include "view/structures/datatypes/topleveldatainformation.h"

class OsdParserTest : public QObject
{
    Q_OBJECT
public:
private Q_SLOTS:
    void testPrimitive();
    void testPrimitive_data();
};

void OsdParserTest::testPrimitive_data()
{
    QTest::addColumn<QString>("xml");
    QTest::addColumn<QString>("secondXml");
    QTest::addColumn<int>("expectedType");
    QString base ("<?xml version=\"1.0\" encoding=\"UTF-8\"?><data><primitive name=\"foo\" type=\"%1\" /></data>");
    QTest::newRow("uint8") << base.arg("uint8") << base.arg("UInt8") << (int)Type_UInt8;
    QTest::newRow("uint16") << base.arg("uint16") << base.arg("UInt16") << (int)Type_UInt16;
    QTest::newRow("uint32") << base.arg("uint32") << base.arg("UInt32") << (int)Type_UInt32;
    QTest::newRow("uint64") << base.arg("uint64") << base.arg("UInt64") << (int)Type_UInt64;
    QTest::newRow("int8") << base.arg("int8") << base.arg("Int8") << (int)Type_Int8;
    QTest::newRow("int16") << base.arg("int16") << base.arg("Int16") << (int)Type_Int16;
    QTest::newRow("int32") << base.arg("int32") << base.arg("Int32") << (int)Type_Int32;
    QTest::newRow("int64") << base.arg("int64") << base.arg("Int64") << (int)Type_Int64;
    QTest::newRow("bool8") << base.arg("bool8") << base.arg("Bool8") << (int)Type_Bool8;
    QTest::newRow("bool16") << base.arg("bool16") << base.arg("Bool16") << (int)Type_Bool16;
    QTest::newRow("bool32") << base.arg("bool32") << base.arg("Bool32") << (int)Type_Bool32;
    QTest::newRow("bool64") << base.arg("bool64") << base.arg("Bool64") << (int)Type_Bool64;
    QTest::newRow("char") << base.arg("char") << base.arg("Char") << (int)Type_Char;
    QTest::newRow("float") << base.arg("float") << base.arg("Float") << (int)Type_Float;
    QTest::newRow("double") << base.arg("double") << base.arg("Double") << (int)Type_Double;
}

void OsdParserTest::testPrimitive()
{
    QFETCH(QString, xml);
    QFETCH(QString, secondXml);
    QFETCH(int, expectedType);
    PrimitiveDataType type = (PrimitiveDataType)expectedType;

    OsdParser parser(xml);
    QList<const TopLevelDataInformation*> tds = parser.parseStructures();
    QCOMPARE(tds.size(), 1);
    const TopLevelDataInformation* td = tds.at(0);
    DataInformation* data = td->actualDataInformation();
    QCOMPARE(data->name(), QString("foo"));
    PrimitiveDataInformation* prim = dynamic_cast<PrimitiveDataInformation*>(data);
    QVERIFY(prim);
    QCOMPARE(prim->type(), type);

    //just to ensure comparison is case insensitive
    OsdParser parser2(secondXml);
    QList<const TopLevelDataInformation*> tds2 = parser2.parseStructures();
    QCOMPARE(tds2.size(), 1);
    const TopLevelDataInformation* td2 = tds2.at(0);
    DataInformation* data2 = td2->actualDataInformation();
    QCOMPARE(data2->name(), QString("foo"));
    PrimitiveDataInformation* prim2 = dynamic_cast<PrimitiveDataInformation*>(data2);
    QVERIFY(prim2);
    QCOMPARE(prim2->type(), type);
}


QTEST_MAIN(OsdParserTest)

#include "osdparsertest.moc"
