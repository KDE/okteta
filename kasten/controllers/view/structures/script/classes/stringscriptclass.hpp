/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGSCRIPTCLASS_HPP
#define KASTEN_STRINGSCRIPTCLASS_HPP

// lib
#include "defaultscriptclass.hpp"

class StringScriptClass : public DefaultScriptClass
{
public:
    StringScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~StringScriptClass() override;

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
    static QScriptValue String_proto_toString(QScriptContext* ctx, QScriptEngine* eng);

private:
    QScriptString s_lengthInCodepoints;
    QScriptString s_lengthInBytes;
    QScriptString s_encoding;
    QScriptString s_terminatedBy;
    QScriptString s_maxCharCount;
    QScriptString s_maxByteCount;

    QScriptValue mStringPrototype;
};

#endif // KASTEN_STRINGSCRIPTCLASS_HPP
