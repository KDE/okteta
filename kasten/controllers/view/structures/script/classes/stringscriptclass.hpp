/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
