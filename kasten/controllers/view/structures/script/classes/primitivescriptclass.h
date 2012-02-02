/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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


#ifndef PRIMITIVESCRIPTCLASS_H
#define PRIMITIVESCRIPTCLASS_H

#include "defaultscriptclass.h"

class PrimitiveScriptClass : public DefaultScriptClass
{
    Q_DISABLE_COPY(PrimitiveScriptClass)
public:
    PrimitiveScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    virtual ~PrimitiveScriptClass();
    virtual QScriptValue prototype() const;

protected:
    virtual bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id);
    virtual bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags);
    virtual QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id);
    virtual bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value);

    static QScriptValue Primitive_proto_toString(QScriptContext* ctx, QScriptEngine* eng);
protected:
    QScriptString s_value;
    QScriptString s_type;
    QScriptString s_int;
    QScriptString s_int8;
    QScriptString s_int16;
    QScriptString s_int32;
    QScriptString s_int64;
    QScriptString s_int64high32;
    QScriptString s_int64low32;
    QScriptString s_uint;
    QScriptString s_uint8;
    QScriptString s_uint16;
    QScriptString s_uint32;
    QScriptString s_uint64;
    QScriptString s_uint64high32;
    QScriptString s_uint64low32;
    QScriptString s_bool;
    QScriptString s_float;
    QScriptString s_double;
    QScriptString s_char;
    //TODO Bitfields probably better in own class?

    QScriptValue mPrimitivePrototype;
};

#endif // PRIMITIVESCRIPTCLASS_H
