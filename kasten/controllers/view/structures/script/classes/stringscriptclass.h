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

#ifndef STRINGSCRIPTCLASS_H
#define STRINGSCRIPTCLASS_H

#include "defaultscriptclass.h"

class StringScriptClass : public DefaultScriptClass
{
    Q_DISABLE_COPY(StringScriptClass)
public:
    StringScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    virtual ~StringScriptClass();
    virtual QScriptValue prototype() const;

protected:
    virtual bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id);
    virtual bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags);
    virtual QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id);
    virtual bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value);

    static QScriptValue String_proto_toString(QScriptContext* ctx, QScriptEngine* eng);
protected:
    QScriptString s_length;
    QScriptString s_lengthInBytes;
    QScriptString s_encoding;
    QScriptString s_terminatedBy;
    QScriptString s_maxCharCount;
    QScriptString s_maxByteCount;
    QScriptString s_termMode;
    QScriptValue mStringPrototype;
};

#endif // STRINGSCRIPTCLASS_H
