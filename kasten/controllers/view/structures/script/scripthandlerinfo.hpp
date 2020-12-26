/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTHANDLERINFO_HPP
#define KASTEN_SCRIPTHANDLERINFO_HPP

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

class ScriptHandlerInfo
{
public:
    explicit ScriptHandlerInfo(QScriptEngine* engine, ScriptLogger* logger);
    ~ScriptHandlerInfo();

    /** The type of function that is being evaluated (most writing is only allowed when updating) */
    enum class Mode
    {
        None = 0,
        Validating = 1,
        Updating = 2,
        DeterminingLength = 4,
        TaggedUnionSelection = 8,
        CustomToString = 16,
        InterpretingPointer = 32,
    };

    QScopedPointer<ArrayScriptClass> mArrayClass;
    QScopedPointer<PrimitiveScriptClass> mPrimitiveClass;
    QScopedPointer<EnumScriptClass> mEnumClass;
    QScopedPointer<StructUnionScriptClass> mStructUnionClass;
    QScopedPointer<StringScriptClass> mStringClass;
    QScopedPointer<BitfieldScriptClass> mBitfieldClass;
    QScopedPointer<PointerScriptClass> mPointerClass;

    /** @return The mode this handler is currently in (determines which properties are accessible */
    Mode mode() const;
    void setMode(Mode m);
    ScriptLogger* logger() const;

private:
    ScriptLogger* mLogger;
    Mode mMode = Mode::None;

    Q_DISABLE_COPY(ScriptHandlerInfo)
};

#endif // KASTEN_SCRIPTHANDLERINFO_HPP
