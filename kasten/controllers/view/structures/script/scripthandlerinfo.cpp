/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scripthandlerinfo.hpp"
#include "classes/arrayscriptclass.hpp"
#include "classes/primitivescriptclass.hpp"
#include "classes/structunionscriptclass.hpp"
#include "classes/stringscriptclass.hpp"
#include "classes/enumscriptclass.hpp"
#include "classes/bitfieldscriptclass.hpp"
#include "classes/pointerscriptclass.hpp"

ScriptHandlerInfo::ScriptHandlerInfo(QScriptEngine* engine, ScriptLogger* logger)
    : mArrayClass(new ArrayScriptClass(engine, this))
    , mPrimitiveClass(new PrimitiveScriptClass(engine, this))
    , mEnumClass(new EnumScriptClass(engine, this))
    , mStructUnionClass(new StructUnionScriptClass(engine, this))
    , mStringClass(new StringScriptClass(engine, this))
    , mBitfieldClass(new BitfieldScriptClass(engine, this))
    , mPointerClass(new PointerScriptClass(engine, this))
    , mLogger(logger)
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
