/*
    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scriptfileparser.hpp"

// lib
#include "scriptvalueconverter.hpp"
#include "parserutils.hpp"
#include <topleveldatainformation.hpp>
#include <dummydatainformation.hpp>
#include <scriptengineinitializer.hpp>
#include <scriptlogger.hpp>
// Qt
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

std::vector<std::unique_ptr<TopLevelDataInformation>> ScriptFileParser::parseStructures() const
{
    std::vector<std::unique_ptr<TopLevelDataInformation>> ret;

    std::unique_ptr<QScriptEngine> engine = ScriptEngineInitializer::newEngine();
    auto* logger = new ScriptLogger();

    QScriptValue value = loadScriptValue(logger, engine.get());
    std::unique_ptr<DataInformation> dataInf;
    if (!value.isValid()) {
        dataInf = std::make_unique<DummyDataInformation>(nullptr, mPluginName);
    } else {
        dataInf = std::unique_ptr<DataInformation>(ScriptValueConverter::convert(value, mPluginName, logger));
    }

    if (!dataInf) {
        dataInf = std::make_unique<DummyDataInformation>(nullptr, mPluginName);
    }
    const QFileInfo fileInfo(mAbsolutePath);
    auto& top = ret.emplace_back(std::make_unique<TopLevelDataInformation>(std::move(dataInf), logger, std::move(engine), fileInfo));
    // handle default lock offset
    QScriptValue lockOffset = value.property(ParserStrings::PROPERTY_DEFAULT_LOCK_OFFSET());
    if (lockOffset.isValid()) {
        ParsedNumber<quint64> offset = ParserUtils::uint64FromScriptValue(lockOffset);
        if (!offset.isValid) {
            top->actualDataInformation()->logError() << "Default lock offset is not a valid number:" << offset.string;
        } else {
            top->setDefaultLockOffset(offset.value);
        }
    }

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
    std::ignore = engine->evaluate(contents, mAbsolutePath);
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
