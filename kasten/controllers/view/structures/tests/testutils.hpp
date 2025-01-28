/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *    SPDX-License-Identifier: LGPL-2.1-or-later
 */

#ifndef KASTEN_TESTUTILS_HPP
#define KASTEN_TESTUTILS_HPP

#include <QString>
#include <QTest>
#include <QScriptEngine>
#include "view/structures/datatypes/primitivedatatype.hpp"
#include "view/structures/datatypes/datainformation.hpp"
#include "view/structures/datatypes/primitive/primitivedatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/signedbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.hpp"
#include "view/structures/datatypes/primitive/bitfield/boolbitfielddatainformation.hpp"
#include "view/structures/datatypes/topleveldatainformation.hpp"
#include "view/structures/parsers/scriptvalueconverter.hpp"
#include "view/structures/script/scriptengineinitializer.hpp"

namespace Utils {

/** Converts a string to a binary number (spaces are ignored)
 * @param val the string representing a binary number
 * @return
 */
template <typename T>
T binary(const char* val)
{
    QString value = QString::fromUtf8(val);
    value = value.remove(QLatin1Char(' '));
    QTEST_ASSERT(unsigned(value.length()) <= sizeof(T) * 8); // otherwise we overflow
    bool ok = false;
    quint64 result = value.toULongLong(&ok, 2);
    QTEST_ASSERT(ok);
    return static_cast<T>(result);
}

DataInformation* evalAndParse(QScriptEngine* eng, const QString& code, ScriptLogger* logger)
{
    QScriptValue result = eng->evaluate(code);
    if (result.isError()) {
        qWarning() << "error parsing" << code << ":" << result.toString();
    }
    return ScriptValueConverter::convert(result, QStringLiteral("result"), logger);
}

DataInformation* evalAndParse(QScriptEngine* eng, const char* code, ScriptLogger* logger)
{
    return evalAndParse(eng, QString::fromUtf8(code), logger);
}

TopLevelDataInformation* evalAndParse(const QString& code)
{
    auto* l = new ScriptLogger();
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
    if (engine->hasUncaughtException()) {
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
    if (value.engine()->hasUncaughtException()) {
        ret = value.engine()->uncaughtException();
        value.engine()->clearExceptions();
    }
    return ret;
}

}

#endif /* KASTEN_TESTUTILS_HPP */
