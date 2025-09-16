/*
    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTHANDLERINFO_HPP
#define KASTEN_SCRIPTHANDLERINFO_HPP

// Std
#include <memory>

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
    ScriptHandlerInfo(const ScriptHandlerInfo&) = delete;
    ScriptHandlerInfo(ScriptHandlerInfo&&) = delete;

    ~ScriptHandlerInfo();

public:
    ScriptHandlerInfo& operator=(const ScriptHandlerInfo&) = delete;
    ScriptHandlerInfo& operator=(ScriptHandlerInfo&&) = delete;

public:
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

    [[nodiscard]]
    ArrayScriptClass* arrayClass() const;
    [[nodiscard]]
    PrimitiveScriptClass* primitiveClass() const;
    [[nodiscard]]
    EnumScriptClass* enumClass() const;
    [[nodiscard]]
    StructUnionScriptClass* structUnionClass() const;
    [[nodiscard]]
    StringScriptClass* stringClass() const;
    [[nodiscard]]
    BitfieldScriptClass* bitfieldClass() const;
    [[nodiscard]]
    PointerScriptClass* pointerClass() const;

    /** @return The mode this handler is currently in (determines which properties are accessible */
    [[nodiscard]]
    Mode mode() const;
    void setMode(Mode m);
    [[nodiscard]]
    ScriptLogger* logger() const;

private:
    ScriptLogger* const mLogger;
    Mode mMode = Mode::None;

    const std::unique_ptr<ArrayScriptClass> mArrayClass;
    const std::unique_ptr<PrimitiveScriptClass> mPrimitiveClass;
    const std::unique_ptr<EnumScriptClass> mEnumClass;
    const std::unique_ptr<StructUnionScriptClass> mStructUnionClass;
    const std::unique_ptr<StringScriptClass> mStringClass;
    const std::unique_ptr<BitfieldScriptClass> mBitfieldClass;
    const std::unique_ptr<PointerScriptClass> mPointerClass;
};

inline
ArrayScriptClass* ScriptHandlerInfo::arrayClass() const
{
    return mArrayClass.get();
}

inline
PrimitiveScriptClass* ScriptHandlerInfo::primitiveClass() const
{
    return mPrimitiveClass.get();
}

inline
EnumScriptClass* ScriptHandlerInfo::enumClass() const
{
    return mEnumClass.get();
}

inline
StructUnionScriptClass* ScriptHandlerInfo::structUnionClass() const
{
    return mStructUnionClass.get();
}

inline
StringScriptClass* ScriptHandlerInfo::stringClass() const
{
    return mStringClass.get();
}

inline
BitfieldScriptClass* ScriptHandlerInfo::bitfieldClass() const
{
    return mBitfieldClass.get();
}

inline
PointerScriptClass* ScriptHandlerInfo::pointerClass() const
{
    return mPointerClass.get();
}

#endif // KASTEN_SCRIPTHANDLERINFO_HPP
