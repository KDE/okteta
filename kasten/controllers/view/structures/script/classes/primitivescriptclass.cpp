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


#include "primitivescriptclass.h"
#include "../../datatypes/primitive/primitivedatainformation.h"
#include "../../parsers/parserutils.h"
#include "../../allprimitivetypes.h"

PrimitiveScriptClass::PrimitiveScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(engine, handlerInfo)
{
    s_value = engine->toStringHandle(QLatin1String("value"));
    mIterableProperties.append(qMakePair(s_value, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_type = engine->toStringHandle(QLatin1String("type"));
    mIterableProperties.append(qMakePair(s_type, QScriptValue::ReadOnly | QScriptValue::Undeletable));

    s_bool = engine->toStringHandle(QLatin1String("bool"));
    mIterableProperties.append(qMakePair(s_bool, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_char = engine->toStringHandle(QLatin1String("char"));
    mIterableProperties.append(qMakePair(s_char, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_double = engine->toStringHandle(QLatin1String("double"));
    mIterableProperties.append(qMakePair(s_double, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_float = engine->toStringHandle(QLatin1String("float"));
    mIterableProperties.append(qMakePair(s_float, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int = engine->toStringHandle(QLatin1String("int"));
    mIterableProperties.append(qMakePair(s_int, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int8 = engine->toStringHandle(QLatin1String("int8"));
    mIterableProperties.append(qMakePair(s_int8, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int16 = engine->toStringHandle(QLatin1String("int16"));
    mIterableProperties.append(qMakePair(s_int16, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int32 = engine->toStringHandle(QLatin1String("int32"));
    mIterableProperties.append(qMakePair(s_int32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int64low32 = engine->toStringHandle(QLatin1String("int64low32"));
    mIterableProperties.append(qMakePair(s_int64low32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int64high32 = engine->toStringHandle(QLatin1String("int64high32"));
    mIterableProperties.append(qMakePair(s_int64high32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int64 = engine->toStringHandle(QLatin1String("int64"));
    mIterableProperties.append(qMakePair(s_int64, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint = engine->toStringHandle(QLatin1String("uint"));
    mIterableProperties.append(qMakePair(s_uint, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint8 = engine->toStringHandle(QLatin1String("uint8"));
    mIterableProperties.append(qMakePair(s_uint8, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint16 = engine->toStringHandle(QLatin1String("uint16"));
    mIterableProperties.append(qMakePair(s_uint16, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint32 = engine->toStringHandle(QLatin1String("uint32"));
    mIterableProperties.append(qMakePair(s_uint32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint64low32 = engine->toStringHandle(QLatin1String("uint64low32"));
    mIterableProperties.append(qMakePair(s_uint64low32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint64high32 = engine->toStringHandle(QLatin1String("uint64high32"));
    mIterableProperties.append(qMakePair(s_uint64high32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint64 = engine->toStringHandle(QLatin1String("uint64"));
    mIterableProperties.append(qMakePair(s_uint64, QScriptValue::ReadOnly | QScriptValue::Undeletable));

    mPrimitivePrototype = engine->newObject();
    mPrimitivePrototype.setProperty(QLatin1String("toString"), engine->newFunction(Primitive_proto_toString));
}

PrimitiveScriptClass::~PrimitiveScriptClass()
{
}

bool PrimitiveScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint*)
{
    Q_UNUSED(data)
    //TODO assign ids to improve speed? cant have children, so it won't conflict
    //no need to modify flags since both read and write are handled
    if (name == s_value || name == s_type)
    {
        *flags &= ~HandlesWriteAccess;
        return true;
    }
    else if (name == s_bool || name == s_char || name == s_int || name == s_uint || name == s_float
            || name == s_double || name == s_int64 || name == s_uint64 || name == s_int64low32
            || name == s_int64high32 || name == s_uint64low32 || name == s_uint64high32 || name == s_int8
            || name == s_int16 || name == s_int32 || name == s_uint8 || name == s_uint16 || name == s_uint32)
    {
        *flags &= ~HandlesWriteAccess;
        return true;
    }
    return false;
}

bool PrimitiveScriptClass::additionalPropertyFlags(const DataInformation*, const QScriptString&, uint, QScriptValue::PropertyFlags*)
{
    return false;
}


QScriptValue PrimitiveScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint)
{
    const PrimitiveDataInformation* pData = data->asPrimitive();

    if (name == s_value)
    {
        if (pData->wasAbleToRead())
            return pData->valueAsQScriptValue();
        QScriptValue callee = engine()->currentContext()->thisObject();
        DataInformation* cause = toDataInformation(callee);
        if (cause)
            pData->logError() << "Attempting to read from uninitialized value. Callee was " << cause->fullObjectPath();
        else
            pData->logError() << "Attempting to read from uninitialized value. Callee could not be determined";
        return engine()->undefinedValue();
    }
    else if (name == s_type)
    {
        //bitfields are handled by own scriptclass and NotPrimitive indicates an error
        Q_ASSERT(!pData->isBitfield());
        Q_ASSERT(pData->type() != Type_Invalid);
        return PrimitiveType::standardTypeName(pData->type());
    }
    else {
        AllPrimitiveTypes value = pData->value();
        if (name == s_bool)
            return value.ulongValue != 0;
        else if (name == s_char)
            return QString(value.ubyteValue > 127 ? QChar::ReplacementCharacter : QChar(value.ubyteValue, 0));
        else if (name == s_float)
            return value.floatValue;
        else if (name == s_double)
            return value.doubleValue;
        else if (name == s_int || name == s_int32 || name == s_int64low32)
            return value.intValue;
        else if (name == s_uint || name == s_uint32 || name == s_uint64low32)
            return value.intValue;
        else if (name == s_int64)
            return QString::number(value.longValue);
        else if (name == s_uint64)
            return QString::number(value.ulongValue);
        else if (name == s_int64high32)
            return qint32(value.ulongValue >> 32);
        else if (name == s_uint64high32)
            return quint32(value.ulongValue >> 32);
        else if (name == s_int8)
            return qint32(value.byteValue);
        else if (name == s_int16)
            return qint32(value.shortValue);
        else if (name == s_uint8)
            return quint32(value.ubyteValue);
        else if (name == s_uint16)
            return quint32(value.ushortValue);
        else
            return QScriptValue();
    }
}

bool PrimitiveScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    Q_UNUSED(value);
    Q_UNUSED(data);
    Q_UNUSED(name);
    //TODO allow changing type
    return false;
}

QScriptValue PrimitiveScriptClass::prototype() const
{
    return mPrimitivePrototype;
}

QScriptValue PrimitiveScriptClass::Primitive_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = toDataInformation(ctx->thisObject());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    //this might allow proper comparison between values without having to call .value
    return data->wasAbleToRead() ? data->valueString() : eng->undefinedValue();
}

