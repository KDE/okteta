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

#ifndef SCRIPTHANDLERINFO_H
#define SCRIPTHANDLERINFO_H

#include <QtGlobal>
#include <QScopedPointer>

class EnumScriptClass;
class StructUnionScriptClass;
class ArrayScriptClass;
class PrimitiveScriptClass;
class StringScriptClass;
class BitfieldScriptClass;
class PointerScriptClass;
class QScriptEngine;
class ScriptLogger;

class ScriptHandlerInfo {
public:
    explicit ScriptHandlerInfo(QScriptEngine* engine, ScriptLogger* logger);
    ~ScriptHandlerInfo();

    /** The type of function that is being evaluated (most writing is only allowed when updating) */
    enum Mode {
        None = 0, Validating = 1, Updating = 2, DeterminingLength = 4, TaggedUnionSelection = 8
    };

    QScopedPointer<ArrayScriptClass> mArrayClass;
    QScopedPointer<PrimitiveScriptClass> mPrimitiveClass;
    QScopedPointer<EnumScriptClass> mEnumClass;
    QScopedPointer<StructUnionScriptClass> mStructUnionClass;
    QScopedPointer<StringScriptClass> mStringClass;
    QScopedPointer<BitfieldScriptClass> mBitfieldClass;
    QScopedPointer<PointerScriptClass> mPointerClass;
    /** @return The mode this handler is currently in (determines which properties are accessible */
    inline Mode mode() const { return mMode; }
    inline void setMode(Mode m) { mMode = m; }
    inline ScriptLogger* logger() { return mLogger; }
private:
    ScriptLogger* mLogger;
    Mode mMode;
private:
    Q_DISABLE_COPY(ScriptHandlerInfo)
};

#endif // SCRIPTHANDLERINFO_H
