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

#include "scripthandler.h"
#include "scriptengineinitializer.h"
#include "scriptvalueconverter.h"
#include "scriptutils.h"
#include "scriptlogger.h"
#include "../datatypes/datainformation.h"
#include "../datatypes/topleveldatainformation.h"
#include "classes/defaultscriptclass.h"
#include "classes/arrayscriptclass.h"
#include "classes/primitivescriptclass.h"
#include "classes/enumscriptclass.h"
#include "classes/structunionscriptclass.h"
#include "classes/stringscriptclass.h"
#include "classes/bitfieldscriptclass.h"

#include <QtGui/QAction>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>

#include <KDebug>

ScriptHandler::ScriptHandler(QScriptEngine* engine, ScriptLogger* logger)
        : mEngine(engine), mLogger(logger), mHandlerInfo(engine)
#ifdef OKTETA_DEBUG_SCRIPT
, mDebugger(new QScriptEngineDebugger())
#endif
{
}

ScriptHandler::~ScriptHandler()
{
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
    if (additionalData && additionalData->validationFunction().isValid())
    {
        //value exists, we assume it has been checked to be a function
#ifdef OKTETA_DEBUG_SCRIPT
        mDebugger->attachTo(mEngine.data());
        mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
        kDebug()
        << "validating element: " << data->name();
#endif

        QScriptValue thisObject = data->toScriptValue(mEngine.data(), &mHandlerInfo);
        QScriptValue mainStruct = data->mainStructure()->toScriptValue(mEngine.data(),
                &mHandlerInfo);
        QScriptValueList args;
        args << mainStruct;
        QScriptValue result = additionalData->validationFunction().call(thisObject, args);
        if (result.isError())
        {
            mLogger->error(QLatin1String("error occurred while validating element ")
                    + data->fullObjectPath(), result);
            data->setValidationError(QLatin1String("Error occurred in validation: ")
                    + result.toString());
        }
        else if (mEngine->hasUncaughtException())
        {
            mLogger->error(QLatin1String("error occurred while validating element ")
                    + data->fullObjectPath(), mEngine->uncaughtExceptionBacktrace(), result);
            data->setValidationError(QLatin1String("Error occurred in validation: ")
                    + result.toString());
        }
        if (result.isBool() || result.isBoolean())
        {
            data->setValidationSuccessful(result.toBool());
        }
    }
}

void ScriptHandler::updateDataInformation(DataInformation* data)
{
    if (!data)
        return;

    //check if has a validation function:
    AdditionalData* additionalData = data->additionalData();
    if (additionalData && additionalData->updateFunction().isValid())
    {
        //value exists, we assume it has been checked to be a function
#ifdef OKTETA_DEBUG_SCRIPT
//         mDebugger->attachTo(mEngine);
//         mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
//         kDebug()
//         << "updating element: " << data->name();
#endif

        QScriptValue thisObject = data->toScriptValue(mEngine.data(), &mHandlerInfo);
        QScriptValue mainStruct = data->mainStructure()->toScriptValue(mEngine.data(),
                &mHandlerInfo);
        QScriptValueList args;
        args << mainStruct;
        QScriptValue result = additionalData->updateFunction().call(thisObject, args);
        if (result.isError())
        {
            mLogger->error(QLatin1String("error occurred while updating element ")
                    + data->fullObjectPath(), result);
        }
        if (mEngine->hasUncaughtException())
        {
            mLogger->error(QLatin1String("error occurred while updating element ")
                    + data->fullObjectPath(), mEngine->uncaughtExceptionBacktrace(), result);
        }
    }
}
