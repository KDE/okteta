/*
 *  This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *  SPDX-FileCopyrightText: 2013 Alex Richardson <alex.richardson@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

// TODO: find better way to work-around simple name creation for QTest::newRow
#undef QT_USE_QSTRINGBUILDER

#include <QTest>
#include <QScriptEngine>
#include "view/structures/script/scriptengineinitializer.hpp"
#include "view/structures/parsers/scriptvalueconverter.hpp"
#include "testutils.hpp"
#include <functional>

struct JsTestData
{
    using CheckCallback = std::function<void(DataInformation*)>;
    JsTestData() = default;
    JsTestData(const char* tag, const char* constructor, const CheckCallback& check)
        : tag(tag)
        , constructorCall(QString::fromUtf8(constructor))
        , check(check)
    {}
    QByteArray tag;
    QString constructorCall;
    CheckCallback check;
};
Q_DECLARE_METATYPE(JsTestData)
Q_DECLARE_METATYPE(JsTestData::CheckCallback)

class JsParserTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testValidationFunc();
    void testValidationFunc_data();
    void testUpdateFunc();
    void testUpdateFunc_data();
    void testByteOrder();
    void testByteOrder_data();
    void testName();
    void testName_data();
    void testCustomTypeName();
    void testCustomTypeName_data();
    void testImport();
    void testImportPathTraversal();

private:
    /** data gets set to the parsed result.
     * This is needed since functions with QVERIFY/QCOMPARE must return void */
    void testCommon(DataInformation** data);

private:
    QScriptEngine engine;
    QVector<JsTestData> primitiveData;
    QVector<JsTestData> bitfieldData;
    QVector<JsTestData> allData;
};

static JsTestData::CheckCallback primitiveTypeCheck(PrimitiveDataType type)
{
    return [type](DataInformation* data) {
        QVERIFY(data->isPrimitive());
        QCOMPARE(data->asPrimitive()->type(), type);
    };
}

static JsTestData::CheckCallback bitfieldCheck(AbstractBitfieldDataInformation::Type type)
{
    return [type](DataInformation* data) {
        QVERIFY(data->isPrimitive());
        QCOMPARE(data->asBitfield()->bitfieldType(), type);
    };
}

void JsParserTest::initTestCase()
{
    ScriptEngineInitializer::addFuctionsToScriptEngine(&engine);

    primitiveData
        << JsTestData("float", "float()", primitiveTypeCheck(PrimitiveDataType::Float))
        << JsTestData("double", "double()", primitiveTypeCheck(PrimitiveDataType::Double))
        << JsTestData("char", "char()", primitiveTypeCheck(PrimitiveDataType::Char))

        << JsTestData("uint8", "uint8()", primitiveTypeCheck(PrimitiveDataType::UInt8))
        << JsTestData("uint16", "uint16()", primitiveTypeCheck(PrimitiveDataType::UInt16))
        << JsTestData("uint32", "uint32()", primitiveTypeCheck(PrimitiveDataType::UInt32))
        << JsTestData("uint64", "uint64()", primitiveTypeCheck(PrimitiveDataType::UInt64))

        << JsTestData("int8", "int8()", primitiveTypeCheck(PrimitiveDataType::Int8))
        << JsTestData("int16", "int16()", primitiveTypeCheck(PrimitiveDataType::Int16))
        << JsTestData("int32", "int32()", primitiveTypeCheck(PrimitiveDataType::Int32))
        << JsTestData("int64", "int64()", primitiveTypeCheck(PrimitiveDataType::Int64))

        << JsTestData("bool8", "bool8()", primitiveTypeCheck(PrimitiveDataType::Bool8))
        << JsTestData("bool16", "bool16()", primitiveTypeCheck(PrimitiveDataType::Bool16))
        << JsTestData("bool32", "bool32()", primitiveTypeCheck(PrimitiveDataType::Bool32))
        << JsTestData("bool64", "bool64()", primitiveTypeCheck(PrimitiveDataType::Bool64));

    bitfieldData
        << JsTestData("signed bitfield", "bitfield(\"signed\", 5)",
                      bitfieldCheck(AbstractBitfieldDataInformation::Type::Signed))
        << JsTestData("unsigned bitfield", "bitfield(\"unsigned\", 5)",
                  bitfieldCheck(AbstractBitfieldDataInformation::Type::Unsigned))
        << JsTestData("bool bitfield", "bitfield(\"bool\", 5)",
                  bitfieldCheck(AbstractBitfieldDataInformation::Type::Boolean));

    allData << primitiveData << bitfieldData;
    // TODO struct, union, taggedUnion, pointer, flags, enum, array, string

    // needed so that imports can be resolved
    QString resources = QFINDTESTDATA("resources");
    QString examples = QFINDTESTDATA("../view/structures/examples");
    QVERIFY2(!resources.isEmpty(), "Test data must exist!");
    QVERIFY2(!examples.isEmpty(), "Test data must exist!");
    qputenv("XDG_DATA_DIRS",
            QFile::encodeName(QFileInfo(resources).absoluteFilePath()) + ':' +
            QFile::encodeName(QFileInfo(examples).absoluteFilePath()));
}

void JsParserTest::testByteOrder_data()
{
    QTest::addColumn<QString>("code");
    QTest::addColumn<JsTestData::CheckCallback>("checkFunction");
    QTest::addColumn<int>("expectedByteOrder");
    // verify that default is inherit
    for (const JsTestData& data : qAsConst(allData)) {
        // default should be inherit
        QString codeStr = QStringLiteral("%1;");
        QTest::newRow(data.tag.constData()) << codeStr.arg(data.constructorCall)
                                            << data.check << (int)DataInformation::DataInformationEndianess::EndianessInherit;

        // use set() function to specify byteOrder
        codeStr = QStringLiteral("%1.set({byteOrder: \"inherit\"})");
        QTest::newRow((data.tag + " set() inherit").constData()) << codeStr.arg(data.constructorCall)
                                                                 << data.check << (int)DataInformation::DataInformationEndianess::EndianessInherit;
        codeStr = QStringLiteral("%1.set({byteOrder: \"littleEndian\"})");
        QTest::newRow((data.tag + " set() little endian").constData()) << codeStr.arg(data.constructorCall)
                                                                       << data.check << (int)DataInformation::DataInformationEndianess::EndianessLittle;
        codeStr = QStringLiteral("%1.set({byteOrder: \"bigEndian\"})");
        QTest::newRow((data.tag + " set() big endian").constData()) << codeStr.arg(data.constructorCall)
                                                                    << data.check << (int)DataInformation::DataInformationEndianess::EndianessBig;
        codeStr = QStringLiteral("%1.set({byteOrder: \"fromSettings\"})");
        QTest::newRow((data.tag + " set() from settings").constData()) << codeStr.arg(data.constructorCall)
                                                                       << data.check << (int)DataInformation::DataInformationEndianess::EndianessFromSettings;

        // direct property access to specify byteOrder
        codeStr = QStringLiteral("var obj = %1; obj.byteOrder = \"inherit\"; obj;");
        QTest::newRow((data.tag + " property assign inherit").constData()) << codeStr.arg(data.constructorCall)
                                                                           << data.check << (int)DataInformation::DataInformationEndianess::EndianessInherit;
        codeStr = QStringLiteral("var obj = %1; obj.byteOrder = \"little-endian\"; obj;");
        QTest::newRow((data.tag + " property assign little endian").constData()) << codeStr.arg(data.constructorCall)
                                                                                 << data.check << (int)DataInformation::DataInformationEndianess::EndianessLittle;
        codeStr = QStringLiteral("var obj = %1; obj.byteOrder = \"big-endian\"; obj;");
        QTest::newRow((data.tag + " property assign big endian").constData()) << codeStr.arg(data.constructorCall)
                                                                              << data.check << (int)DataInformation::DataInformationEndianess::EndianessBig;
        codeStr = QStringLiteral("var obj = %1; obj.byteOrder = \"from-settings\"; obj;");
        QTest::newRow((data.tag + " property assign from settings").constData()) << codeStr.arg(data.constructorCall)
                                                                                 << data.check << (int)DataInformation::DataInformationEndianess::EndianessFromSettings;
    }
}

void JsParserTest::testCommon(DataInformation** dataPtr)
{
    QFETCH(QString, code);
    QFETCH(JsTestData::CheckCallback, checkFunction);
    QScriptValue value = engine.evaluate(code);
    QVERIFY(value.isValid());
    QVERIFY(!value.isError());
    QVERIFY(value.isObject());
    ScriptLogger logger;
    QScopedPointer<DataInformation> data
        (ScriptValueConverter::convert(value, QStringLiteral("converted"), &logger));
    QVERIFY(logger.rowCount() == 0);
    QVERIFY(data);
    checkFunction(data.data());
    *dataPtr = data.take();
}

void JsParserTest::testByteOrder()
{
    DataInformation* data = nullptr;
    testCommon(&data);
    if (QTest::currentTestFailed()) {
        return; // Qt doesn't use exceptions, we must manually check after each call
    }
    QFETCH(int, expectedByteOrder);
    QCOMPARE((int)data->byteOrder(), expectedByteOrder);
}

namespace {
QString updateFunction() { return QStringLiteral("function () { /* do nothing*/; }"); }
}

void JsParserTest::testUpdateFunc_data()
{
    QTest::addColumn<QString>("code");
    QTest::addColumn<JsTestData::CheckCallback>("checkFunction");

    for (const JsTestData& data : qAsConst(allData)) {
        QString codeStr = QStringLiteral("%1.setUpdate(") + updateFunction() + QStringLiteral(");");
        QTest::newRow((data.tag + "-setUpdate()").constData())
            << codeStr.arg(data.constructorCall) << data.check;

        codeStr = QStringLiteral("%1.set({updateFunc: ") + updateFunction() + QStringLiteral("});");
        QTest::newRow((data.tag + "-set()").constData())
            << codeStr.arg(data.constructorCall) << data.check;

        codeStr = QStringLiteral("var obj = %1; obj.updateFunc = ") + updateFunction() + QStringLiteral("; obj;");
        QTest::newRow((data.tag + "-property assign").constData())
            << codeStr.arg(data.constructorCall) << data.check;
    }
}

void JsParserTest::testUpdateFunc()
{
    DataInformation* data = nullptr;
    testCommon(&data);
    if (QTest::currentTestFailed()) {
        return; // Qt doesn't use exceptions, we must manually check after each call
    }
    QVERIFY(data);
    QScriptValue update = data->updateFunc();
    QVERIFY(update.isValid());
    QVERIFY(update.isFunction());
    QCOMPARE(update.toString(), updateFunction());
}

namespace {
QString validationFunction() { return QStringLiteral("function () { return true; }"); }
}

void JsParserTest::testValidationFunc_data()
{
    QTest::addColumn<QString>("code");
    QTest::addColumn<JsTestData::CheckCallback>("checkFunction");

    for (const JsTestData& data : qAsConst(allData)) {
        QString codeStr = QStringLiteral("%1.setValidation(") + validationFunction() + QStringLiteral(");");
        QTest::newRow((data.tag + "-setUpdate()").constData())
            << codeStr.arg(data.constructorCall) << data.check;

        codeStr = QStringLiteral("%1.set({validationFunc: ") + validationFunction() + QStringLiteral("});");
        QTest::newRow((data.tag + "-set()").constData())
            << codeStr.arg(data.constructorCall) << data.check;

        codeStr = QStringLiteral("var obj = %1; obj.validationFunc = ") + validationFunction() + QStringLiteral("; obj;");
        QTest::newRow((data.tag + "-property assign").constData())
            << codeStr.arg(data.constructorCall) << data.check;
    }
}

void JsParserTest::testValidationFunc()
{
    DataInformation* data = nullptr;
    testCommon(&data);
    if (QTest::currentTestFailed()) {
        return; // Qt doesn't use exceptions, we must manually check after each call

    }
    QScriptValue validation = data->validationFunc();
    QVERIFY(validation.isValid());
    QVERIFY(validation.isFunction());
    QCOMPARE(validation.toString(), validationFunction());
}

void JsParserTest::testName_data()
{
    QTest::addColumn<QString>("code");
    QTest::addColumn<JsTestData::CheckCallback>("checkFunction");

    for (const JsTestData& data : qAsConst(allData)) {
        QString codeStr = QStringLiteral("%1.set({name: \"expectedName\"});");
        QTest::newRow((data.tag + "-set()").constData())
            << codeStr.arg(data.constructorCall) << data.check;

        codeStr = QStringLiteral("var obj = %1;obj.name = \"expectedName\"; obj;");
        QTest::newRow((data.tag + "-property assignment").constData())
            << codeStr.arg(data.constructorCall) << data.check;
    }
}

void JsParserTest::testName()
{
    DataInformation* data = nullptr;
    testCommon(&data);
    if (QTest::currentTestFailed()) {
        return; // Qt doesn't use exceptions, we must manually check after each call

    }
    QCOMPARE(data->name(), QStringLiteral("expectedName"));
}

void JsParserTest::testCustomTypeName_data()
{
    QTest::addColumn<QString>("code");
    QTest::addColumn<JsTestData::CheckCallback>("checkFunction");

    for (const JsTestData& data : qAsConst(allData)) {
        QString codeStr = QStringLiteral("%1.set({typeName: 'myCustomType'});");
        QTest::newRow((data.tag + "-set()").constData())
            << codeStr.arg(data.constructorCall) << data.check;

        codeStr = QStringLiteral("var obj = %1;obj.typeName = 'myCustomType'; obj;");
        QTest::newRow((data.tag + "-property assignment").constData())
            << codeStr.arg(data.constructorCall) << data.check;
    }
}

void JsParserTest::testCustomTypeName()
{
    DataInformation* data = nullptr;
    testCommon(&data);
    if (QTest::currentTestFailed()) {
        return; // Qt doesn't use exceptions, we must manually check after each call

    }
    QCOMPARE(data->typeName(), QStringLiteral("myCustomType"));
}

void JsParserTest::testImport()
{
    QScopedPointer<QScriptEngine> eng(ScriptEngineInitializer::newEngine());
#if defined(Q_OS_MACOS) || defined(Q_OS_WINDOWS)
    QEXPECT_FAIL("", "QStandardPaths::GenericDataLocation can't be modified on macOS/Windows", Continue);
#endif
    QScriptValue val = eng->evaluate(QStringLiteral("s = importScript('simpleImport.js');s.foo()"));
    QCOMPARE(val.toString(), QStringLiteral("100"));
}

void JsParserTest::testImportPathTraversal()
{
    QScopedPointer<QScriptEngine> eng(ScriptEngineInitializer::newEngine());
    QScriptValue val = eng->evaluate(QStringLiteral("s = importScript('../../pathtraversal.js');s.foo()"));
    QVERIFY(val.isError());
    QCOMPARE(val.toString(), QStringLiteral("Error: importScript(): You may only access installed structure files! Path traversal detected."));
}

QTEST_GUILESS_MAIN(JsParserTest)

#include "jsparsertest.moc"
