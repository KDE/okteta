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

#include "../structuredefinitionfile.h"
#include "../datatypes/topleveldatainformation.h"
#include "../datatypes/dummydatainformation.h"
#include "../script/scriptengineinitializer.h"
#include "../script/scriptlogger.h"
#include "../script/scriptvalueconverter.h"

#include <QScriptEngine>


ScriptFileParser::ScriptFileParser(const QString& pluginName, const QString& absolutePath)
        : AbstractStructureParser(pluginName, absolutePath)
{
}

ScriptFileParser::~ScriptFileParser()
{
}

QStringList ScriptFileParser::parseStructureNames()
{
    return QStringList() << mPluginName;
}

QVector<TopLevelDataInformation*> ScriptFileParser::parseStructures()
{
    QVector<TopLevelDataInformation*> ret;

    QScriptEngine* engine = ScriptEngineInitializer::newEngine();
    ScriptLogger* logger = new ScriptLogger();
    const QFileInfo fileInfo(mAbsolutePath);
    const QString objName = mPluginName;
    QFile scriptFile(fileInfo.absoluteFilePath());
    if (!scriptFile.open(QIODevice::ReadOnly))
    {
        logger->error(QLatin1String("Could not open file ") + fileInfo.absoluteFilePath());
        ret.append(new TopLevelDataInformation(new DummyDataInformation(0, objName), engine, logger,
                fileInfo));
        return ret;
    }
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();
    engine->evaluate(contents, fileInfo.absoluteFilePath());

    QScriptValue obj = engine->globalObject();
    QScriptValue initMethod = obj.property(QLatin1String("init"));
    if (!initMethod.isFunction())
    {
        logger->error(QLatin1String("Script has no init function! Cannot evaluate script!"));
        ret.append(new TopLevelDataInformation(new DummyDataInformation(0, objName), engine, logger,
                fileInfo));
        return ret;
    }
    QScriptValue thisObj = engine->newObject();
    QScriptValueList args;
    QScriptValue result = initMethod.call(thisObj, args);
    if (result.isError())
    {
        //return invalid
        logger->error(QLatin1String("Error occurred while calling init()"), result);
        ret.append(new TopLevelDataInformation(new DummyDataInformation(0, objName), engine, logger,
                fileInfo));
        return ret;
    }

    DataInformation* dataInf = ScriptValueConverter::convert(result, objName, logger);

    if (dataInf)
        ret.append(new TopLevelDataInformation(dataInf, engine, logger, fileInfo));
    else
        ret.append(new TopLevelDataInformation(new DummyDataInformation(0, objName), engine, logger,
                fileInfo));

    return ret;
}
