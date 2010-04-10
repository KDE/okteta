/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "scripthandler.h"
#include "scriptengineinitializer.h"
#include "scriptvalueconverter.h"
#include "scriptutils.h"
#include "../datatypes/topleveldatainformation.h"

#include <QtGui/QAction>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>

#include <KDebug>

ScriptHandler::ScriptHandler(QString scriptFile, QString name) :
    mFile(scriptFile), mName(name)
#ifdef OKTETA_DEBUG_SCRIPT
            , mDebugger(new QScriptEngineDebugger())
#endif
{
    init();
}

ScriptHandler::~ScriptHandler()
{
#ifdef OKTETA_DEBUG_SCRIPT
    delete mDebugger;
#endif

}

bool ScriptHandler::init()
{
    ScriptEngineInitializer::addFuctionsToScriptEngine(mEngine);

    QFile scriptFile(mFile);
    if (!scriptFile.open(QIODevice::ReadOnly))
    {
        kError() << "could not open file" << mFile << " -> cannot evaluate script";
        return false;
    }
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();
#ifdef OKTETA_DEBUG_SCRIPT
    mDebugger->attachTo(&mEngine);
    mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
#endif
    mEngine.evaluate(contents, mFile);

    if (mEngine.hasUncaughtException())
    {
        ScriptUtils::object()->logScriptError(mEngine.uncaughtExceptionBacktrace());
        return false;
    }
    return true;
}

DataInformation* ScriptHandler::initialDataInformationFromScript()
{
    QScriptValue obj = mEngine.globalObject();
    QScriptValue initMethod = obj.property("init");

    ScriptValueConverter conv(initMethod, mName);
    DataInformation* ret = conv.convert();
    if (!ret)
        ScriptUtils::object()->logScriptError("failed to parse object from file "
                + mFile);
    return ret;
}

void ScriptHandler::validateData(DataInformation* data)
{
    if (!data)
        return;

    data->setHasBeenValidated(false); //not yet validated

    if (data->hasChildren())
    {
        //first validate the children
        for (uint i = 0; i < data->childCount(); ++i)
        {
            validateData(data->childAt(i));
        }
    }

    //check if has a validation function:
    AdditionalData* additionalData = data->additionalData();
    if (additionalData && additionalData->validationFunction())
    {
        //value exists, we assume it has been checked to be a function
#ifdef OKTETA_DEBUG_SCRIPT
        mDebugger->attachTo(&mEngine);
        mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
        kDebug()
            << "validating element: " << data->name();
#endif

        QScriptValue thisObject = mEngine.newQObject(data,
                QScriptEngine::QtOwnership, QScriptEngine::ExcludeDeleteLater);
        QScriptValue mainStruct = mEngine.newQObject(
                data->topLevelDataInformation()->actualDataInformation(),
                QScriptEngine::QtOwnership, QScriptEngine::ExcludeDeleteLater);
        QScriptValueList args;
        args << mainStruct;
        QScriptValue result = additionalData->validationFunction()->call(thisObject,
                mainStruct);
        if (result.isError())
        {
            ScriptUtils::object()->logScriptError("error occurred while "
                "validating element " + data->name(), result);
            data->setValidationError("Error occurred in validation: "
                    + result.toString());
        }
        if (mEngine.hasUncaughtException())
        {
            ScriptUtils::object()->logScriptError(
                    mEngine.uncaughtExceptionBacktrace());
            data->setValidationError("Error occurred in validation: "
                    + result.toString());
        }
        if (result.isBool() || result.isBoolean())
        {
            data->validated(result.toBool());
        }
    }
    else
    {
        //if has children, this structure is valid if none of the children is invalid
        if (data->hasChildren())
        {
            bool hasValidatedChildren = false;
            bool allChildrenValid = true;
            for (uint i = 0; i < data->childCount(); ++i)
            {
                DataInformation* child = data->childAt(i);
                if (child->hasBeenValidated())
                {
                    hasValidatedChildren = true;
                    if (!child->validationSuccessful())
                    {
                        allChildrenValid = false;
                        break; //one is invalid -> whole structure is invalid
                    }
                }
            }
            if (hasValidatedChildren)
            {
                data->setHasBeenValidated(true);
                data->setValidationSuccessful(allChildrenValid);
            }
        }
    }

}

QScriptEngine* ScriptHandler::engine()
{
    return &mEngine;
}

