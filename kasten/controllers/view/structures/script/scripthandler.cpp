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
#include "../datatypes/datainformation.h"
#include "../datatypes/topleveldatainformation.h"
#include "classes/defaultscriptclass.h"
#include "classes/arrayscriptclass.h"
#include "classes/primitivescriptclass.h"
#include "classes/enumscriptclass.h"
#include "classes/structunionscriptclass.h"
#include "classes/stringscriptclass.h"

#include <QtGui/QAction>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>

#include <KDebug>

ScriptHandler::ScriptHandler(QScriptEngine* engine, QString scriptFile, QString name) :
    mEngine(engine), mFile(scriptFile), mName(name)
#ifdef OKTETA_DEBUG_SCRIPT
, mDebugger(new QScriptEngineDebugger())
#endif
{
    init();
    mHandlerInfo.mArrayClass.reset(new ArrayScriptClass(mEngine, &mHandlerInfo));
    mHandlerInfo.mPrimitiveClass.reset(new PrimitiveScriptClass(mEngine, &mHandlerInfo));
    mHandlerInfo.mEnumClass.reset(new EnumScriptClass(mEngine, &mHandlerInfo));
    mHandlerInfo.mStructUnionClass.reset(new StructUnionScriptClass(mEngine, &mHandlerInfo));
    mHandlerInfo.mStringScriptClass.reset(new StringScriptClass(mEngine, &mHandlerInfo));
}

ScriptHandler::~ScriptHandler()
{
    delete mEngine;
#ifdef OKTETA_DEBUG_SCRIPT
    delete mDebugger;
#endif
}

bool ScriptHandler::init()
{
    ScriptEngineInitializer::addFuctionsToScriptEngine(*mEngine);

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
    mDebugger->attachTo(mEngine);
    mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
#endif
    mEngine->evaluate(contents, mFile);

    if (mEngine->hasUncaughtException())
    {
        ScriptUtils::object()->logScriptError(mEngine->uncaughtExceptionBacktrace());
        return false;
    }
    return true;
}

DataInformation* ScriptHandler::initialDataInformationFromScript()
{
    QScriptValue obj = mEngine->globalObject();
    QScriptValue initMethod = obj.property(QLatin1String("init"));

    ScriptValueConverter conv(initMethod, mName);
    DataInformation* ret = conv.convert();
    if (!ret)
        ScriptUtils::object()->logScriptError(QLatin1String("failed to parse object from file ") + mFile);
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
    if (additionalData && additionalData->validationFunction().isValid())
    {
        //value exists, we assume it has been checked to be a function
#ifdef OKTETA_DEBUG_SCRIPT
        mDebugger->attachTo(mEngine);
        mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
        kDebug()
        << "validating element: " << data->name();
#endif

//         QScriptValue thisObject = mEngine->newQObject(data,
//                 QScriptEngine::QtOwnership, QScriptEngine::ExcludeDeleteLater);
//         QScriptValue mainStruct = mEngine->newQObject(data->mainStructure(),
//                 QScriptEngine::QtOwnership, QScriptEngine::ExcludeDeleteLater);
        QScriptValue thisObject = data->toScriptValue(mEngine, &mHandlerInfo);
        QScriptValue mainStruct = data->mainStructure()->toScriptValue(mEngine, &mHandlerInfo);
        QScriptValueList args;
        args << mainStruct;
        QScriptValue result = additionalData->validationFunction().call(thisObject, args);
        if (result.isError())
        {
            ScriptUtils::object()->logScriptError(QLatin1String("error occurred while "
                "validating element ") + data->name(), result);
            data->setValidationError(QLatin1String("Error occurred in validation: ")
                    + result.toString());
        }
        if (mEngine->hasUncaughtException())
        {
            ScriptUtils::object()->logScriptError(
                    mEngine->uncaughtExceptionBacktrace());
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

//         QScriptValue thisObject = mEngine->newQObject(data,
//                 QScriptEngine::QtOwnership, QScriptEngine::ExcludeDeleteLater);
//         QScriptValue mainStruct = mEngine->newQObject(data->mainStructure(),
//                 QScriptEngine::QtOwnership, QScriptEngine::ExcludeDeleteLater);
        QScriptValue thisObject = data->toScriptValue(mEngine, &mHandlerInfo);
        QScriptValue mainStruct = data->mainStructure()->toScriptValue(mEngine, &mHandlerInfo);
        QScriptValueList args;
        args << mainStruct;
        QScriptValue result = additionalData->updateFunction().call(thisObject, args);
        if (result.isError())
        {
            ScriptUtils::object()->logScriptError(QLatin1String("error occurred while "
                "updating element ") + data->name(), result);
        }
        if (mEngine->hasUncaughtException())
        {
            ScriptUtils::object()->logScriptError(mEngine->uncaughtExceptionBacktrace());
            ScriptUtils::object()->logScriptError(QLatin1String("error occurred while "
                "updating element ") + data->name(), result);
        }
    }
}

QScriptEngine* ScriptHandler::engine()
{
    return mEngine;
}

ScriptHandlerInfo* ScriptHandler::handlerInfo()
{
    return &mHandlerInfo;
}
