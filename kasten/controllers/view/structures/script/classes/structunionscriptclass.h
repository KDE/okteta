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


#ifndef STRUCTUNIONSCRIPTCLASS_H
#define STRUCTUNIONSCRIPTCLASS_H

#include "defaultscriptclass.h"

class StructUnionScriptClass : public DefaultScriptClass
{
    Q_DISABLE_COPY(StructUnionScriptClass)
public:
    StructUnionScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    virtual ~StructUnionScriptClass();
    virtual QScriptValue prototype() const;

protected:
    virtual bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id);
    virtual bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags);
    virtual QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id);
    virtual bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value);

    static QScriptValue StructUnion_proto_toString(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue StructUnion_proto_setChildren(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue StructUnion_proto_child(QScriptContext* ctx, QScriptEngine* eng);

protected:
    QScriptString s_childCount;
    QScriptString s_children;
    QScriptValue mStructUnionPrototype;
};

#endif // STRUCTUNIONSCRIPTCLASS_H
