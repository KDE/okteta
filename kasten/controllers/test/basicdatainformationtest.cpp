/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    Copyright 2012  Alex Richardson <alex.richardson@gmx.de>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <QtTest>
#include <QtCore/QDate>

#include "view/structures/datatypes/array/arraydatainformation.h"
#include "view/structures/datatypes/array/primitivearraydata.h"
#include "view/structures/datatypes/topleveldatainformation.h"
#include "view/structures/datatypes/primitive/primitivetemplateinfo.h"
#include "view/structures/datatypes/primitive/enumdatainformation.h"
#include "view/structures/datatypes/primitive/flagdatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/boolbitfielddatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/signedbitfielddatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.h"
#include "view/structures/datatypes/primitivefactory.h"
#include "view/structures/datatypes/uniondatainformation.h"
#include "view/structures/datatypes/structuredatainformation.h"
#include "view/structures/datatypes/structuredatainformation.h"

struct ExpectedResults {
	ExpectedResults()
	: parent(0), size(0),isTopLevel(false), isEnum(false), isStruct(false), isUnion(false),
		isArray(false), isBitfield(false), isPrimitive(false), canHaveChildren(false), hasChildren(false)
		{
			columnFlags[DataInformation::ColumnName] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
			columnFlags[DataInformation::ColumnType] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
			columnFlags[DataInformation::ColumnValue] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
			noFileColumnFlags[DataInformation::ColumnName] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
			noFileColumnFlags[DataInformation::ColumnType] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
			noFileColumnFlags[DataInformation::ColumnValue] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		}
	DataInformationBase* parent;
	BitCount32 size;
	bool isTopLevel : 1;
	bool isEnum : 1;
	bool isStruct : 1;
	bool isUnion : 1;
	bool isArray : 1;
	bool isBitfield : 1;
	bool isPrimitive : 1;
	bool canHaveChildren : 1;
	bool hasChildren : 1;
	Qt::ItemFlags columnFlags[DataInformation::COLUMN_COUNT];
	Qt::ItemFlags noFileColumnFlags[DataInformation::COLUMN_COUNT];
};

class BasicDataInformationTest : public QObject
{
	Q_OBJECT
private:
	void basicTest(DataInformation* data, const ExpectedResults& expected) const;
private Q_SLOTS:
	void initTestCase();
	void testPrimitives();
	void testBitfields();
	void testStructs();
	void testUnions();
	void testArrays();
	void testEnums();
	void cleanupTestCase();
private:
	QVector<PrimitiveDataInformation*> primitives;
	QVector<AbstractBitfieldDataInformation*> bitfields;
	StructureDataInformation* emptyStruct;
	StructureDataInformation* structWithChildren;
	UnionDataInformation* emptyUnion;
	UnionDataInformation* unionWithChildren;
	ArrayDataInformation* emptyPrimitiveArray;
	ArrayDataInformation* emptyComplexArray;
	ArrayDataInformation* primitiveArrayWithChildren;
	ArrayDataInformation* complexArrayWithChildren;
	FlagDataInformation* flagData;
	EnumDataInformation* enumData;
};

void BasicDataInformationTest::basicTest(DataInformation* data, const ExpectedResults& expected) const
{
	QCOMPARE(data->isTopLevel(), expected.isTopLevel);
	QCOMPARE(data->isArray(), expected.isArray);
	QCOMPARE(data->isBitfield(), expected.isBitfield);
	QCOMPARE(data->isEnum(), expected.isEnum);
	QCOMPARE(data->isPrimitive(), expected.isPrimitive);
	QCOMPARE(data->isStruct(), expected.isStruct);
	QCOMPARE(data->isUnion(), expected.isUnion);
	QCOMPARE(data->size(), expected.size);
	QCOMPARE(data->parent(), expected.parent);
	DataInformation* clone1 = (data->clone());
	QScopedPointer<TopLevelDataInformation> top(new TopLevelDataInformation(clone1, QFileInfo(), 0, false));
	QCOMPARE(clone1->parent(), top.data()); //top takes ownership of clone1
	QCOMPARE(top->actualDataInformation(), clone1);

	QScopedPointer<DataInformation> clone2(clone1->clone());
	QVERIFY(clone2->parent() == 0); //cloning should reset parent to NULL, else we get dangling pointers

	QCOMPARE(data->flags(DataInformation::ColumnName, true), expected.columnFlags[DataInformation::ColumnName]);
	QCOMPARE(data->flags(DataInformation::ColumnType, true), expected.columnFlags[DataInformation::ColumnType]);
	QCOMPARE(data->flags(DataInformation::ColumnValue, true), expected.columnFlags[DataInformation::ColumnValue]);

	QCOMPARE(data->flags(DataInformation::ColumnName, false), expected.noFileColumnFlags[DataInformation::ColumnName]);
	QCOMPARE(data->flags(DataInformation::ColumnType, false), expected.noFileColumnFlags[DataInformation::ColumnType]);
	QCOMPARE(data->flags(DataInformation::ColumnValue, false), expected.noFileColumnFlags[DataInformation::ColumnValue]);
}


void BasicDataInformationTest::initTestCase()
{
//	qRegisterMetaType<const DataInformation*>();
//	qRegisterMetaType<DataInformation*>();
	for (int i = Type_START; i < Type_Bitfield; ++i) {
		primitives.append(PrimitiveFactory::newInstance(QLatin1String("prim"), static_cast<PrimitiveDataType>(i)));
	}
	bitfields.append(new BoolBitfieldDataInformation(QLatin1String("bitfield"), 24));
	bitfields.append(new UnsignedBitfieldDataInformation(QLatin1String("bitfield"), 24));
	bitfields.append(new SignedBitfieldDataInformation(QLatin1String("bitfield"), 24));

	emptyStruct = new StructureDataInformation(QLatin1String("emptyStruct"));
	structWithChildren = new StructureDataInformation(QLatin1String("structWithChildren"));
	structWithChildren->addDataTypeToStruct(PrimitiveFactory::newInstance(QLatin1String("prim"), Type_UInt32));
	structWithChildren->addDataTypeToStruct(PrimitiveFactory::newInstance(QLatin1String("prim2"), Type_UInt64));

	emptyUnion = new UnionDataInformation(QLatin1String("emptyUnion"));
	unionWithChildren = new UnionDataInformation(QLatin1String("unionWithChildren"));
	unionWithChildren->addDataTypeToUnion(PrimitiveFactory::newInstance(QLatin1String("prim"), Type_UInt32));
	unionWithChildren->addDataTypeToUnion(PrimitiveFactory::newInstance(QLatin1String("prim2"), Type_UInt64));

	emptyPrimitiveArray = new ArrayDataInformation(QLatin1String("emptyPrimitiveArray"), 0, PrimitiveFactory::newInstance(QLatin1String("prim"), Type_UInt32));
	emptyComplexArray = new ArrayDataInformation(QLatin1String("emptyComplexArray"), 0, structWithChildren->clone());
	primitiveArrayWithChildren = new ArrayDataInformation(QLatin1String("primitiveArrayWithChildren"), 2, PrimitiveFactory::newInstance(QLatin1String("prim"), Type_UInt32));
	complexArrayWithChildren = new ArrayDataInformation(QLatin1String("complexArrayWithChildren"), 2, structWithChildren->clone());

	QMap<AllPrimitiveTypes, QString> enumVals;
	enumVals[1] = QLatin1String("one");
	enumVals[2] = QLatin1String("two");
	enumVals[4] = QLatin1String("four");
	EnumDefinition::Ptr edef(new EnumDefinition(enumVals, QLatin1String("eDef"), Type_UInt32));
	flagData = new FlagDataInformation(QLatin1String("flagData"), PrimitiveFactory::newInstance(QLatin1String("prim"), Type_UInt32), edef);
	enumData = new EnumDataInformation(QLatin1String("enumData"), PrimitiveFactory::newInstance(QLatin1String("prim"), Type_UInt32), edef);
}

void BasicDataInformationTest::testBitfields()
{
	ExpectedResults exp;
	exp.isPrimitive = true;
	exp.isBitfield = true;
	exp.size = 24;
	exp.columnFlags[DataInformation::ColumnValue] = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	for (int i = 0; i < bitfields.size(); ++i) {
		basicTest(bitfields.at(i), exp);
	}
}

void BasicDataInformationTest::testPrimitives()
{
	ExpectedResults exp;
	exp.isPrimitive = true;
	exp.columnFlags[DataInformation::ColumnValue] = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	for (int i = 0; i < primitives.size(); ++i) {
		PrimitiveDataInformation* data = primitives.at(i);
		PrimitiveDataType t = data->type();
		QCOMPARE(t, static_cast<PrimitiveDataType>(i));
		if (t == Type_Bool8 || t ==  Type_Int8 || t == Type_UInt8 || t == Type_Char)
			exp.size = 8;
		else if (t == Type_Bool16 || t == Type_Int16 || t == Type_UInt16)
			exp.size = 16;
		else if (t == Type_Bool32 || t == Type_Int32 || t == Type_UInt32 || t == Type_Float)
			exp.size = 32;
		else if (t == Type_Bool64 || t == Type_Int64 || t == Type_UInt64 || t == Type_Double)
			exp.size = 64;
		else
			QVERIFY(false);
		basicTest(data, exp);
	}
}

void BasicDataInformationTest::testArrays()
{
	ExpectedResults exp;
	exp.isArray = true;
	exp.canHaveChildren = true;
	exp.size = 0;
	basicTest(emptyComplexArray, exp);
	basicTest(emptyPrimitiveArray, exp);
	exp.hasChildren = true;
	exp.size = 64;
	basicTest(primitiveArrayWithChildren, exp);
	exp.size = 96 * 2;
	basicTest(complexArrayWithChildren, exp);
}


void BasicDataInformationTest::testEnums()
{
	ExpectedResults exp;
	exp.isPrimitive = true;
	exp.isEnum = true;
	exp.size = 32;
	exp.columnFlags[DataInformation::ColumnValue] = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	basicTest(enumData, exp);
	basicTest(flagData, exp);
}

void BasicDataInformationTest::testStructs()
{
	ExpectedResults exp;
	exp.isStruct = true;
	exp.canHaveChildren = true;
	exp.size = 0;
	basicTest(emptyStruct, exp);
	exp.hasChildren = true;
	exp.size = 96;
	basicTest(structWithChildren, exp);
}

void BasicDataInformationTest::testUnions()
{
	ExpectedResults exp;
	exp.isUnion = true;
	exp.canHaveChildren = true;
	exp.size = 0;
	basicTest(emptyUnion, exp);
	exp.hasChildren = true;
	exp.size = 64;
	basicTest(unionWithChildren, exp);
}

void BasicDataInformationTest::cleanupTestCase()
{
	qDeleteAll(primitives);
	qDeleteAll(bitfields);
	delete emptyStruct;
	delete structWithChildren;
	delete emptyUnion;
	delete unionWithChildren;
	delete emptyPrimitiveArray;
	delete emptyComplexArray;
	delete primitiveArrayWithChildren;
	delete complexArrayWithChildren;
	delete flagData;
	delete enumData;
}

QTEST_MAIN(BasicDataInformationTest)


#include "basicdatainformationtest.moc"
