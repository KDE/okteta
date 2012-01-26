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
#include "../../datatypes/topleveldatainformation.h"

StructUnionScriptClass::StructUnionScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(engine, handlerInfo)
{
    s_childCount = engine->toStringHandle(QLatin1String("childCount"));
    s_children = engine->toStringHandle(QLatin1String("children"));

    mStructUnionPrototype = engine->newObject();
    mStructUnionPrototype.setProperty(QLatin1String("toString"),
                                      engine->newFunction(StructUnion_proto_toString));
    mStructUnionPrototype.setProperty(QLatin1String("setChildren"),
                                      engine->newFunction(StructUnion_proto_setChildren));
    mStructUnionPrototype.setProperty(QLatin1String("child"),
                                      engine->newFunction(StructUnion_proto_child));
}

StructUnionScriptClass::~StructUnionScriptClass()
{
}

bool StructUnionScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
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
    else
    {
        bool isArrayIndex;
        quint32 pos = name.toArrayIndex(&isArrayIndex);
        uint count = data->childCount();
        bool isValidChild = false;
        if (isArrayIndex && pos < count)
        {
            isValidChild = true;
        }
        else
        {
            //compare name, names that match special properties/functions will be
            //hidden since these were checked before
            QString objName = name.toString();
            for (uint i = 0 ; i < count; ++i)
            {
                if (objName == data->childAt(i)->name())
                {
                    isValidChild = true;
                    pos = i;
                    break;
                }
            }
        }
        if (isValidChild)
        {
            *id = pos + 1; //add 1 to distinguish from the default value of 0
            *flags &= ~HandlesWriteAccess; //writing is not yet supported
            return true;
        }
    }
    return false; //not found
}

bool StructUnionScriptClass::additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint id, QScriptValue::PropertyFlags* flags)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (id != 0)
    {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    else if (name == s_childCount)
    {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    else if (name == s_children)
    {
        //there is no way to set a write-only property
        return true;
    }
    else
    {
        //TODO is this necessary, will there be any way a child has no id set?
        //check named children
        QString objName = name.toString();
        uint count = data->childCount();
        for (uint i = 0 ; i < count; ++i)
        {
            DataInformation* child = data->childAt(i);
            Q_CHECK_PTR(child);
            if (objName == child->name())
            {
                *flags |= QScriptValue::ReadOnly;
                return true;
            }
        }
    }
    return false;
}


QScriptValue StructUnionScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    const DataInformationWithChildren* dataW = static_cast<const DataInformationWithChildren*>(data);
    //do a dynamic cast in debug mode to ensure the static cast was valid
    Q_CHECK_PTR(dynamic_cast<const DataInformationWithChildren*>(dataW));

    if (id != 0)
    {
        quint32 pos = id - 1;
#ifdef OKTETA_DEBUG_SCRIPT
        kDebug() << "accessing property with id=" << id << "and name=" << name.toString();
#endif
        if (pos >= data->childCount())
            return engine()->undefinedValue();
        else
        {
            Q_CHECK_PTR(data->childAt(pos));
            return data->childAt(pos)->toScriptValue(engine(), mHandlerInfo);
        }
    }
    else if (name == s_childCount)
        return dataW->childCount();
    else if (name == s_children)
        return engine()->undefinedValue();
    else
    {
        //TODO is this necessary, will there be any way a child has no id set?
        //TODO testing seems to indicate this is not necessary, will leave it thought until I'm sure
        //check named children
        QString objName = name.toString();
        uint count = data->childCount();
        for (uint i = 0 ; i < count; ++i)
        {
            DataInformation* child = data->childAt(i);
            Q_CHECK_PTR(child);
            if (objName == child->name())
            {
                return child->toScriptValue(engine(), mHandlerInfo);
            }
        }
    }
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
    //TODO set children!!
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
        ctx->throwError(QScriptContext::RangeError,
                        QLatin1String("(struct/union).child(name) needs at least one argument"));
        return eng->undefinedValue();
    }
    QScriptValue arg = ctx->argument(0);
    if (!arg.isString())
    {
        ctx->throwError(QScriptContext::TypeError,
                        QLatin1String("(struct/union).child(name) argument has to be a string"));
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
        return ctx->throwError(QScriptContext::RangeError,
                               QLatin1String("(struct/union).child(children) needs one argument"));
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
