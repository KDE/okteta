/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ARRAYSCRIPTCLASS_HPP
#define KASTEN_ARRAYSCRIPTCLASS_HPP

#include "defaultscriptclass.hpp"

class ArrayScriptClass : public DefaultScriptClass
{
    Q_DISABLE_COPY(ArrayScriptClass)

public:
    ArrayScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~ArrayScriptClass() override;
    QScriptValue prototype() const override;

protected:
    bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id) override;
    bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags) override;
    QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id) override;
    bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) override;

    static QScriptValue Array_proto_toString(QScriptContext* ctx, QScriptEngine* eng);

protected:
    QScriptString s_length;
    QScriptString s_childType; // deprecated
    QScriptString s_type;
    QScriptValue mArrayPrototype;
};

#endif // KASTEN_ARRAYSCRIPTCLASS_HPP
