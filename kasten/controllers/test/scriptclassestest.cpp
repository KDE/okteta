/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    SPDX-FileCopyrightText: 2012, 2013 Alex Richardson <alex.richardson@gmx.de>
 *
 *    SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QTest>

#include <QObject>
#include <QScriptValueIterator>
#include <QScriptEngine>

#include "view/structures/datatypes/primitive/primitivetemplateinfo.hpp"
#include "view/structures/datatypes/primitive/primitivedatainformation.hpp"
#include "view/structures/datatypes/primitive/enumdatainformation.hpp"
#include "view/structures/datatypes/primitive/flagdatainformation.hpp"
#include "view/structures/datatypes/primitive/pointerdatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/boolbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/signedbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitivefactory.hpp"
#include "view/structures/datatypes/topleveldatainformation.hpp"
#include "view/structures/datatypes/strings/stringdatainformation.hpp"
#include "view/structures/datatypes/array/arraydatainformation.hpp"
#include "view/structures/datatypes/structuredatainformation.hpp"
#include "view/structures/datatypes/uniondatainformation.hpp"
#include "view/structures/script/scripthandler.hpp"
#include "view/structures/script/classes/arrayscriptclass.hpp"
#include "view/structures/script/classes/primitivescriptclass.hpp"
#include "view/structures/script/classes/structunionscriptclass.hpp"
#include "view/structures/script/classes/stringscriptclass.hpp"
#include "view/structures/script/classes/enumscriptclass.hpp"
#include "view/structures/script/classes/bitfieldscriptclass.hpp"
#include "view/structures/script/classes/pointerscriptclass.hpp"
#include "view/structures/script/scriptengineinitializer.hpp"
#include "view/structures/script/classes/defaultscriptclass.hpp"
#include "view/structures/script/safereference.hpp"
#include "view/structures/parsers/scriptvalueconverter.hpp"
#include "testutils.hpp"

class ScriptClassesTest : public QObject
{
    Q_OBJECT

    using PropertyPair = QPair<QString, QScriptValue::PropertyFlags>;

private:
    static void checkProperties(const QVector<PropertyPair>& expected, DataInformation* data,
                                const char* tag);
    static PropertyPair pair(const char* name,
                             QScriptValue::PropertyFlags flags = QScriptValue::Undeletable | QScriptValue::ReadOnly)
    {
        return PropertyPair(QString::fromUtf8(name), flags);
    }

private Q_SLOTS:
    void initTestCase();
    // check that all properties are available in the iterator
    void checkIterators();
    void testReplaceObject(); // check replacing datatype
    void cleanupTestCase();
    void testSafeReferenceDeleteObject();
    void testSafePrimitiveArrayReference();
    void testScriptValueContents_data();
    void testScriptValueContents();

private:
    QVector<PropertyPair> commonProperties;
    QVector<PropertyPair> primitiveProperties;
    QVector<TopLevelDataInformation*> primitives;

    QVector<PropertyPair> enumProperties;
    EnumDataInformation* enumData;
    QScopedPointer<TopLevelDataInformation> enumDataTop;
    FlagDataInformation* flagData;
    QScopedPointer<TopLevelDataInformation> flagDataTop;

    QVector<PropertyPair> bitfieldProperties;
    SignedBitfieldDataInformation* signedBitfield;
    QScopedPointer<TopLevelDataInformation> signedBitfieldTop;
    UnsignedBitfieldDataInformation* unsignedBitfield;
    QScopedPointer<TopLevelDataInformation> unsignedBitfieldTop;
    BoolBitfieldDataInformation* boolBitfield;
    QScopedPointer<TopLevelDataInformation> boolBitfieldTop;

    QVector<PropertyPair> structUnionProperties; // without children
    StructureDataInformation* structData;
    QScopedPointer<TopLevelDataInformation> structDataTop;
    UnionDataInformation* unionData;
    QScopedPointer<TopLevelDataInformation> unionDataTop;

    QVector<PropertyPair> arrayProperties; // without children
    ArrayDataInformation* arrayData;
    QScopedPointer<TopLevelDataInformation> arrayDataTop;

    QVector<PropertyPair> stringProperties; // without children
    StringDataInformation* stringData;
    QScopedPointer<TopLevelDataInformation> stringDataTop;

};

void ScriptClassesTest::initTestCase()
{
    // we are only testing properties when updating
    // TODO fix this
    commonProperties << pair("name", QScriptValue::Undeletable)
                     << pair("wasAbleToRead") << pair("parent")
                     << pair("valid")
                     << pair("validationError")
                     << pair("byteOrder", QScriptValue::Undeletable)
                     << pair("updateFunc", QScriptValue::Undeletable)
                     << pair("validationFunc", QScriptValue::Undeletable)
                     << pair("datatype", QScriptValue::Undeletable)
                     << pair("typeName", QScriptValue::Undeletable)
                     << pair("toStringFunc", QScriptValue::Undeletable);

    primitiveProperties << commonProperties << pair("value") << pair("char") << pair("int")
                        << pair("int8") << pair("int16") << pair("int32") << pair("int64") << pair("uint")
                        << pair("uint8") << pair("uint16") << pair("uint32") << pair("uint64") << pair("bool")
                        << pair("float") << pair("double") << pair("int64high32") << pair("int64low32")
                        << pair("uint64high32") << pair("uint64low32") << pair("type");
    std::sort(primitiveProperties.begin(), primitiveProperties.end());
    LoggerWithContext lwc(nullptr, QString());
    PrimitiveDataType type = PrimitiveDataType::START;
    while (type < PrimitiveDataType::Bitfield) {
        PrimitiveDataInformation* prim = PrimitiveFactory::newInstance(QStringLiteral("prim"), type, lwc);
        prim->setValue(10);
        primitives << new TopLevelDataInformation(prim);
        type = static_cast<PrimitiveDataType>(static_cast<int>(type) + 1);
    }

    enumProperties << primitiveProperties << pair("enumValues", QScriptValue::Undeletable);
    // TODO valueString property (i.e. the current value as enumerator name)
    // XXX enumName
    std::sort(enumProperties.begin(), enumProperties.end());

    QMap<AllPrimitiveTypes, QString> enumValues;
    enumValues.insert(1, QStringLiteral("one"));
    enumValues.insert(2, QStringLiteral("tow"));
    enumValues.insert(4, QStringLiteral("four"));
    EnumDefinition::Ptr enumDef(new EnumDefinition(enumValues,
                                                   QStringLiteral("theEnum"), PrimitiveDataType::Int32));
    enumData = new EnumDataInformation(QStringLiteral("enumData"),
                                       PrimitiveFactory::newInstance(QStringLiteral("dummy"), PrimitiveDataType::Int32, lwc), enumDef);
    enumDataTop.reset(
        new TopLevelDataInformation(enumData, nullptr, ScriptEngineInitializer::newEngine()));
    flagData = new FlagDataInformation(QStringLiteral("flagData"),
                                       PrimitiveFactory::newInstance(QStringLiteral("dummy"), PrimitiveDataType::Int32, lwc), enumDef);
    flagDataTop.reset(
        new TopLevelDataInformation(flagData, nullptr, ScriptEngineInitializer::newEngine()));

    bitfieldProperties << primitiveProperties << pair("width", QScriptValue::Undeletable);
    std::sort(bitfieldProperties.begin(), bitfieldProperties.end());
    unsignedBitfield = new UnsignedBitfieldDataInformation(QStringLiteral("unsignedBit"), 42);
    unsignedBitfieldTop.reset(
        new TopLevelDataInformation(unsignedBitfield, nullptr, ScriptEngineInitializer::newEngine()));
    signedBitfield = new SignedBitfieldDataInformation(QStringLiteral("signedBit"), 42);
    signedBitfieldTop.reset(
        new TopLevelDataInformation(signedBitfield, nullptr, ScriptEngineInitializer::newEngine()));
    boolBitfield = new BoolBitfieldDataInformation(QStringLiteral("boolBit"), 42);
    boolBitfieldTop.reset(
        new TopLevelDataInformation(boolBitfield, nullptr, ScriptEngineInitializer::newEngine()));

    stringProperties << commonProperties << pair("terminatedBy", QScriptValue::Undeletable)
                     << pair("byteCount") << pair("maxCharCount", QScriptValue::Undeletable)
                     << pair("charCount") << pair("encoding", QScriptValue::Undeletable)
                     << pair("maxByteCount", QScriptValue::Undeletable);
    std::sort(stringProperties.begin(), stringProperties.end());
    stringData = new StringDataInformation(QStringLiteral("string"), StringDataInformation::StringType::Latin1);
    stringDataTop.reset(
        new TopLevelDataInformation(stringData, nullptr, ScriptEngineInitializer::newEngine()));

    arrayProperties << commonProperties << pair("length", QScriptValue::Undeletable)
                    << pair("type", QScriptValue::Undeletable);
    std::sort(arrayProperties.begin(), arrayProperties.end());
    arrayData = new ArrayDataInformation(QStringLiteral("array"), 20,
                                         PrimitiveFactory::newInstance(QStringLiteral("inner"), PrimitiveDataType::Int32, lwc));
    arrayDataTop.reset(
        new TopLevelDataInformation(arrayData, nullptr, ScriptEngineInitializer::newEngine()));

    structUnionProperties << commonProperties << pair("childCount");
    // property children is only writable -> it is not in the iterator
    structData = new StructureDataInformation(QStringLiteral("struct"));
    structDataTop.reset(
        new TopLevelDataInformation(structData, nullptr, ScriptEngineInitializer::newEngine()));
    unionData = new UnionDataInformation(QStringLiteral("union"));
    unionDataTop.reset(
        new TopLevelDataInformation(unionData, nullptr, ScriptEngineInitializer::newEngine()));
    std::sort(structUnionProperties.begin(), structUnionProperties.end());

}

Q_DECLARE_METATYPE(QScriptClass*)

static inline void scriptValueContentsAddRow(const char* tag, DataInformation* data, QScriptClass* cls)
{
    QTest::newRow(tag) << data << cls;
}

void ScriptClassesTest::testScriptValueContents_data()
{
    QTest::addColumn<DataInformation*>("data");
    QTest::addColumn<QScriptClass*>("scriptClass");

    scriptValueContentsAddRow("struct", structData,
                              structDataTop->scriptHandler()->handlerInfo()->mStructUnionClass.data());
    scriptValueContentsAddRow("union", unionData,
                              unionDataTop->scriptHandler()->handlerInfo()->mStructUnionClass.data());
    scriptValueContentsAddRow("array", arrayData,
                              arrayDataTop->scriptHandler()->handlerInfo()->mArrayClass.data());
    scriptValueContentsAddRow("string", stringData,
                              stringDataTop->scriptHandler()->handlerInfo()->mStringClass.data());
}

void ScriptClassesTest::testScriptValueContents()
{
    QFETCH(DataInformation*, data);
    QFETCH(QScriptClass*, scriptClass);

    QScriptValue val = data->toScriptValue(data->topLevelDataInformation());
    QVERIFY(val.isValid());
    QVERIFY(val.isObject());
    QCOMPARE(val.scriptClass(), scriptClass);
    QVERIFY(val.data().isVariant());
    QVariant variant = val.data().toVariant();
    QVERIFY(variant.isValid());
    QVERIFY(variant.canConvert<SafeReference>());
    QCOMPARE(variant.value<SafeReference>().data(), data);
    QCOMPARE(DefaultScriptClass::toDataInformation(val), data);
}

void ScriptClassesTest::checkProperties(const QVector<PropertyPair>& expected,
                                        DataInformation* data, const char* tag)
{
    // check in updating mode
    // TODO check also in other modes
    data->topLevelDataInformation()->scriptHandler()->handlerInfo()->setMode(ScriptHandlerInfo::Mode::Updating);
    QScriptValue value = data->toScriptValue(data->topLevelDataInformation()->scriptEngine(),
                                             data->topLevelDataInformation()->scriptHandler()->handlerInfo());

    QScriptValueIterator it(value);
    QList<PropertyPair> foundProperties;
    while (it.hasNext()) {
        it.next();
        foundProperties.append(qMakePair(it.name(), it.flags()));
    }
    data->topLevelDataInformation()->scriptHandler()->handlerInfo()->setMode(ScriptHandlerInfo::Mode::None);
    std::sort(foundProperties.begin(), foundProperties.end());
    if (foundProperties.size() != expected.size()) {
        for (int i = 0; i < qMin(foundProperties.size(), expected.size()); ++i) {
            if (foundProperties.at(i) != expected.at(i)) {
                qWarning() << tag << ":" << foundProperties.at(i) << ", but expected:" << expected.at(i);
            }
            QCOMPARE(foundProperties.at(i).first, expected.at(i).first);
            QCOMPARE(foundProperties.at(i).second, expected.at(i).second);
        }
    }
    for (int i = 0; i < foundProperties.size(); ++i) {
        if (foundProperties.at(i) != expected.at(i)) {
            qWarning() << tag << ":" << foundProperties.at(i) << "!=" << expected.at(i);
        }
        QCOMPARE(foundProperties.at(i).first, expected.at(i).first);
        QCOMPARE(foundProperties.at(i).second, expected.at(i).second);
    }

    QCOMPARE(foundProperties.size(), expected.size());
}

void ScriptClassesTest::checkIterators()
{
    for (auto* top : qAsConst(primitives)) {
        checkProperties(primitiveProperties, top->actualDataInformation(), "primitives");
    }

    checkProperties(enumProperties, enumData, "enum");
    checkProperties(enumProperties, flagData, "flag");

    checkProperties(bitfieldProperties, boolBitfield, "bool bitfield");
    checkProperties(bitfieldProperties, signedBitfield, "signed bitfield");
    checkProperties(bitfieldProperties, unsignedBitfield, "unsignedBitfield");

    checkProperties(stringProperties, stringData, "string");

    checkProperties(arrayProperties, arrayData, "array");

    checkProperties(structUnionProperties, structData, "struct");
    checkProperties(structUnionProperties, unionData, "union");
}

void ScriptClassesTest::testReplaceObject()
{
    QScriptEngine* eng = ScriptEngineInitializer::newEngine();
    auto* logger = new ScriptLogger();
    logger->setLogToStdOut(true);
    QString unionDef = QStringLiteral(
        "union({\n"
        QT_UNICODE_LITERAL("    innerStruct : struct({ first : uint8(), second : uint16() }),\n")
        QT_UNICODE_LITERAL("    innerArray : array(uint8(), 5),\n")
        QT_UNICODE_LITERAL("    innerPointer : pointer(uint8(), double())\n")
        QT_UNICODE_LITERAL("});\n"));
    QScriptValue val = eng->evaluate(unionDef);
    QVERIFY(val.isObject());
    DataInformation* main = ScriptValueConverter::convert(val, QStringLiteral("container"), logger, nullptr);
    QVERIFY(main);
    QCOMPARE(logger->rowCount(), 0);
    TopLevelDataInformation top(main, logger, eng);

    // first we read the struct, which changes the type of the first child
    // access it again after changing to ensure it was set properly
    QScriptValue structUpdate = eng->evaluate(QStringLiteral(
                                                  "(function() { this.first.datatype = int32(); this.first.name = \"changed\"; })"));
    QVERIFY(structUpdate.isFunction());
    StructureDataInformation* structData = main->childAt(0)->asStruct();
    QVERIFY(structData);
    structData->setUpdateFunc(structUpdate);
    QCOMPARE(structData->name(), QStringLiteral("innerStruct"));

    // array changes its own type, this is the critical one
    // access it again after changing to ensure it was set properly
    QScriptValue arrayUpdate = eng->evaluate(QStringLiteral(
                                                 "(function() { this.datatype = float(); this.name = \"changedToFloat\"; })"));
    ArrayDataInformation* arrayData = main->childAt(1)->asArray();
    arrayData->setUpdateFunc(arrayUpdate);

    QVERIFY(arrayData);
    QScriptValue pointerTargetUpdate = eng->evaluate(QStringLiteral(
                                                         "(function() { this.datatype = array(int8(), 5); this.parent.name = \"changedToArrayPointer\"; })"));
    PointerDataInformation* ptrData = main->childAt(2)->asPointer();
    QVERIFY(ptrData);
    ptrData->pointerTarget()->setUpdateFunc(pointerTargetUpdate);

    QScriptValue unionUpdate = eng->evaluate(QStringLiteral(
                                                 "(function() { this.datatype = ") + unionDef + QStringLiteral(" this.name = \"newContainer\"; })"));
    main->setUpdateFunc(unionUpdate);

    // now just call update
    QCOMPARE(structData->childCount(), 2u);
    QCOMPARE((int)structData->childAt(0)->asPrimitive()->type(), (int)PrimitiveDataType::UInt8);
    QCOMPARE(structData->childAt(0)->name(), QStringLiteral("first"));
    QCOMPARE(structData->childAt(1)->name(), QStringLiteral("second"));
    top.scriptHandler()->updateDataInformation(structData);
    // now structdata should have different children
    QCOMPARE(structData->childCount(), 2u);
    QCOMPARE((int)structData->childAt(0)->asPrimitive()->type(), (int)PrimitiveDataType::Int32); // different now
    QCOMPARE(structData->childAt(0)->name(), QStringLiteral("changed")); // different now
    QCOMPARE(structData->childAt(1)->name(), QStringLiteral("second")); // still the same

    QCOMPARE(arrayData->name(), QStringLiteral("innerArray"));
    top.scriptHandler()->updateDataInformation(arrayData);
    QVERIFY(main->childAt(1)->hasBeenUpdated());
    QVERIFY(main->childAt(1)->isPrimitive());
    QCOMPARE(main->childAt(1)->name(), QStringLiteral("changedToFloat"));

    QCOMPARE(ptrData->name(), QStringLiteral("innerPointer"));
    QVERIFY(main->childAt(2)->isPointer());
    QVERIFY(main->childAt(2)->asPointer()->pointerTarget()->isPrimitive());
    top.scriptHandler()->updateDataInformation(ptrData->pointerTarget());
    QVERIFY(main->childAt(2)->isPointer());
    QVERIFY(main->childAt(2)->asPointer()->pointerTarget()->isArray());
    QCOMPARE(main->childAt(2)->name(), QStringLiteral("changedToArrayPointer"));

    // now reset to state before
    QCOMPARE(main->name(), QStringLiteral("container"));
    top.scriptHandler()->updateDataInformation(main);
    // main is now a dangling pointer
    main = top.actualDataInformation();
    QString nnnname = QStringLiteral("newContainer");
    QCOMPARE(main->name(), nnnname);
    QVERIFY(main->childAt(0)->isStruct());
    QCOMPARE(main->childAt(0)->name(), QStringLiteral("innerStruct"));
    QCOMPARE(main->childAt(1)->name(), QStringLiteral("innerArray"));
    QCOMPARE(main->childAt(2)->name(), QStringLiteral("innerPointer"));
}

namespace {
QString invalidObjectError() { return QStringLiteral("ReferenceError: Attempting to access an invalid object"); }
}

void ScriptClassesTest::testSafePrimitiveArrayReference()
{
    QVERIFY(arrayData->arrayType()->isPrimitive());
    QVERIFY(arrayData->length() > 2);
    arrayDataTop->logger()->setLogToStdOut(true);
    QScriptEngine* eng = arrayDataTop->scriptEngine();
    eng->pushContext();
    eng->currentContext()->activationObject().setProperty(QStringLiteral("myArray"),
                                                          arrayData->toScriptValue(arrayDataTop.data()));
    arrayDataTop->scriptHandler()->handlerInfo()->setMode(ScriptHandlerInfo::Mode::Updating);
    QScriptValue v0 = eng->evaluate(QStringLiteral("myArray[0]"));
    QCOMPARE(Utils::property(v0, "name").toString(), QString::number(0));
    QVERIFY(DefaultScriptClass::toDataInformation(v0) != nullptr);
    // access index 1 -> index 0 should become invalid, since there is only one object available
    QScriptValue v1 = eng->evaluate(QStringLiteral("myArray[1]"));
    QVERIFY(DefaultScriptClass::toDataInformation(v1) != nullptr);
    QVERIFY(DefaultScriptClass::toDataInformation(v0) == nullptr);
    QVERIFY(!eng->hasUncaughtException());
    QCOMPARE(Utils::property(v1, "name").toString(), QString::number(1));
    QVERIFY(!eng->hasUncaughtException());
    QCOMPARE(Utils::property(v0, "name").toString(), invalidObjectError());
    QVERIFY(!eng->hasUncaughtException());
    // even after accessing a v0 property (which will fail), v1 properties should remain valid
    QCOMPARE(Utils::property(v1, "name").toString(), QString::number(1));
    QVERIFY(!eng->hasUncaughtException());
    arrayDataTop->scriptHandler()->handlerInfo()->setMode(ScriptHandlerInfo::Mode::None);
    eng->popContext();
}

void ScriptClassesTest::testSafeReferenceDeleteObject()
{
    QScopedPointer<TopLevelDataInformation> top(Utils::evalAndParse("struct({bar: uint8()}).set({name: 'foo'});"));
    QVERIFY(top->actualDataInformation()->isStruct());
    top->scriptHandler()->handlerInfo()->setMode(ScriptHandlerInfo::Mode::TaggedUnionSelection);
    QScriptValue val = top->actualDataInformation()->toScriptValue(top.data());
    QScriptValue name = Utils::property(val, "name");
    QVERIFY(name.isValid());
    QVERIFY(!name.isError());
    QCOMPARE(name.toString(), QStringLiteral("foo"));
    top->setActualDataInformation(new DummyDataInformation(nullptr));
    // val should now point to an invalid reference -> accessing name should throw an error
    name = Utils::property(val, "name");
    QVERIFY(name.isValid());
    QVERIFY(name.isError());
    QCOMPARE(name.toString(), invalidObjectError());
}

void ScriptClassesTest::cleanupTestCase()
{
    qDeleteAll(primitives);
}

QTEST_GUILESS_MAIN(ScriptClassesTest)

#include "scriptclassestest.moc"
