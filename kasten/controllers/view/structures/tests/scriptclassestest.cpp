/*
    SPDX-FileCopyrightText: 2012, 2013 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

// test
#include "testutils.hpp"
// sut
#include <primitivetemplateinfo.hpp>
#include <primitivedatainformation.hpp>
#include <enumdatainformation.hpp>
#include <flagdatainformation.hpp>
#include <pointerdatainformation.hpp>
#include <boolbitfielddatainformation.hpp>
#include <unsignedbitfielddatainformation.hpp>
#include <signedbitfielddatainformation.hpp>
#include <primitivefactory.hpp>
#include <topleveldatainformation.hpp>
#include <stringdatainformation.hpp>
#include <arraydatainformation.hpp>
#include <structuredatainformation.hpp>
#include <uniondatainformation.hpp>
#include <scripthandler.hpp>
#include <arrayscriptclass.hpp>
#include <primitivescriptclass.hpp>
#include <structunionscriptclass.hpp>
#include <stringscriptclass.hpp>
#include <enumscriptclass.hpp>
#include <bitfieldscriptclass.hpp>
#include <pointerscriptclass.hpp>
#include <scriptengineinitializer.hpp>
#include <defaultscriptclass.hpp>
#include <safereference.hpp>
#include <scriptvalueconverter.hpp>
// Qt
#include <QTest>
#include <QObject>
#include <QScriptValueIterator>
#include <QScriptEngine>
#include <QScriptContext>
// Std
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>

class ScriptClassesTest : public QObject
{
    Q_OBJECT

public:
    struct PropertyData
    {
        QString name;
        QScriptValue::PropertyFlags flags;

        bool operator==(const PropertyData& other) const
        {
            return (name == other.name) && (flags == other.flags);
        }
        bool operator!=(const PropertyData& other) const
        {
            return !(operator==(other));
        }
        bool operator<(const PropertyData& other) const
        {
            return (name < other.name) || (!(other.name < name) && flags < other.flags);
        }
    };

private:
    static void checkProperties(const std::vector<PropertyData>& expected, DataInformation* data,
                                const char* tag);
    static PropertyData property(const char* name,
                                 QScriptValue::PropertyFlags flags = QScriptValue::Undeletable | QScriptValue::ReadOnly)
    {
        return {QString::fromUtf8(name), flags};
    }

private Q_SLOTS:
    void initTestCase();

    // check that all properties are available in the iterator
    void checkIterators();
    void testReplaceObject(); // check replacing datatype
    void testSafeReferenceDeleteObject();
    void testSafePrimitiveArrayReference();
    void testScriptValueContents_data();
    void testScriptValueContents();

private:
    std::vector<PropertyData> commonProperties;
    std::vector<PropertyData> primitiveProperties;
    std::vector<std::unique_ptr<TopLevelDataInformation>> primitives;

    std::vector<PropertyData> enumProperties;
    EnumDataInformation* enumData;
    std::unique_ptr<TopLevelDataInformation> enumDataTop;
    FlagDataInformation* flagData;
    std::unique_ptr<TopLevelDataInformation> flagDataTop;

    std::vector<PropertyData> bitfieldProperties;
    SignedBitfieldDataInformation* signedBitfield;
    std::unique_ptr<TopLevelDataInformation> signedBitfieldTop;
    UnsignedBitfieldDataInformation* unsignedBitfield;
    std::unique_ptr<TopLevelDataInformation> unsignedBitfieldTop;
    BoolBitfieldDataInformation* boolBitfield;
    std::unique_ptr<TopLevelDataInformation> boolBitfieldTop;

    std::vector<PropertyData> structUnionProperties; // without children
    StructureDataInformation* structData;
    std::unique_ptr<TopLevelDataInformation> structDataTop;
    UnionDataInformation* unionData;
    std::unique_ptr<TopLevelDataInformation> unionDataTop;

    std::vector<PropertyData> arrayProperties; // without children
    ArrayDataInformation* arrayData;
    std::unique_ptr<TopLevelDataInformation> arrayDataTop;

    std::vector<PropertyData> stringProperties; // without children
    StringDataInformation* stringData;
    std::unique_ptr<TopLevelDataInformation> stringDataTop;

};

inline QDebug operator<<(QDebug debug, const ScriptClassesTest::PropertyData& pair)
{
    const bool oldAutoInsertSpacesSetting = debug.autoInsertSpaces();
    debug.nospace() << "Property(" << pair.name << ':' << pair.flags << ')';
    debug.setAutoInsertSpaces(oldAutoInsertSpacesSetting);
    return debug.maybeSpace();
}

void ScriptClassesTest::initTestCase()
{
    // we are only testing properties when updating
    // TODO fix this
    commonProperties = {
        property("name", QScriptValue::Undeletable),
        property("wasAbleToRead"),
        property("parent"),
        property("valid"),
        property("validationError"),
        property("byteOrder", QScriptValue::Undeletable),
        property("updateFunc", QScriptValue::Undeletable),
        property("validationFunc", QScriptValue::Undeletable),
        property("datatype", QScriptValue::Undeletable),
        property("typeName", QScriptValue::Undeletable),
        property("toStringFunc", QScriptValue::Undeletable),
    };

    primitiveProperties = commonProperties;
    primitiveProperties.insert(primitiveProperties.end(), {
        property("value"), property("char"), property("int"),
        property("int8"),  property("int16"),  property("int32"),  property("int64"),  property("uint"),
        property("uint8"),  property("uint16"),  property("uint32"),  property("uint64"),  property("bool"),
        property("float"),  property("double"),  property("int64high32"),  property("int64low32"),
        property("uint64high32"),  property("uint64low32"),  property("type"),
    });
    std::sort(primitiveProperties.begin(), primitiveProperties.end());

    LoggerWithContext lwc(nullptr, QString());
    PrimitiveDataType type = PrimitiveDataType::START;
    while (type < PrimitiveDataType::Bitfield) {
        auto prim = PrimitiveFactory::newInstance(QStringLiteral("prim"), type, lwc);
        prim->setValue(10);
        primitives.emplace_back(std::make_unique<TopLevelDataInformation>(std::move(prim)));
        type = static_cast<PrimitiveDataType>(static_cast<int>(type) + 1);
    }

    enumProperties = primitiveProperties;
    enumProperties.emplace_back(property("enumValues", QScriptValue::Undeletable));
    // TODO valueString property (i.e. the current value as enumerator name)
    // XXX enumName
    std::sort(enumProperties.begin(), enumProperties.end());

    std::map<AllPrimitiveTypes, QString> enumValues {
        {1, QStringLiteral("one")},
        {2, QStringLiteral("two")},
        {4, QStringLiteral("four")},
    };
    auto enumDef = std::make_shared<EnumDefinition>(std::move(enumValues),
                                                    QStringLiteral("theEnum"), PrimitiveDataType::Int32);
    auto managedEnumData = std::make_unique<EnumDataInformation>(QStringLiteral("enumData"),
                                                                 PrimitiveFactory::newInstance(QStringLiteral("dummy"), PrimitiveDataType::Int32, lwc), enumDef);
    enumData = managedEnumData.get();
    enumDataTop = std::make_unique<TopLevelDataInformation>(std::move(managedEnumData), nullptr, ScriptEngineInitializer::newEngine());
    auto managedFlagData = std::make_unique<FlagDataInformation>(QStringLiteral("flagData"),
                                                                 PrimitiveFactory::newInstance(QStringLiteral("dummy"), PrimitiveDataType::Int32, lwc), enumDef);
    flagData = managedFlagData.get();
    flagDataTop = std::make_unique<TopLevelDataInformation>(std::move(managedFlagData), nullptr, ScriptEngineInitializer::newEngine());

    bitfieldProperties = primitiveProperties;
    bitfieldProperties.emplace_back(property("width", QScriptValue::Undeletable));
    std::sort(bitfieldProperties.begin(), bitfieldProperties.end());

    auto managedUnsignedBitfield = std::make_unique<UnsignedBitfieldDataInformation>(QStringLiteral("unsignedBit"), 42);
    unsignedBitfield = managedUnsignedBitfield.get();
    unsignedBitfieldTop = std::make_unique<TopLevelDataInformation>(std::move(managedUnsignedBitfield), nullptr, ScriptEngineInitializer::newEngine());
    auto managedSignedBitfield = std::make_unique<SignedBitfieldDataInformation>(QStringLiteral("signedBit"), 42);
    signedBitfield = managedSignedBitfield.get();
    signedBitfieldTop = std::make_unique<TopLevelDataInformation>(std::move(managedSignedBitfield), nullptr, ScriptEngineInitializer::newEngine());
    auto managedBoolBitfield = std::make_unique<BoolBitfieldDataInformation>(QStringLiteral("boolBit"), 42);
    boolBitfield = managedBoolBitfield.get();
    boolBitfieldTop = std::make_unique<TopLevelDataInformation>(std::move(managedBoolBitfield), nullptr, ScriptEngineInitializer::newEngine());

    stringProperties = commonProperties;
    stringProperties.insert(stringProperties.end(), {
        property("terminatedBy", QScriptValue::Undeletable),
        property("byteCount"),
        property("maxCharCount", QScriptValue::Undeletable),
        property("charCount"),
        property("encoding", QScriptValue::Undeletable),
        property("maxByteCount", QScriptValue::Undeletable),
    });
    std::sort(stringProperties.begin(), stringProperties.end());
    auto managedStringData = std::make_unique<StringDataInformation>(QStringLiteral("string"), StringDataInformation::StringType::Latin1);
    stringData = managedStringData.get();
    stringDataTop = std::make_unique<TopLevelDataInformation>(std::move(managedStringData), nullptr, ScriptEngineInitializer::newEngine());

    arrayProperties = commonProperties;
    arrayProperties.insert(arrayProperties.end(), {
        property("length", QScriptValue::Undeletable),
        property("type", QScriptValue::Undeletable),
    });
    std::sort(arrayProperties.begin(), arrayProperties.end());
    auto managedArrayData = std::make_unique<ArrayDataInformation>(QStringLiteral("array"), 20,
                                                                   PrimitiveFactory::newInstance(QStringLiteral("inner"), PrimitiveDataType::Int32, lwc),
                                                                   lwc);
    arrayData = managedArrayData.get();
    arrayDataTop = std::make_unique<TopLevelDataInformation>(std::move(managedArrayData), nullptr, ScriptEngineInitializer::newEngine());

    structUnionProperties = commonProperties;
    structUnionProperties.emplace_back(property("childCount"));
    // property children is only writable -> it is not in the iterator
    auto managedStructData = std::make_unique<StructureDataInformation>(QStringLiteral("struct"));
    structData = managedStructData.get();
    structDataTop = std::make_unique<TopLevelDataInformation>(std::move(managedStructData), nullptr, ScriptEngineInitializer::newEngine());
    auto managedUnionData = std::make_unique<UnionDataInformation>(QStringLiteral("union"));
    unionData = managedUnionData.get();
    unionDataTop = std::make_unique<TopLevelDataInformation>(std::move(managedUnionData), nullptr, ScriptEngineInitializer::newEngine());
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
                              structDataTop->scriptHandler()->handlerInfo()->structUnionClass());
    scriptValueContentsAddRow("union", unionData,
                              unionDataTop->scriptHandler()->handlerInfo()->structUnionClass());
    scriptValueContentsAddRow("array", arrayData,
                              arrayDataTop->scriptHandler()->handlerInfo()->arrayClass());
    scriptValueContentsAddRow("string", stringData,
                              stringDataTop->scriptHandler()->handlerInfo()->stringClass());
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

void ScriptClassesTest::checkProperties(const std::vector<PropertyData>& expected,
                                        DataInformation* data, const char* tag)
{
    // check in updating mode
    // TODO check also in other modes
    data->topLevelDataInformation()->scriptHandler()->handlerInfo()->setMode(ScriptHandlerInfo::Mode::Updating);
    QScriptValue value = data->toScriptValue(data->topLevelDataInformation()->scriptEngine(),
                                             data->topLevelDataInformation()->scriptHandler()->handlerInfo());

    QScriptValueIterator it(value);
    std::vector<PropertyData> foundProperties;
    while (it.hasNext()) {
        it.next();
        foundProperties.emplace_back(PropertyData{it.name(), it.flags()});
    }
    data->topLevelDataInformation()->scriptHandler()->handlerInfo()->setMode(ScriptHandlerInfo::Mode::None);
    std::sort(foundProperties.begin(), foundProperties.end());
    if (foundProperties.size() != expected.size()) {
        for (std::size_t i = 0; i < std::min(foundProperties.size(), expected.size()); ++i) {
            if (foundProperties.at(i) != expected.at(i)) {
                qWarning() << tag << ":" << foundProperties.at(i) << ", but expected:" << expected.at(i);
            }
            QCOMPARE(foundProperties.at(i).name, expected.at(i).name);
            QCOMPARE(foundProperties.at(i).flags, expected.at(i).flags);
        }
    }
    for (std::size_t i = 0; i < foundProperties.size(); ++i) {
        if (foundProperties.at(i) != expected.at(i)) {
            qWarning() << tag << ":" << foundProperties.at(i) << "!=" << expected.at(i);
        }
        QCOMPARE(foundProperties.at(i).name, expected.at(i).name);
        QCOMPARE(foundProperties.at(i).flags, expected.at(i).flags);
    }

    QCOMPARE(foundProperties.size(), expected.size());
}

void ScriptClassesTest::checkIterators()
{
    for (const auto& top : primitives) {
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
    std::unique_ptr<QScriptEngine> ownedEngine = ScriptEngineInitializer::newEngine();
    auto logger = std::make_unique<ScriptLogger>();
    logger->setLogToStdOut(true);
    QString unionDef = QStringLiteral(
        "union({\n"
        QT_UNICODE_LITERAL("    innerStruct : struct({ first : uint8(), second : uint16() }),\n")
        QT_UNICODE_LITERAL("    innerArray : array(uint8(), 5),\n")
        QT_UNICODE_LITERAL("    innerPointer : pointer(uint8(), double())\n")
        QT_UNICODE_LITERAL("});\n"));
    QScriptValue val = ownedEngine->evaluate(unionDef);
    QVERIFY(val.isObject());
    std::unique_ptr<DataInformation> managedMain = ScriptValueConverter::convert(val, QStringLiteral("container"), logger.get(), nullptr);
    QVERIFY(managedMain);
    QCOMPARE(logger->rowCount(), 0);

    DataInformation* const main = managedMain.get();
    QScriptEngine* const eng = ownedEngine.get();
    TopLevelDataInformation top(std::move(managedMain), std::move(logger), std::move(ownedEngine));

    // first we read the struct, which changes the type of the first child
    // access it again after changing to ensure it was set properly
    QScriptValue structUpdate = eng->evaluate(QStringLiteral(
                                                  "(function() { this.first.datatype = int32(); this.first.name = \"changed\"; })"));
    QVERIFY(structUpdate.isFunction());
    StructureDataInformation* const innerStructData = main->childAt(0)->asStruct();
    QVERIFY(innerStructData);
    innerStructData->setUpdateFunc(structUpdate);
    QCOMPARE(innerStructData->name(), QStringLiteral("innerStruct"));

    // array changes its own type, this is the critical one
    // access it again after changing to ensure it was set properly
    QScriptValue arrayUpdate = eng->evaluate(QStringLiteral(
                                                 "(function() { this.datatype = float(); this.name = \"changedToFloat\"; })"));
    ArrayDataInformation* const innerArrayData = main->childAt(1)->asArray();
    innerArrayData->setUpdateFunc(arrayUpdate);

    QVERIFY(innerArrayData);
    QScriptValue pointerTargetUpdate = eng->evaluate(QStringLiteral(
                                                         "(function() { this.datatype = array(int8(), 5); this.parent.name = \"changedToArrayPointer\"; })"));
    PointerDataInformation* const ptrData = main->childAt(2)->asPointer();
    QVERIFY(ptrData);
    ptrData->pointerTarget()->setUpdateFunc(pointerTargetUpdate);

    QScriptValue unionUpdate = eng->evaluate(QStringLiteral(
                                                 "(function() { this.datatype = ") + unionDef + QStringLiteral(" this.name = \"newContainer\"; })"));
    main->setUpdateFunc(unionUpdate);

    // now just call update
    QCOMPARE(innerStructData->childCount(), 2U);
    QCOMPARE((int)innerStructData->childAt(0)->asPrimitive()->type(), (int)PrimitiveDataType::UInt8);
    QCOMPARE(innerStructData->childAt(0)->name(), QStringLiteral("first"));
    QCOMPARE(innerStructData->childAt(1)->name(), QStringLiteral("second"));
    top.scriptHandler()->updateDataInformation(innerStructData);
    // now structdata should have different children
    QCOMPARE(innerStructData->childCount(), 2U);
    QCOMPARE((int)innerStructData->childAt(0)->asPrimitive()->type(), (int)PrimitiveDataType::Int32); // different now
    QCOMPARE(innerStructData->childAt(0)->name(), QStringLiteral("changed")); // different now
    QCOMPARE(innerStructData->childAt(1)->name(), QStringLiteral("second")); // still the same

    QCOMPARE(innerArrayData->name(), QStringLiteral("innerArray"));
    top.scriptHandler()->updateDataInformation(innerArrayData);
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
    DataInformation* const newMain = top.actualDataInformation();
    QString nnnname = QStringLiteral("newContainer");
    QCOMPARE(newMain->name(), nnnname);
    QVERIFY(newMain->childAt(0)->isStruct());
    QCOMPARE(newMain->childAt(0)->name(), QStringLiteral("innerStruct"));
    QCOMPARE(newMain->childAt(1)->name(), QStringLiteral("innerArray"));
    QCOMPARE(newMain->childAt(2)->name(), QStringLiteral("innerPointer"));
}

namespace {
QString invalidObjectError() { return QStringLiteral("ReferenceError: Attempting to access an invalid object"); }
}

void ScriptClassesTest::testSafePrimitiveArrayReference()
{
    QVERIFY(arrayData->arrayType()->isPrimitive());
    QVERIFY(arrayData->length() > 2);
    arrayDataTop->logger()->setLogToStdOut(true);
    QScriptEngine* const eng = arrayDataTop->scriptEngine();
    std::ignore = eng->pushContext();
    eng->currentContext()->activationObject().setProperty(QStringLiteral("myArray"),
                                                          arrayData->toScriptValue(arrayDataTop.get()));
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
    std::unique_ptr<TopLevelDataInformation> top(Utils::evalAndParse("struct({bar: uint8()}).set({name: 'foo'});"));
    QVERIFY(top->actualDataInformation()->isStruct());
    top->scriptHandler()->handlerInfo()->setMode(ScriptHandlerInfo::Mode::TaggedUnionSelection);
    QScriptValue val = top->actualDataInformation()->toScriptValue(top.get());
    QScriptValue name = Utils::property(val, "name");
    QVERIFY(name.isValid());
    QVERIFY(!name.isError());
    QCOMPARE(name.toString(), QStringLiteral("foo"));
    top->setActualDataInformation(std::make_unique<DummyDataInformation>(nullptr));
    // val should now point to an invalid reference -> accessing name should throw an error
    name = Utils::property(val, "name");
    QVERIFY(name.isValid());
    QVERIFY(name.isError());
    QCOMPARE(name.toString(), invalidObjectError());
}

QTEST_GUILESS_MAIN(ScriptClassesTest)

#include "scriptclassestest.moc"
