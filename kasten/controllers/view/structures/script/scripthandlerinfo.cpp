/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include "scripthandlerinfo.h"
#include "classes/arrayscriptclass.h"
#include "classes/primitivescriptclass.h"
#include "classes/structunionscriptclass.h"
#include "classes/stringscriptclass.h"
#include "classes/enumscriptclass.h"
#include "classes/bitfieldscriptclass.h"
#include "classes/pointerscriptclass.h"

ScriptHandlerInfo::ScriptHandlerInfo(QScriptEngine* engine, ScriptLogger* logger)
    : mArrayClass(new ArrayScriptClass(engine, this)),
      mPrimitiveClass(new PrimitiveScriptClass(engine, this)),
      mEnumClass(new EnumScriptClass(engine, this)),
      mStructUnionClass(new StructUnionScriptClass(engine, this)),
      mStringClass(new StringScriptClass(engine, this)),
      mBitfieldClass(new BitfieldScriptClass(engine, this)),
      mPointerClass(new PointerScriptClass(engine, this)),
      mLogger(logger), mMode(None)
{
    Q_CHECK_PTR(mLogger);
}

ScriptHandlerInfo::~ScriptHandlerInfo()
{
}

