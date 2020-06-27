/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGSCRIPTCLASS_HPP
#define KASTEN_STRINGSCRIPTCLASS_HPP

#include "defaultscriptclass.hpp"

class StringScriptClass : public DefaultScriptClass
{
    Q_DISABLE_COPY(StringScriptClass)

public:
    StringScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~StringScriptClass() override;
    QScriptValue prototype() const override;

protected:
    bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id) override;
    bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags) override;
    QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id) override;
    bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) override;

    static QScriptValue String_proto_toString(QScriptContext* ctx, QScriptEngine* eng);

protected:
    QScriptString s_lengthInCodepoints;
    QScriptString s_lengthInBytes;
    QScriptString s_encoding;
    QScriptString s_terminatedBy;
    QScriptString s_maxCharCount;
    QScriptString s_maxByteCount;
    QScriptValue mStringPrototype;
};

#endif // KASTEN_STRINGSCRIPTCLASS_HPP
