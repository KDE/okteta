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
    , mArrayClass(std::make_unique<ArrayScriptClass>(engine, this))
    , mPrimitiveClass(std::make_unique<PrimitiveScriptClass>(engine, this))
    , mEnumClass(std::make_unique<EnumScriptClass>(engine, this))
    , mStructUnionClass(std::make_unique<StructUnionScriptClass>(engine, this))
    , mStringClass(std::make_unique<StringScriptClass>(engine, this))
    , mBitfieldClass(std::make_unique<BitfieldScriptClass>(engine, this))
    , mPointerClass(std::make_unique<PointerScriptClass>(engine, this))
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
