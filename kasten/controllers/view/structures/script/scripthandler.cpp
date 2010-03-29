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

#include <QtGui/QAction>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>

#include <KDebug>

ScriptHandler::ScriptHandler(QString scriptFile, QString name) :
    mFile(scriptFile), mName(name), mDebugger(new QScriptEngineDebugger())
{
    init();
}

ScriptHandler::~ScriptHandler()
{
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

    mDebugger->attachTo(&mEngine);
    mDebugger->action(QScriptEngineDebugger::InterruptAction)->trigger();

    mEngine.evaluate(contents, mFile);

    if (mEngine.hasUncaughtException())
    {
        kWarning() << mEngine.uncaughtExceptionBacktrace();
        return false;
    }
    return true;
}

DataInformation* ScriptHandler::initialDataInformationFromScript()
{
    QScriptValue obj = mEngine.globalObject();
    QScriptValue initMethod = obj.property("init");

    ScriptValueConverter conv(initMethod, mEngine, mName);
    DataInformation* ret = conv.convert();
    if (!ret)
        kWarning() << "failed to parse object from file " << mFile;
    return ret;
}

