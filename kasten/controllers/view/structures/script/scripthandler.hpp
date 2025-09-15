/*
    SPDX-FileCopyrightText: 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTHANDLER_HPP
#define KASTEN_SCRIPTHANDLER_HPP

#include "scripthandlerinfo.hpp"
// Std
#include <memory>

class DataInformation;
class ArrayDataInformation;
class TaggedUnionDataInformation;
class ScriptLogger;
class TopLevelDataInformation;
class QScriptValue;
class QString;

class ScriptHandler
{
public:
    ScriptHandler(QScriptEngine* engine, TopLevelDataInformation* topLevel);
    ScriptHandler(const ScriptHandler&) = delete;

    virtual ~ScriptHandler();

    ScriptHandler& operator=(const ScriptHandler&) = delete;

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
    std::unique_ptr<QScriptEngine> mEngine;
    TopLevelDataInformation* mTopLevel;
    ScriptHandlerInfo mHandlerInfo;
};

#endif /* KASTEN_SCRIPTHANDLER_HPP */
