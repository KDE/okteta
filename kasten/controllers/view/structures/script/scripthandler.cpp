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
#include "scriptutils.h"
#include "scriptlogger.h"
#include "../datatypes/datainformation.h"
#include "../datatypes/topleveldatainformation.h"
#include "../parsers/scriptvalueconverter.h"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QStringList>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>
#include <QtScript/QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>

#include <KDebug>

ScriptHandler::ScriptHandler(QScriptEngine* engine, TopLevelDataInformation* topLevel)
        : mEngine(engine), mTopLevel(topLevel), mHandlerInfo(engine)
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
    Q_CHECK_PTR(data);

    if (data->hasBeenValidated())
        return;
    data->setHasBeenValidated(false); //not yet validated

    //first validate the children
    for (uint i = 0; i < data->childCount(); ++i)
        validateData(data->childAt(i));

    //check if has a validation function:
    QScriptValue validationFunc = data->validationFunc();
    if (!validationFunc.isValid())
    {
        //value exists, we assume it has been checked to be a function
#ifdef OKTETA_DEBUG_SCRIPT
        mDebugger->attachTo(mEngine.data());
        mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
        kDebug() << "validating element: " << data->name();
#endif

        QScriptValue thisObject = data->toScriptValue(mEngine.data(), &mHandlerInfo);
        QScriptValue mainStruct = data->mainStructure()->toScriptValue(mEngine.data(),
                &mHandlerInfo);
        QScriptValueList args;
        args << mainStruct;
        mHandlerInfo.setMode(ScriptHandlerInfo::Validating);
        QScriptValue result = validationFunc.call(thisObject, args);
        if (result.isError())
        {
            mTopLevel->logger()->error(data) << "Error occurred while validating element: " << result.toString();
            data->setValidationError(QLatin1String("Error occurred in validation: ")
                    + result.toString());
        }
        else if (mEngine->hasUncaughtException())
        {
            mTopLevel->logger()->error(data) << "Error occurred while validating element:"
                    << result.toString() << "\nBacktrace:" << mEngine->uncaughtExceptionBacktrace();
            data->setValidationError(QLatin1String("Error occurred in validation: ")
                    + result.toString());
        }
        if (result.isBool() || result.isBoolean())
        {
            data->setValidationSuccessful(result.toBool());
        }
        if (result.isString())
        {
            //error string
            QString str = result.toString();
            if  (!str.isEmpty())
                data->setValidationError(str);
        }
        mHandlerInfo.setMode(ScriptHandlerInfo::None);
    }
}

void ScriptHandler::updateDataInformation(DataInformation* data)
{
    Q_CHECK_PTR(data);

    //check if has an update function:
    QScriptValue updateFunc = data->updateFunc();
    if (updateFunc.isValid())
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
        //ensure we get the right properties
        mHandlerInfo.setMode(ScriptHandlerInfo::Updating);
        QScriptValue result = updateFunc.call(thisObject, args);
        if (result.isError())
        {
            mTopLevel->logger()->error(data) << "Error occurred while updating element: " << result.toString();
        }
        if (mEngine->hasUncaughtException())
        {
            mTopLevel->logger()->error(data) << "Error occurred while updating element:"
                    << result.toString() << "\nBacktrace:" << mEngine->uncaughtExceptionBacktrace();
        }
        mHandlerInfo.setMode(ScriptHandlerInfo::None);
    }
}
