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


#include "structunionscriptclass.h"
#include "../../datatypes/datainformationwithchildren.h"

StructUnionScriptClass::StructUnionScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(engine, handlerInfo)
{
    s_childCount = engine->toStringHandle(QLatin1String("childCount"));
    s_children = engine->toStringHandle(QLatin1String("children"));

    mStructUnionPrototype = engine->newObject();
    mStructUnionPrototype.setProperty("toString", engine->newFunction(StructUnion_proto_toString));
    mStructUnionPrototype.setProperty("setChildren", engine->newFunction(StructUnion_proto_setChildren));
    mStructUnionPrototype.setProperty("child", engine->newFunction(StructUnion_proto_child));
}

StructUnionScriptClass::~StructUnionScriptClass()
{
}

bool StructUnionScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint*)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (name == s_childCount)
    {
        *flags &= ~HandlesWriteAccess;
        return true;
    }
    else if (name == s_children)
    {
        *flags &= ~HandlesReadAccess;
        return true;
    }
    return false;
}

bool StructUnionScriptClass::additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint, QScriptValue::PropertyFlags* flags)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (name == s_childCount)
    {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    else if (name == s_children)
    {
        //there is no write-only property
        return true;
    }
    return false;
}


QScriptValue StructUnionScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint)
{
    const DataInformationWithChildren* dataW = static_cast<const DataInformationWithChildren*>(data);
    //do a dynamic cast in debug mode to ensure the static cast was valid
    Q_CHECK_PTR(dynamic_cast<const DataInformationWithChildren*>(dataW));

    if (name == s_childCount)
        return dataW->childCount();
    else if (name == s_children)
        return engine()->undefinedValue();
    return QScriptValue();
}

bool StructUnionScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    DataInformationWithChildren* dataW = static_cast<DataInformationWithChildren*>(data);
    //do a dynamic cast in debug mode to ensure the static cast was valid
    Q_CHECK_PTR(dynamic_cast<DataInformationWithChildren*>(dataW));

    if (name == s_children)
    {
        dataW->setChildren(value);
        return true;
    }
    return false;
}

QScriptValue StructUnionScriptClass::prototype() const
{
    return mStructUnionPrototype;
}

QScriptValue StructUnionScriptClass::StructUnion_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(ctx->thisObject().data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    return data->typeName(); //TODO better toString
}

QScriptValue StructUnionScriptClass::StructUnion_proto_child(QScriptContext* ctx, QScriptEngine* eng)
{
    if (ctx->argumentCount() < 1)
    {
        ctx->throwError(QScriptContext::RangeError, "(struct/union).child(name) needs at least one argument");
        return eng->undefinedValue();
    }
    QScriptValue arg = ctx->argument(0);
    if (!arg.isString())
    {
        ctx->throwError(QScriptContext::TypeError, "(struct/union).child(name) argument has to be a string");
        return QScriptValue::UndefinedValue;
    }
    DataInformation* data = qscriptvalue_cast<DataInformation*>(ctx->thisObject().data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    uint count = data->childCount();
    QString name = arg.toString();
    for (uint i = 0; i < count; i++)
    {
        DataInformation* child = data->childAt(i);
        if (child->name() == name)
            return child->toScriptValue(eng, data->topLevelDataInformation()->scriptHandlerInfo());
    }
    return eng->nullValue();
}

QScriptValue StructUnionScriptClass::StructUnion_proto_setChildren(QScriptContext* ctx, QScriptEngine* eng)
{
    if (ctx->argumentCount() < 1)
    {
        return ctx->throwError(QScriptContext::RangeError, "(struct/union).child(children) needs one argument");
    }
    DataInformation* data = qscriptvalue_cast<DataInformation*>(ctx->thisObject().data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    DataInformationWithChildren* dataW = static_cast<DataInformationWithChildren*>(data);
    //do a dynamic cast in debug mode to ensure the static cast was valid
    Q_CHECK_PTR(dynamic_cast<DataInformationWithChildren*>(dataW));

    dataW->setChildren(ctx->argument(0));
    return eng->undefinedValue();
}
