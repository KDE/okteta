/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KASTEN_TESTUTILS_HPP
#define KASTEN_TESTUTILS_HPP

// sut
#include <primitivedatatype.hpp>
#include <datainformation.hpp>
#include <primitivedatainformation.hpp>
#include <signedbitfielddatainformation.hpp>
#include <unsignedbitfielddatainformation.hpp>
#include <boolbitfielddatainformation.hpp>
#include <topleveldatainformation.hpp>
#include <scriptvalueconverter.hpp>
#include <scriptengineinitializer.hpp>
// Qt
#include <QString>
#include <QTest>
#include <QScriptEngine>

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

std::unique_ptr<DataInformation> evalAndParse(QScriptEngine* eng, const QString& code, ScriptLogger* logger)
{
    QScriptValue result = eng->evaluate(code);
    if (result.isError()) {
        qWarning() << "error parsing" << code << ":" << result.toString();
    }
    return ScriptValueConverter::convert(result, QStringLiteral("result"), logger);
}

std::unique_ptr<DataInformation> evalAndParse(QScriptEngine* eng, const char* code, ScriptLogger* logger)
{
    return evalAndParse(eng, QString::fromUtf8(code), logger);
}

std::unique_ptr<TopLevelDataInformation> evalAndParse(const QString& code)
{
    auto l = std::make_unique<ScriptLogger>();
    l->setLogToStdOut(true);
    std::unique_ptr<QScriptEngine> engine = ScriptEngineInitializer::newEngine();
    std::unique_ptr<DataInformation> inf = evalAndParse(engine.get(), code, l.get());
    QTEST_ASSERT(inf);
    return std::make_unique<TopLevelDataInformation>(std::move(inf), std::move(l), std::move(engine));
}

std::unique_ptr<TopLevelDataInformation> evalAndParse(const char* code)
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
