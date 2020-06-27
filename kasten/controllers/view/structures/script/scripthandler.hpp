/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTHANDLER_HPP
#define KASTEN_SCRIPTHANDLER_HPP

#include <QScopedPointer>

#include "scripthandlerinfo.hpp"

class DataInformation;
class ArrayDataInformation;
class TaggedUnionDataInformation;
class ScriptLogger;
class TopLevelDataInformation;
class QScriptEngineDebugger;
class QScriptValue;

class ScriptHandler
{
    Q_DISABLE_COPY(ScriptHandler)

public:
    ScriptHandler(QScriptEngine* engine, TopLevelDataInformation* topLevel);
    virtual ~ScriptHandler();

public:
    void validateData(DataInformation* data);
    /** The pointer may be changed while updating, CHECK AS SOON AS FUNCTION RETURNS! */
    void updateDataInformation(DataInformation* data);
    void updateLength(ArrayDataInformation* array);
    QString customToString(const DataInformation* data, const QScriptValue& func);

    QScriptEngine* engine() const;
    ScriptHandlerInfo* handlerInfo();

    QScriptValue callFunction(QScriptValue func, DataInformation* data, ScriptHandlerInfo::Mode mode);

private:
    QScopedPointer<QScriptEngine> mEngine;
    TopLevelDataInformation* mTopLevel;
    ScriptHandlerInfo mHandlerInfo;
};

#endif /* KASTEN_SCRIPTHANDLER_HPP */
