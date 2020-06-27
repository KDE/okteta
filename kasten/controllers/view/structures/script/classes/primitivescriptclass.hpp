/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRIMITIVESCRIPTCLASS_HPP
#define KASTEN_PRIMITIVESCRIPTCLASS_HPP

#include "defaultscriptclass.hpp"

class PrimitiveScriptClass : public DefaultScriptClass
{
    Q_DISABLE_COPY(PrimitiveScriptClass)

public:
    PrimitiveScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~PrimitiveScriptClass() override;
    QScriptValue prototype() const override;

protected:
    bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id) override;
    bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags) override;
    QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id) override;
    bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) override;

    static QScriptValue Primitive_proto_toString(QScriptContext* ctx, QScriptEngine* eng);

protected:
    QScriptString s_value;
    QScriptString s_type;
    QScriptString s_int;
    QScriptString s_int8;
    QScriptString s_int16;
    QScriptString s_int32;
    QScriptString s_int64;
    QScriptString s_int64high32;
    QScriptString s_int64low32;
    QScriptString s_uint;
    QScriptString s_uint8;
    QScriptString s_uint16;
    QScriptString s_uint32;
    QScriptString s_uint64;
    QScriptString s_uint64high32;
    QScriptString s_uint64low32;
    QScriptString s_bool;
    QScriptString s_float;
    QScriptString s_double;
    QScriptString s_char;
    // TODO Bitfields probably better in own class?

    QScriptValue mPrimitivePrototype;
};

#endif // KASTEN_PRIMITIVESCRIPTCLASS_HPP
