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
    QString value = QString::fromUtf8(val);
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
    return ScriptValueConverter::convert(result, QStringLiteral("result"), logger);
}

DataInformation* evalAndParse(QScriptEngine* eng, const char* code, ScriptLogger* logger)
{
    return evalAndParse(eng, QString::fromUtf8(code), logger);
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
    return evalAndParse(QString::fromUtf8(code));
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
    return evaluate(engine, QString::fromUtf8(code));
}

/** The same as value.property(), but if there is an exception return that instead*/
QScriptValue property(const QScriptValue& value, const char* property)
{
    QScriptValue ret = value.property(QString::fromUtf8(property));
    if (value.engine()->hasUncaughtException())
    {
        ret = value.engine()->uncaughtException();
        value.engine()->clearExceptions();
    }
    return ret;
}

}

#endif /* TESTUTILS_H_ */
