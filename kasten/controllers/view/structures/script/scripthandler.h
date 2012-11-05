/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include <QScriptValue>
#include <QScopedPointer>

#include <config-structtool.h>
#include "scripthandlerinfo.h"

class DataInformation;
class ArrayDataInformation;
class TaggedUnionDataInformation;
class ScriptLogger;
class TopLevelDataInformation;
class QScriptEngineDebugger;

class ScriptHandler
{
    Q_DISABLE_COPY(ScriptHandler)

public:
    ScriptHandler(QScriptEngine* engine, TopLevelDataInformation* topLevel);
    virtual ~ScriptHandler();

    void validateData(DataInformation* data);
    /** The pointer may be changed while updating, CHECK AS SOON AS FUNCTION RETURNS! */
    void updateDataInformation(DataInformation* data);
    void updateLength(ArrayDataInformation* array);

    QScriptEngine* engine() const;
    ScriptHandlerInfo* handlerInfo();

    QScriptValue callFunction(QScriptValue func, DataInformation* data, ScriptHandlerInfo::Mode mode);
private:
    QScopedPointer<QScriptEngine> mEngine;
    TopLevelDataInformation* mTopLevel;
#ifdef OKTETA_DEBUG_SCRIPT
    QScopedPointer<QScriptEngineDebugger> mDebugger;
#endif
    ScriptHandlerInfo mHandlerInfo;
};

inline QScriptEngine* ScriptHandler::engine() const
{
    return mEngine.data();
}

inline ScriptHandlerInfo* ScriptHandler::handlerInfo()
{
    return &mHandlerInfo;
}

#endif /* SCRIPTHANDLER_H_ */
