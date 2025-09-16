/*
    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ARRAYSCRIPTCLASS_HPP
#define KASTEN_ARRAYSCRIPTCLASS_HPP

// lib
#include "defaultscriptclass.hpp"

class ArrayScriptClass : public DefaultScriptClass
{
public:
    ArrayScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~ArrayScriptClass() override;

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
    static QScriptValue Array_proto_toString(QScriptContext* ctx, QScriptEngine* eng);

private:
    QScriptString s_length;
    QScriptString s_childType; // deprecated
    QScriptString s_type;

    QScriptValue mArrayPrototype;
};

#endif // KASTEN_ARRAYSCRIPTCLASS_HPP
