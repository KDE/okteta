/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_POINTERSCRIPTCLASS_HPP
#define KASTEN_POINTERSCRIPTCLASS_HPP

// lib
#include "primitivescriptclass.hpp"

class PointerScriptClass : public PrimitiveScriptClass
{
public:
    PointerScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~PointerScriptClass() override;

protected: // DefaultScriptClass API
    [[nodiscard]]
    bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id) override;
    [[nodiscard]]
    QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id) override;
    [[nodiscard]]
    bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) override;

private:
    QScriptString s_target;
    QScriptString s_type;
    QScriptString s_scale;
    QScriptString s_interpretFunc;
};

#endif // KASTEN_POINTERSCRIPTCLASS_HPP
