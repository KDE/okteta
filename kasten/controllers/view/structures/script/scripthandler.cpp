/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scripthandler.hpp"
#include "scriptengineinitializer.hpp"
#include "scriptutils.hpp"
#include "scriptlogger.hpp"
#include "classes/defaultscriptclass.hpp"
#include "../datatypes/datainformation.hpp"
#include "../datatypes/topleveldatainformation.hpp"
#include "../datatypes/array/arraydatainformation.hpp"
#include "../parsers/parserutils.hpp"

#include <QStringList>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QScriptEngine>

ScriptHandler::ScriptHandler(QScriptEngine* engine, TopLevelDataInformation* topLevel)
    : mEngine(engine)
    , mTopLevel(topLevel)
    , mHandlerInfo(engine, topLevel->logger())
{
}

ScriptHandler::~ScriptHandler() = default;

QScriptEngine* ScriptHandler::engine() const
{
    return mEngine.data();
}

ScriptHandlerInfo* ScriptHandler::handlerInfo()
{
    return &mHandlerInfo;
}

void ScriptHandler::validateData(DataInformation* data)
{
    Q_CHECK_PTR(data);

    if (data->hasBeenValidated()) {
        return;
    }
    // first validate the children
    for (uint i = 0; i < data->childCount(); ++i) {
        validateData(data->childAt(i));
    }

    // check if has a validation function:
    QScriptValue validationFunc = data->validationFunc();
    if (validationFunc.isValid()) {
        QScriptValue result = callFunction(validationFunc, data, ScriptHandlerInfo::Mode::Validating);
        if (result.isError()) {
            mTopLevel->logger()->error(data) << "Error occurred while validating element: "
                                             << result.toString();
            data->setValidationError(QStringLiteral("Error occurred in validation: ")
                                     + result.toString());
        } else if (mEngine->hasUncaughtException()) {
            mTopLevel->logger()->error(data) << "Error occurred while validating element:"
                                             << result.toString() << "\nBacktrace:" << mEngine->uncaughtExceptionBacktrace();
            data->setValidationError(QStringLiteral("Error occurred in validation: ")
                                     + result.toString());
            mEngine->clearExceptions();
        }
        if (result.isBool() || result.isBoolean()) {
            data->mValidationSuccessful = result.toBool();
        }
        if (result.isString()) {
            // error string
            QString str = result.toString();
            if (!str.isEmpty()) {
                data->setValidationError(str);
            }
        }
        data->mHasBeenValidated = true;
    }
}

void ScriptHandler::updateDataInformation(DataInformation* data)
{
    Q_CHECK_PTR(data);
    // check if has an update function:
    Q_ASSERT(!data->hasBeenUpdated());
    QScriptValue updateFunc = data->updateFunc();
    data->mHasBeenUpdated = true;
    if (updateFunc.isValid()) {
        QString context = data->fullObjectPath(); // we mustn't use data after updateFunc.call(), save context
        QScriptValue result = callFunction(updateFunc, data, ScriptHandlerInfo::Mode::Updating);
        if (result.isError()) {
            mTopLevel->logger()->error(context) << "Error occurred while updating element: "
                                                << result.toString();
        }
        if (mEngine->hasUncaughtException()) {
            mTopLevel->logger()->error(context) << "Error occurred while updating element:"
                                                << result.toString() << "\nBacktrace:" << mEngine->uncaughtExceptionBacktrace();
            mEngine->clearExceptions();
        }
    }
}

void ScriptHandler::updateLength(ArrayDataInformation* array)
{
    QScriptValue lengthFunc = array->lengthFunction();
    if (lengthFunc.isValid()) {
        Q_ASSERT(lengthFunc.isFunction());

        QScriptValue result = callFunction(lengthFunc, array, ScriptHandlerInfo::Mode::DeterminingLength);
        if (mEngine->hasUncaughtException()) {
            mTopLevel->logger()->error(array) << "Error occurred while calculating length:"
                                              << result.toString() << "\nBacktrace:" << mEngine->uncaughtExceptionBacktrace();
            mEngine->clearExceptions();
        }
        ParsedNumber<uint> value = ParserUtils::uintFromScriptValue(result);
        if (value.isValid) {
            array->setArrayLength(value.value);
        } else {
            array->logError() << "Length function did not return a valid number! Result was: " << result.toString();
        }
    }
}

QString ScriptHandler::customToString(const DataInformation* data, const QScriptValue& func)
{
    Q_ASSERT(func.isValid());
    Q_ASSERT(func.isFunction());
    Q_ASSERT(data->wasAbleToRead()); // this should never be called if EOF was reached
    // it is effectively const, since nothing may be modified while mode is CustomToString
    // const_cast is okay in this case
    QScriptValue result = callFunction(func, const_cast<DataInformation*>(data), ScriptHandlerInfo::Mode::CustomToString);
    if (result.isError()) {
        data->logError() << "toStringFunc caused an error:" << result.toString();
    }
    return result.toString();
}

QScriptValue ScriptHandler::callFunction(QScriptValue func, DataInformation* data,
                                         ScriptHandlerInfo::Mode mode)
{
    Q_ASSERT(func.isFunction());
    // value exists, we assume it has been checked to be a function
    QScriptValue thisObject = data->toScriptValue(mEngine.data(), &mHandlerInfo);
    QScriptValue mainStruct = data->mainStructure()->toScriptValue(mEngine.data(), &mHandlerInfo);
    const QScriptValueList args { mainStruct };
    // ensure we get the right properties
    mHandlerInfo.setMode(mode);
    QScriptValue result = func.call(thisObject, args);
    mHandlerInfo.setMode(ScriptHandlerInfo::Mode::None);
    return result;
}
