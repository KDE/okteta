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

#include "scriptfileparser.h"

#include "scriptvalueconverter.h"
#include "../datatypes/topleveldatainformation.h"
#include "../datatypes/dummydatainformation.h"
#include "../script/scriptengineinitializer.h"
#include "../script/scriptlogger.h"

#include <QScriptEngine>

ScriptFileParser::ScriptFileParser(const QString& pluginName, const QString& absolutePath)
        : AbstractStructureParser(pluginName, absolutePath)
{
}

ScriptFileParser::~ScriptFileParser()
{
}

QStringList ScriptFileParser::parseStructureNames() const
{
    return QStringList() << mPluginName;
}

QVector<TopLevelDataInformation*> ScriptFileParser::parseStructures() const
{
    QVector<TopLevelDataInformation*> ret;

    QScriptEngine* engine = ScriptEngineInitializer::newEngine();
    ScriptLogger* logger = new ScriptLogger();

    DataInformation* dataInf = convert(logger, engine);
    Q_CHECK_PTR(dataInf);
    const QFileInfo fileInfo(mAbsolutePath);
    ret.append(new TopLevelDataInformation(dataInf, logger, engine, fileInfo));
    return ret;
}

DataInformation* ScriptFileParser::convert(ScriptLogger* logger, QScriptEngine* engine) const
{
    QFile scriptFile(mAbsolutePath);
    if (!scriptFile.open(QIODevice::ReadOnly))
    {
        logger->error() << "Could not open file " << mAbsolutePath;
        return new DummyDataInformation(0, mPluginName);
    }

    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();
    engine->evaluate(contents, mAbsolutePath);

    QScriptValue obj = engine->globalObject();
    QScriptValue initMethod = obj.property(QLatin1String("init"));
    if (!initMethod.isFunction())
    {
        logger->error() << "Script has no 'init' function! Cannot evaluate script!";
        return new DummyDataInformation(0, mPluginName);
    }

    QScriptValue thisObj = engine->newObject();
    QScriptValueList args;
    QScriptValue result = initMethod.call(thisObj, args);
    if (result.isError())
    {
        logger->error() << "Exception occurred while calling init()";
        return new DummyDataInformation(0, mPluginName);
    }

    DataInformation* dataInf = ScriptValueConverter::convert(result, mPluginName, logger);
    if (!dataInf)
        dataInf = new DummyDataInformation(0, mPluginName);
    return dataInf;
}
