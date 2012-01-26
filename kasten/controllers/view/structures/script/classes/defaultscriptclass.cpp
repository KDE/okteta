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

#include "defaultscriptclass.h"

#include "../../parsers/abstractstructureparser.h"
#include "../../datatypes/datainformation.h"

DefaultScriptClass::DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : QScriptClass(engine), mHandlerInfo(handlerInfo)
{
    valid = engine->toStringHandle(QLatin1String("valid"));
    wasAbleToRead = engine->toStringHandle(QLatin1String("wasAbleToRead"));
    validationError = engine->toStringHandle(QLatin1String("validationError"));
    parent = engine->toStringHandle(QLatin1String("parent"));
    byteOrder = engine->toStringHandle(QLatin1String("byteOrder"));
    qScriptRegisterMetaType<DataInfPtr>(engine, DefaultScriptClass::toScriptValue, DefaultScriptClass::fromScriptValue);

    //TODO remove, every subclass should have proto
    mDefaultPrototype = engine->newObject();
    mDefaultPrototype.setProperty(QLatin1String("toString"), engine->newFunction(Default_proto_toString));
}

DefaultScriptClass::~DefaultScriptClass()
{
}

QScriptValue DefaultScriptClass::toScriptValue(QScriptEngine* eng, const DataInfPtr& data)
{
    QScriptValue obj = eng->newObject();
    obj.setData(eng->newVariant(QVariant::fromValue(data)));
    return obj;
}

void DefaultScriptClass::fromScriptValue(const QScriptValue& obj, DataInfPtr& data)
{
    data = qvariant_cast<DataInfPtr>(obj.data().toVariant());
}

QScriptClass::QueryFlags DefaultScriptClass::queryProperty(const QScriptValue& object,
            const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
        return 0;
    if (name == valid || name == validationError || name == byteOrder)
    {
        return flags;
    }
    else if (name == wasAbleToRead)
    {
        return flags &= ~HandlesWriteAccess;
    }
    else if (name == parent)
    {
        return flags &= ~HandlesWriteAccess;
    }
    else if (queryAdditionalProperty(data, name, &flags, id))
    {
        return flags;
    }
    else
    {
        return 0;
    }
}

QScriptValue DefaultScriptClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return QScriptValue();
    }
    if (name == valid)
    {
        return data->validationSuccessful();
    }
    else if (name == wasAbleToRead)
    {
        return data->wasAbleToRead();
    }
    else if (name == parent)
    {
        //parent() cannot be null
        if (data->parent()->isTopLevel())
            return QScriptValue::NullValue;
        return data->parent()->asDataInformation()->toScriptValue(engine(), mHandlerInfo);
    }
    else if (name == validationError)
    {
        return data->validationError();
    }
    else if (name == byteOrder)
    {
        return AbstractStructureParser::byteOrderToString(data->byteOrder());
    }
    QScriptValue other = additionalProperty(data, name, id);
    if (other.isValid())
        return other;
    else
        return engine()->undefinedValue();
}

void DefaultScriptClass::setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return;
    }

    if (name == valid)
    {
        data->setValidationSuccessful(value.toBool());
    }
    else if (name == validationError)
    {
        data->setValidationError(value.toString());
    }
    else if (name == byteOrder)
    {
        data->setByteOrder(AbstractStructureParser::byteOrderFromString(value.toString()));
    }
    else if (name == wasAbleToRead)
    {
        return; //can't write
    }
    else
    {
        //TODO access by name
        bool setAdditional = setAdditionalProperty(data, name, id, value);
        if (setAdditional)
            return;
        else
        {
            kDebug() << "cannot set unknown property: " << name.toString();
        }
    }
}

QScriptValue::PropertyFlags DefaultScriptClass::propertyFlags(const QScriptValue& object, const QScriptString& name, uint id)
{
    QScriptValue::PropertyFlags result;
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return result;
    }
    //TODO allow removing of children, not just setting
    result |= QScriptValue::Undeletable; //no property is deleteable
    if (name == wasAbleToRead)
    {
        result |= QScriptValue::ReadOnly;
    }
    else if (name == parent)
    {
        result |= QScriptValue::ReadOnly;
    }
    else if (!additionalPropertyFlags(data, name, id, &result))
    {
        //is a child element
        result |= QScriptValue::ReadOnly;
    }
    return result;
}

QScriptValue DefaultScriptClass::prototype() const
{
    return mDefaultPrototype;
}

QScriptValue DefaultScriptClass::Default_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(ctx->thisObject().data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    return QString(data->typeName() + QLatin1Char(' ') + data->name());
}
