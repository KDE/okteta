/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENUMSCRIPTCLASS_HPP
#define KASTEN_ENUMSCRIPTCLASS_HPP

// lib
#include "primitivescriptclass.hpp"

class EnumScriptClass : public PrimitiveScriptClass
{
public:
    EnumScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~EnumScriptClass() override;

protected: // DefaultScriptClass API
    [[nodiscard]]
    bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id) override;
    [[nodiscard]]
    QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id) override;
    [[nodiscard]]
    bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) override;

private:
    const QScriptString s_values;
};

#endif // KASTEN_ENUMSCRIPTCLASS_HPP
