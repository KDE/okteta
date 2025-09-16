/*
    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRIMITIVESCRIPTCLASS_HPP
#define KASTEN_PRIMITIVESCRIPTCLASS_HPP

// lib
#include "defaultscriptclass.hpp"

class PrimitiveScriptClass : public DefaultScriptClass
{
public:
    PrimitiveScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~PrimitiveScriptClass() override;

public: // QScriptClass API
    [[nodiscard]]
    QScriptValue prototype() const override;

protected: // DefaultScriptClass API
    [[nodiscard]]
    bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id) override;
    [[nodiscard]]
    bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags) override;
    [[nodiscard]]
    QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id) override;
    [[nodiscard]]
    bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) override;

private:
    [[nodiscard]]
    static QScriptValue Primitive_proto_toString(QScriptContext* ctx, QScriptEngine* eng);

protected:
    const QScriptString s_value;
    const QScriptString s_type;
    const QScriptString s_bool;
    const QScriptString s_char;
    const QScriptString s_double;
    const QScriptString s_float;
    const QScriptString s_int;
    const QScriptString s_int8;
    const QScriptString s_int16;
    const QScriptString s_int32;
    const QScriptString s_int64low32;
    const QScriptString s_int64high32;
    const QScriptString s_int64;
    const QScriptString s_uint;
    const QScriptString s_uint8;
    const QScriptString s_uint16;
    const QScriptString s_uint32;
    const QScriptString s_uint64low32;
    const QScriptString s_uint64high32;
    const QScriptString s_uint64;
    // TODO Bitfields probably better in own class?

private:
    QScriptValue mPrimitivePrototype;
};

#endif // KASTEN_PRIMITIVESCRIPTCLASS_HPP
