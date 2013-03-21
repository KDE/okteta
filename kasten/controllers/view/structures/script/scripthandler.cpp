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
#include "classes/defaultscriptclass.h"
#include "../datatypes/datainformation.h"
#include "../datatypes/topleveldatainformation.h"
#include "../datatypes/array/arraydatainformation.h"
#include "../parsers/parserutils.h"

#include <QStringList>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QScriptEngine>
#include <QScriptEngineDebugger>

ScriptHandler::ScriptHandler(QScriptEngine* engine, TopLevelDataInformation* topLevel)
        : mEngine(engine), mTopLevel(topLevel), mHandlerInfo(engine, topLevel->logger())
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
    //first validate the children
    for (uint i = 0; i < data->childCount(); ++i)
        validateData(data->childAt(i));

    //check if has a validation function:
    QScriptValue validationFunc = data->validationFunc();
    if (!validationFunc.isValid())
    {
#ifdef OKTETA_DEBUG_SCRIPT
        mDebugger->attachTo(mEngine.data());
        mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
        kDebug() << "validating element: " << data->name();
#endif
        QScriptValue result = callFunction(validationFunc, data, ScriptHandlerInfo::Validating);
        if (result.isError())
        {
            mTopLevel->logger()->error(data) << "Error occurred while validating element: "
                    << result.toString();
            data->setValidationError(QLatin1String("Error occurred in validation: ")
                    + result.toString());
        }
        else if (mEngine->hasUncaughtException())
        {
            mTopLevel->logger()->error(data) << "Error occurred while validating element:"
                    << result.toString() << "\nBacktrace:" << mEngine->uncaughtExceptionBacktrace();
            data->setValidationError(QLatin1String("Error occurred in validation: ")
                    + result.toString());
            mEngine->clearExceptions();
        }
        if (result.isBool() || result.isBoolean())
        {
            data->mValidationSuccessful = result.toBool();
        }
        if (result.isString())
        {
            //error string
            QString str = result.toString();
            if (!str.isEmpty())
                data->setValidationError(str);
        }
    }
    data->mHasBeenValidated = true;
}

void ScriptHandler::updateDataInformation(DataInformation* data)
{
    Q_CHECK_PTR(data);
    //check if has an update function:
    Q_ASSERT(!data->hasBeenUpdated());
    QScriptValue updateFunc = data->updateFunc();
    data->mHasBeenUpdated = true;
    if (updateFunc.isValid())
    {
        QString context = data->fullObjectPath(); //we mustn't use data after updateFunc.call(), save context
        QScriptValue result = callFunction(updateFunc, data, ScriptHandlerInfo::Updating);
        if (result.isError())
        {
            mTopLevel->logger()->error(context) << "Error occurred while updating element: "
                    << result.toString();
        }
        if (mEngine->hasUncaughtException())
        {
            mTopLevel->logger()->error(context) << "Error occurred while updating element:"
                    << result.toString() << "\nBacktrace:" << mEngine->uncaughtExceptionBacktrace();
            mEngine->clearExceptions();
        }
    }
}

void ScriptHandler::updateLength(ArrayDataInformation* array)
{
    QScriptValue lengthFunc = array->lengthFunction();
    if (lengthFunc.isValid())
    {
        Q_ASSERT(lengthFunc.isFunction());

        QScriptValue result = callFunction(lengthFunc, array, ScriptHandlerInfo::DeterminingLength);
        if (mEngine->hasUncaughtException())
        {
            mTopLevel->logger()->error(array) << "Error occurred while calculating length:"
                    << result.toString() << "\nBacktrace:" << mEngine->uncaughtExceptionBacktrace();
            mEngine->clearExceptions();
        }
        ParsedNumber<uint> value = ParserUtils::uintFromScriptValue(result);
        if (value.isValid)
            array->setArrayLength(value.value);
        else
            array->logError() << "Length function did not return a valid number! Result was: " << result.toString();
    }
}

QScriptValue ScriptHandler::callFunction(QScriptValue func, DataInformation* data,
        ScriptHandlerInfo::Mode mode)
{
    Q_ASSERT(func.isFunction());
    //value exists, we assume it has been checked to be a function
    QScriptValue thisObject = data->toScriptValue(mEngine.data(), &mHandlerInfo);
    QScriptValue mainStruct = data->mainStructure()->toScriptValue(mEngine.data(), &mHandlerInfo);
    QScriptValueList args;
    args << mainStruct;
    //ensure we get the right properties
    mHandlerInfo.setMode(mode);
    QScriptValue result = func.call(thisObject, args);
    mHandlerInfo.setMode(ScriptHandlerInfo::None);
    return result;
}

