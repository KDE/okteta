/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "scriptfileparser.hpp"

#include "scriptvalueconverter.hpp"
#include "parserutils.hpp"
#include "../datatypes/topleveldatainformation.hpp"
#include "../datatypes/dummydatainformation.hpp"
#include "../script/scriptengineinitializer.hpp"
#include "../script/scriptlogger.hpp"

#include <QScriptEngine>

ScriptFileParser::ScriptFileParser(const QString& pluginName, const QString& absolutePath)
    : AbstractStructureParser(pluginName, absolutePath)
{
}

ScriptFileParser::~ScriptFileParser() = default;

QStringList ScriptFileParser::parseStructureNames() const
{
    return {mPluginName};
}

QVector<TopLevelDataInformation*> ScriptFileParser::parseStructures() const
{
    QVector<TopLevelDataInformation*> ret;

    QScriptEngine* engine = ScriptEngineInitializer::newEngine();
    ScriptLogger* logger = new ScriptLogger();

    QScriptValue value = loadScriptValue(logger, engine);
    DataInformation* dataInf;
    if (!value.isValid()) {
        dataInf = new DummyDataInformation(nullptr, mPluginName);
    } else {
        dataInf = ScriptValueConverter::convert(value, mPluginName, logger);
    }

    if (!dataInf) {
        dataInf = new DummyDataInformation(nullptr, mPluginName);
    }
    const QFileInfo fileInfo(mAbsolutePath);
    TopLevelDataInformation* top = new TopLevelDataInformation(dataInf, logger, engine, fileInfo);
    // handle default lock offset
    QScriptValue lockOffset = value.property(ParserStrings::PROPERTY_DEFAULT_LOCK_OFFSET());
    if (lockOffset.isValid()) {
        ParsedNumber<quint64> offset = ParserUtils::uint64FromScriptValue(lockOffset);
        if (!offset.isValid) {
            dataInf->logError() << "Default lock offset is not a valid number:" << offset.string;
        } else {
            top->setDefaultLockOffset(offset.value);
        }
    }
    ret.append(top);
    return ret;
}

QScriptValue ScriptFileParser::loadScriptValue(ScriptLogger* logger, QScriptEngine* engine) const
{
    QFile scriptFile(mAbsolutePath);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        logger->error() << "Could not open file " << mAbsolutePath;
        return {};
    }

    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();
    engine->evaluate(contents, mAbsolutePath);
    if (engine->hasUncaughtException()) {
        // check if it was a syntax error:
        QScriptSyntaxCheckResult syntaxError = QScriptEngine::checkSyntax(contents);
        if (syntaxError.state() == QScriptSyntaxCheckResult::Error) {
            // give a detailed syntax error message
            logger->error() << "Syntax error in script: " << syntaxError.errorMessage();
            logger->error() << "Line number: " << syntaxError.errorLineNumber()
                            << "Column:" << syntaxError.errorColumnNumber();
        } else {
            // just print the generic exception message
            logger->error() << "Error evaluating script: " << engine->uncaughtException().toString();
            logger->error() << "Line number: " << engine->uncaughtExceptionLineNumber();
            logger->error() << "Backtrace: " << engine->uncaughtExceptionBacktrace();
        }
        return {};
    }
    QScriptValue obj = engine->globalObject();
    QScriptValue initMethod = obj.property(QStringLiteral("init"));
    if (!initMethod.isFunction()) {
        logger->error() << "Script has no 'init' function! Cannot evaluate script!";
        return {};
    }

    QScriptValue thisObj = engine->newObject();
    QScriptValueList args;
    QScriptValue result = initMethod.call(thisObj, args);
    if (result.isError()) {
        logger->error() << "Exception occurred while calling init():" << result.toString();
        return {};
    }
    return result;
}
