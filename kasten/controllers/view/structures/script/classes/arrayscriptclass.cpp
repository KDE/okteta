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


#include "arrayscriptclass.h"
#include "../../datatypes/array/arraydatainformation.h"

ArrayScriptClass::ArrayScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(engine, handlerInfo)
{
    length = engine->toStringHandle(QLatin1String("length"));
    childType = engine->toStringHandle(QLatin1String("childType"));

    mArrayPrototype = engine->newObject();
    mArrayPrototype.setProperty(QLatin1String("toString"),
                                engine->newFunction(Array_proto_toString));
}

ArrayScriptClass::~ArrayScriptClass()
{
}

bool ArrayScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (name == length)
        return true;
    else if (name == childType)
        return true;
    else
    {
        bool isArrayIndex;
        quint32 pos = name.toArrayIndex(&isArrayIndex);
        if (isArrayIndex && pos <= data->childCount())
        {
            *id = pos + 1; //add 1 to distinguish from the default value of 0
            *flags &= ~HandlesWriteAccess; //writing is not yet supported
            return true;
        }
    }
    return false; //not found
}

bool ArrayScriptClass::additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint id, QScriptValue::PropertyFlags* flags)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (id != 0)
    {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    if (name == length)
        return true;
    else if (name == childType)
        return true;
    return false;
}


QScriptValue ArrayScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    const ArrayDataInformation* aData = data->asArray();
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
            return aData->childToScriptValue(pos, engine(), mHandlerInfo);
        }
    }
    else if (name == length)
        return aData->length();
    else if (name == childType)
        return aData->childType();
    return QScriptValue();
}

bool ArrayScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    ArrayDataInformation* aData = data->asArray();
    if (name == length)
    {
        if (!value.isNumber())
        {
            engine()->currentContext()->throwError(QScriptContext::TypeError,
                QLatin1String("Value is not a number: ") + value.toString());
        }
        else
        {
            aData->setArrayLength(value.toInt32(), engine()->currentContext());
        }
        return true;
    }
    else if (name == childType)
    {
        aData->setArrayType(value, engine()->currentContext());
        return true;
    }
    return false;
}

QScriptValue ArrayScriptClass::prototype() const
{
    return mArrayPrototype;
}

QScriptValue ArrayScriptClass::Array_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(ctx->thisObject().data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    return data->typeName();
}

