/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUNIONSCRIPTCLASS_HPP
#define KASTEN_STRUCTUNIONSCRIPTCLASS_HPP

#include "defaultscriptclass.hpp"

class StructUnionScriptClass : public DefaultScriptClass
{
    Q_DISABLE_COPY(StructUnionScriptClass)

public:
    StructUnionScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~StructUnionScriptClass() override;
    QScriptValue prototype() const override;

protected:
    bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id) override;
    bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags) override;
    QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id) override;
    bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) override;

    static QScriptValue StructUnion_proto_toString(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue StructUnion_proto_setChildren(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue StructUnion_proto_child(QScriptContext* ctx, QScriptEngine* eng);

protected:
    QScriptString s_childCount;
    QScriptString s_children;
    QScriptValue mStructUnionPrototype;
};

#endif // KASTEN_STRUCTUNIONSCRIPTCLASS_HPP
