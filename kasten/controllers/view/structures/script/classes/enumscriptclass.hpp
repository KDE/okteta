/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENUMSCRIPTCLASS_HPP
#define KASTEN_ENUMSCRIPTCLASS_HPP

#include "primitivescriptclass.hpp"

class EnumScriptClass : public PrimitiveScriptClass
{
    Q_DISABLE_COPY(EnumScriptClass)

public:
    EnumScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~EnumScriptClass() override;

protected:
    bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id) override;
    QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id) override;
    bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) override;

protected:
    QScriptString s_values;
};

#endif // KASTEN_ENUMSCRIPTCLASS_HPP
