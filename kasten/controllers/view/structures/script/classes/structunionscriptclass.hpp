/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUNIONSCRIPTCLASS_HPP
#define KASTEN_STRUCTUNIONSCRIPTCLASS_HPP

// lib
#include "defaultscriptclass.hpp"

class StructUnionScriptClass : public DefaultScriptClass
{
public:
    StructUnionScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~StructUnionScriptClass() override;

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
    static QScriptValue StructUnion_proto_toString(QScriptContext* ctx, QScriptEngine* eng);
    [[nodiscard]]
    static QScriptValue StructUnion_proto_setChildren(QScriptContext* ctx, QScriptEngine* eng);
    [[nodiscard]]
    static QScriptValue StructUnion_proto_child(QScriptContext* ctx, QScriptEngine* eng);

private:
    const QScriptString s_childCount;
    const QScriptString s_children;

    QScriptValue mStructUnionPrototype;
};

#endif // KASTEN_STRUCTUNIONSCRIPTCLASS_HPP
