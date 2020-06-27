/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *    SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QTest>

#include "view/structures/datatypes/array/arraydatainformation.hpp"
#include "view/structures/datatypes/array/primitivearraydata.hpp"
#include "view/structures/datatypes/strings/stringdatainformation.hpp"
#include "view/structures/datatypes/topleveldatainformation.hpp"
#include "view/structures/datatypes/primitive/primitivetemplateinfo.hpp"
#include "view/structures/datatypes/primitive/enumdatainformation.hpp"
#include "view/structures/datatypes/primitive/flagdatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/boolbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/signedbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitivefactory.hpp"
#include "view/structures/datatypes/uniondatainformation.hpp"
#include "view/structures/datatypes/structuredatainformation.hpp"

struct ExpectedResults
{
    ExpectedResults()
        : isTopLevel(false)
        , isEnum(false)
        , isStruct(false)
        , isUnion(false)
        , isArray(false)
        , isBitfield(false)
        , isPrimitive(false)
        , isString(false)
        , isDummy(false)
        , canHaveChildren(false)
        , hasChildren(false)
    {
        columnFlags[DataInformation::ColumnName] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        columnFlags[DataInformation::ColumnType] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        columnFlags[DataInformation::ColumnValue] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        noFileColumnFlags[DataInformation::ColumnName] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        noFileColumnFlags[DataInformation::ColumnType] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        noFileColumnFlags[DataInformation::ColumnValue] = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    DataInformationBase* parent = nullptr;
    BitCount32 size = 0;
    bool isTopLevel : 1;
    bool isEnum : 1;
    bool isStruct : 1;
    bool isUnion : 1;
    bool isArray : 1;
    bool isBitfield : 1;
    bool isPrimitive : 1;
    bool isString : 1;
    bool isDummy : 1;
    bool canHaveChildren : 1;
    bool hasChildren : 1;
    Qt::ItemFlags columnFlags[DataInformation::COLUMN_COUNT];
    Qt::ItemFlags noFileColumnFlags[DataInformation::COLUMN_COUNT];
};

class BasicDataInformationTest : public QObject
{
    Q_OBJECT

private:
    void basicTest(DataInformationBase* data, const ExpectedResults& expected) const;

private Q_SLOTS:
    void initTestCase();
    void testPrimitives();
    void testBitfields();
    void testStructs();
    void testUnions();
    void testArrays();
    void testEnums();
    void testString();
    void testDummy();
    void testTopLevel();
    void cleanupTestCase();

private:
    QVector<PrimitiveDataInformation*> primitives;
    QVector<AbstractBitfieldDataInformation*> bitfields;
    TopLevelDataInformation* topLevel;
    StructureDataInformation* emptyStruct;
    StructureDataInformation* structWithChildren;
    UnionDataInformation* emptyUnion;
    UnionDataInformation* unionWithChildren;
    ArrayDataInformation* emptyPrimitiveArray;
    ArrayDataInformation* emptyComplexArray;
    ArrayDataInformation* primitiveArrayWithChildren;
    ArrayDataInformation* complexArrayWithChildren;
    StringDataInformation* emptyString;
    DummyDataInformation* dummy;
    FlagDataInformation* flagData;
    EnumDataInformation* enumData;
};

#if 0

#ifdef NDEBUG
#pragma message("NDEBUG defined")
#else
#pragma message("NDEBUG not defined")
#endif

#ifdef DEBUG
#pragma message("DEBUG defined")
#else
#pragma message("DEBUG not defined")
#endif

#ifdef QT_NO_DEBUG
#pragma message("QT_NO_DEBUG defined")
#else
#pragma message("QT_NO_DEBUG not defined")
#endif

#ifdef QT_DEBUG
#pragma message("QT_DEBUG defined")
#else
#pragma message("QT_DEBUG not defined")
#endif

#endif

namespace {
template <typename T> void castChecker(bool isValid, DataInformationBase* data, const T* constValue, T* nonConstValue)
{
    if (isValid) {
        QCOMPARE(constValue, static_cast<const T*>(data));
        QCOMPARE(nonConstValue, static_cast<T*>(data));
    } else {
        QCOMPARE(constValue, static_cast<const T*>(nullptr));
        QCOMPARE(nonConstValue, static_cast<T*>(nullptr));
    }
}
#define CAST_CHECKER(isValid, value, func, type) do { \
        const DataInformationBase* constData = data; \
        const type* constVal = constData->func(); \
        type* nonConstVal = data->func(); \
        castChecker(isValid, value, constVal, nonConstVal); \
    } while (0)

}
void BasicDataInformationTest::basicTest(DataInformationBase* data, const ExpectedResults& expected) const
{
    QVERIFY(data);
    QCOMPARE(data->isTopLevel(), expected.isTopLevel);
    if (expected.isTopLevel) {
        CAST_CHECKER(true, data, asTopLevel, TopLevelDataInformation);
        CAST_CHECKER(false, data, asArray, ArrayDataInformation);
        CAST_CHECKER(false, data, asBitfield, AbstractBitfieldDataInformation);
        CAST_CHECKER(false, data, asDataInformation, DataInformation);
        CAST_CHECKER(false, data, asDummy, DummyDataInformation);
        CAST_CHECKER(false, data, asEnum, EnumDataInformation);
        CAST_CHECKER(false, data, asPrimitive, PrimitiveDataInformation);
        CAST_CHECKER(false, data, asString, StringDataInformation);
        CAST_CHECKER(false, data, asStruct, StructureDataInformation);
        CAST_CHECKER(false, data, asUnion, UnionDataInformation);
        return; // no more can be done with a TopLeveDataInformation
    }

    CAST_CHECKER(false, data, asTopLevel, TopLevelDataInformation);
    CAST_CHECKER(true, data, asDataInformation, DataInformation);

    QCOMPARE(data->isArray(), expected.isArray);
    if (expected.isArray) {
        CAST_CHECKER(true, data, asArray, ArrayDataInformation);
    } else {
        CAST_CHECKER(false, data, asArray, ArrayDataInformation);
    }

    QCOMPARE(data->isBitfield(), expected.isBitfield);
    if (expected.isBitfield) {
        CAST_CHECKER(true, data, asBitfield, AbstractBitfieldDataInformation);
    } else {
        CAST_CHECKER(false, data, asBitfield, AbstractBitfieldDataInformation);
    }

    QCOMPARE(data->isEnum(), expected.isEnum);
    if (expected.isEnum) {
        CAST_CHECKER(true, data, asEnum, EnumDataInformation);
    } else {
        CAST_CHECKER(false, data, asEnum, EnumDataInformation);
    }

    QCOMPARE(data->isPrimitive(), expected.isPrimitive);
    if (expected.isPrimitive) {
        CAST_CHECKER(true, data, asPrimitive, PrimitiveDataInformation);
    } else {
        CAST_CHECKER(false, data, asPrimitive, PrimitiveDataInformation);
    }

    QCOMPARE(data->isStruct(), expected.isStruct);
    if (expected.isStruct) {
        CAST_CHECKER(true, data, asStruct, StructureDataInformation);
    } else {
        CAST_CHECKER(false, data, asStruct, StructureDataInformation);
    }

    QCOMPARE(data->isUnion(), expected.isUnion);
    if (expected.isUnion) {
        CAST_CHECKER(true, data, asUnion, UnionDataInformation);
    } else {
        CAST_CHECKER(false, data, asUnion, UnionDataInformation);
    }

    QCOMPARE(data->isString(), expected.isString);
    if (expected.isString) {
        CAST_CHECKER(true, data, asString, StringDataInformation);
    } else {
        CAST_CHECKER(false, data, asString, StringDataInformation);
    }

    QCOMPARE(data->isDummy(), expected.isDummy);
    if (expected.isDummy) {
        CAST_CHECKER(true, data, asDummy, DummyDataInformation);
        return; // the other checks cannot be done with a dummy
    }

    CAST_CHECKER(false, data, asDummy, DummyDataInformation);

    DataInformation* dataInf = data->asDataInformation();
    QVERIFY(dataInf);

    QCOMPARE(dataInf->size(), expected.size);
    QCOMPARE(dataInf->parent(), expected.parent);

    DataInformation* clone1 = (dataInf->clone());
    QScopedPointer<TopLevelDataInformation> top(new TopLevelDataInformation(clone1));
    QCOMPARE(clone1->parent(), top.data()); // top takes ownership of clone1
    QCOMPARE(top->actualDataInformation(), clone1);

    QScopedPointer<DataInformation> clone2(clone1->clone());
    QVERIFY(clone2->parent() == nullptr); // cloning should reset parent to NULL, else we get dangling pointers

    QCOMPARE(dataInf->flags(DataInformation::ColumnName, true), expected.columnFlags[DataInformation::ColumnName]);
    QCOMPARE(dataInf->flags(DataInformation::ColumnType, true), expected.columnFlags[DataInformation::ColumnType]);
    QCOMPARE(dataInf->flags(DataInformation::ColumnValue, true), expected.columnFlags[DataInformation::ColumnValue]);

    QCOMPARE(dataInf->flags(DataInformation::ColumnName, false), expected.noFileColumnFlags[DataInformation::ColumnName]);
    QCOMPARE(dataInf->flags(DataInformation::ColumnType, false), expected.noFileColumnFlags[DataInformation::ColumnType]);
    QCOMPARE(dataInf->flags(DataInformation::ColumnValue, false), expected.noFileColumnFlags[DataInformation::ColumnValue]);
}

void BasicDataInformationTest::initTestCase()
{
//	qRegisterMetaType<const DataInformation*>();
//	qRegisterMetaType<DataInformation*>();
    LoggerWithContext lwc(nullptr, QString());

    PrimitiveDataType type = PrimitiveDataType::START;
    while (type < PrimitiveDataType::Bitfield) {
        primitives.append(PrimitiveFactory::newInstance(QStringLiteral("prim"), type, lwc));
        type = static_cast<PrimitiveDataType>(static_cast<int>(type) + 1);
    }
    QCOMPARE(PrimitiveFactory::newInstance(QStringLiteral("invalid"), PrimitiveDataType::Bitfield, lwc), static_cast<PrimitiveDataInformation*>(nullptr));
    QCOMPARE(PrimitiveFactory::newInstance(QStringLiteral("invalid"), QStringLiteral("invalid_type"), lwc), static_cast<PrimitiveDataInformation*>(nullptr));
    bitfields.append(new BoolBitfieldDataInformation(QStringLiteral("bitfield"), 24));
    bitfields.append(new UnsignedBitfieldDataInformation(QStringLiteral("bitfield"), 24));
    bitfields.append(new SignedBitfieldDataInformation(QStringLiteral("bitfield"), 24));

    emptyStruct = new StructureDataInformation(QStringLiteral("emptyStruct"));
    QVector<DataInformation*> structChildren;
    structChildren << PrimitiveFactory::newInstance(QStringLiteral("prim"), PrimitiveDataType::UInt32, lwc)
                   << PrimitiveFactory::newInstance(QStringLiteral("prim2"), PrimitiveDataType::UInt64, lwc);
    structWithChildren = new StructureDataInformation(QStringLiteral("structWithChildren"), structChildren);

    emptyUnion = new UnionDataInformation(QStringLiteral("emptyUnion"));
    QVector<DataInformation*> unionChildren;
    unionChildren << PrimitiveFactory::newInstance(QStringLiteral("prim"), PrimitiveDataType::UInt32, lwc)
                  << PrimitiveFactory::newInstance(QStringLiteral("prim2"), PrimitiveDataType::UInt64, lwc);
    unionWithChildren = new UnionDataInformation(QStringLiteral("unionWithChildren"), unionChildren);

    emptyPrimitiveArray = new ArrayDataInformation(QStringLiteral("emptyPrimitiveArray"), 0, PrimitiveFactory::newInstance(QStringLiteral("prim"), PrimitiveDataType::UInt32, lwc));
    emptyComplexArray = new ArrayDataInformation(QStringLiteral("emptyComplexArray"), 0, structWithChildren->clone());
    primitiveArrayWithChildren = new ArrayDataInformation(QStringLiteral("primitiveArrayWithChildren"), 2, PrimitiveFactory::newInstance(QStringLiteral("prim"), PrimitiveDataType::UInt32, lwc));
    complexArrayWithChildren = new ArrayDataInformation(QStringLiteral("complexArrayWithChildren"), 2, structWithChildren->clone());

    QMap<AllPrimitiveTypes, QString> enumVals;
    enumVals[1] = QStringLiteral("one");
    enumVals[2] = QStringLiteral("two");
    enumVals[4] = QStringLiteral("four");
    EnumDefinition::Ptr edef(new EnumDefinition(enumVals, QStringLiteral("eDef"), PrimitiveDataType::UInt32));
    flagData = new FlagDataInformation(QStringLiteral("flagData"), PrimitiveFactory::newInstance(QStringLiteral("prim"), PrimitiveDataType::UInt32, lwc), edef);
    enumData = new EnumDataInformation(QStringLiteral("enumData"), PrimitiveFactory::newInstance(QStringLiteral("prim"), PrimitiveDataType::UInt32, lwc), edef);
    emptyString = new StringDataInformation(QStringLiteral("string"), StringDataInformation::StringType::ASCII);
    dummy = new DummyDataInformation(nullptr);
    topLevel = new TopLevelDataInformation(new DummyDataInformation(nullptr));
}

void BasicDataInformationTest::testBitfields()
{
    ExpectedResults exp;
    exp.isPrimitive = true;
    exp.isBitfield = true;
    exp.size = 24;
    exp.columnFlags[DataInformation::ColumnValue] = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    for (auto* bitField : qAsConst(bitfields)) {
        basicTest(bitField, exp);
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
        QCOMPARE(t, PrimitiveDataType(static_cast<PrimitiveDataType>(i)));
        QCOMPARE(data->type(), PrimitiveDataType(static_cast<PrimitiveDataType>(i)));
        if (t == PrimitiveDataType::Bool8 || t ==  PrimitiveDataType::Int8 || t == PrimitiveDataType::UInt8 || t == PrimitiveDataType::Char) {
            exp.size = 8;
        } else if (t == PrimitiveDataType::Bool16 || t == PrimitiveDataType::Int16 || t == PrimitiveDataType::UInt16) {
            exp.size = 16;
        } else if (t == PrimitiveDataType::Bool32 || t == PrimitiveDataType::Int32 || t == PrimitiveDataType::UInt32 || t == PrimitiveDataType::Float) {
            exp.size = 32;
        } else if (t == PrimitiveDataType::Bool64 || t == PrimitiveDataType::Int64 || t == PrimitiveDataType::UInt64 || t == PrimitiveDataType::Double) {
            exp.size = 64;
        } else {
            QVERIFY(false);
        }
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

void BasicDataInformationTest::testDummy()
{
    ExpectedResults exp;
    exp.isDummy = true;
    exp.size = 0;
    basicTest(dummy, exp);
}

void BasicDataInformationTest::testString()
{
    ExpectedResults exp;
    exp.isString = true;
    exp.canHaveChildren = true;
    exp.size = 0;
    basicTest(emptyString, exp);
}

void BasicDataInformationTest::testTopLevel()
{
    ExpectedResults exp;
    exp.isTopLevel = true;
    basicTest(topLevel, exp);
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
    delete emptyString;
    delete dummy;
    delete topLevel;
}

QTEST_GUILESS_MAIN(BasicDataInformationTest)

#include "basicdatainformationtest.moc"
