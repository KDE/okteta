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
#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <QString>
#include <QtTest/QTest>
#include <QScriptEngine>
#include "view/structures/datatypes/primitivedatatype.h"
#include "view/structures/datatypes/datainformation.h"
#include "view/structures/datatypes/primitive/primitivedatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/signedbitfielddatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/boolbitfielddatainformation.h"
#include "view/structures/datatypes/topleveldatainformation.h"
#include "view/structures/parsers/scriptvalueconverter.h"
#include "view/structures/script/scriptengineinitializer.h"

namespace Utils
{

/** Converts a string to a binary number (spaces are ignored)
 * @param val the string representing a binary number
 * @return
 */
template<typename T>
T binary(const char* val)
{
    QString value = QLatin1String(val);
    value = value.remove(QLatin1Char(' '));
    QTEST_ASSERT(unsigned(value.length()) <= sizeof(T) * 8); //otherwise we overflow
    bool ok = false;
    quint64 result = value.toULongLong(&ok, 2);
    QTEST_ASSERT(ok);
    return static_cast<T>(result);
}

DataInformation* evalAndParse(QScriptEngine* eng, const QString& code, ScriptLogger* logger)
{
    QScriptValue result = eng->evaluate(code);
    if (result.isError())
        qWarning() << "error parsing" << code << ":" << result.toString();
    return ScriptValueConverter::convert(result, QLatin1String("result"), logger);
}

DataInformation* evalAndParse(QScriptEngine* eng, const char* code, ScriptLogger* logger)
{
    return evalAndParse(eng, QLatin1String(code), logger);
}

TopLevelDataInformation* evalAndParse(const QString& code)
{
    ScriptLogger* l = new ScriptLogger();
    l->setLogToStdOut(true);
    QScriptEngine* engine = ScriptEngineInitializer::newEngine();
    DataInformation* inf = evalAndParse(engine, code, l);
    QTEST_ASSERT(inf);
    return new TopLevelDataInformation(inf, l, engine);
}

TopLevelDataInformation* evalAndParse(const char* code)
{
    return evalAndParse(QLatin1String(code));
}

/** The same as engine->evaluate, but if there is an exception return that instead */
QScriptValue evaluate(QScriptEngine* engine, const QString& code)
{
    QScriptValue ret = engine->evaluate(code);
    if (engine->hasUncaughtException())
    {
        ret = engine->uncaughtException();
        engine->clearExceptions();
    }
    return ret;
}

QScriptValue evaluate(QScriptEngine* engine, const char* code)
{
    return evaluate(engine, QLatin1String(code));
}

/** The same as value.property(), but if there is an exception return that instead*/
QScriptValue property(const QScriptValue& value, const char* property)
{
    QScriptValue ret = value.property(QLatin1String(property));
    if (value.engine()->hasUncaughtException())
    {
        ret = value.engine()->uncaughtException();
        value.engine()->clearExceptions();
    }
    return ret;
}

struct DataInformationCheck
{
    virtual ~DataInformationCheck() {}
    virtual void check(DataInformation* data) = 0;
};

struct PrimitiveTypeCheck : public DataInformationCheck
{
    virtual ~PrimitiveTypeCheck() {}
    PrimitiveTypeCheck(PrimitiveDataType type) : mType(type) {};
    virtual void check(DataInformation* data)
    {
        QVERIFY(data->isPrimitive());
        QCOMPARE(data->asPrimitive()->type().value, mType.value);
    }
private:
    PrimitiveDataType mType;
};

struct SignedBitfieldCheck : public DataInformationCheck
{
    virtual ~SignedBitfieldCheck() {}
    virtual void check(DataInformation* data)
    {
        QVERIFY(data->isBitfield());
        QVERIFY(dynamic_cast<SignedBitfieldDataInformation*>(data));
    }
};

struct UnsignedBitfieldCheck : public DataInformationCheck
{
    virtual ~UnsignedBitfieldCheck() {}
    virtual void check(DataInformation* data)
    {
        QVERIFY(data->isBitfield());
        QVERIFY(dynamic_cast<UnsignedBitfieldDataInformation*>(data));
    }
};

struct BoolBitfieldCheck : public DataInformationCheck
{
    virtual ~BoolBitfieldCheck() {}
    virtual void check(DataInformation* data)
    {
        QVERIFY(data->isBitfield());
        QVERIFY(dynamic_cast<BoolBitfieldDataInformation*>(data));
    }
};

}
Q_DECLARE_METATYPE(Utils::DataInformationCheck*)

#endif /* TESTUTILS_H_ */
