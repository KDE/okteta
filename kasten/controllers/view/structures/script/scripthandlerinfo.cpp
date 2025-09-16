/*
    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scripthandlerinfo.hpp"

// lib
#include <arrayscriptclass.hpp>
#include <primitivescriptclass.hpp>
#include <structunionscriptclass.hpp>
#include <stringscriptclass.hpp>
#include <enumscriptclass.hpp>
#include <bitfieldscriptclass.hpp>
#include <pointerscriptclass.hpp>

ScriptHandlerInfo::ScriptHandlerInfo(QScriptEngine* engine, ScriptLogger* logger)
    : mLogger(logger)
    , mArrayClass(new ArrayScriptClass(engine, this))
    , mPrimitiveClass(new PrimitiveScriptClass(engine, this))
    , mEnumClass(new EnumScriptClass(engine, this))
    , mStructUnionClass(new StructUnionScriptClass(engine, this))
    , mStringClass(new StringScriptClass(engine, this))
    , mBitfieldClass(new BitfieldScriptClass(engine, this))
    , mPointerClass(new PointerScriptClass(engine, this))
{
    Q_CHECK_PTR(mLogger);
}

ScriptHandlerInfo::~ScriptHandlerInfo() = default;

ScriptHandlerInfo::Mode ScriptHandlerInfo::mode() const
{
    return mMode;
}

void ScriptHandlerInfo::setMode(Mode m)
{
    mMode = m;
}

ScriptLogger* ScriptHandlerInfo::logger() const
{
    return mLogger;
}
