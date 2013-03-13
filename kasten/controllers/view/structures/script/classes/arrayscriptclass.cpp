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


#include "arrayscriptclass.h"
#include "../../datatypes/array/arraydatainformation.h"
#include "../../parsers/parserutils.h"
#include "../../parsers/scriptvalueconverter.h"
#include "../scriptlogger.h"

#include <KDebug>

ArrayScriptClass::ArrayScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(engine, handlerInfo)
{
    s_length = engine->toStringHandle(ParserStrings::PROPERTY_LENGTH);
    mIterableProperties.append(qMakePair(s_length, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_childType = engine->toStringHandle(QLatin1String("childType"));
    //the preferred property (the same as childType)
    s_type = engine->toStringHandle(ParserStrings::PROPERTY_TYPE);
    mIterableProperties.append(qMakePair(s_type, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));

    mArrayPrototype = engine->newObject();
    mArrayPrototype.setProperty(QLatin1String("toString"), engine->newFunction(Array_proto_toString));
}

ArrayScriptClass::~ArrayScriptClass()
{
}

bool ArrayScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (name == s_length)
        return true;
    else if (name == s_type || name == s_childType)
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
    Q_UNUSED(name)
    if (name == s_childType)
        return true; //undeleteable is on by default
    if (id != 0)
    {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    return false;
}


QScriptValue ArrayScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    const ArrayDataInformation* aData = data->asArray();
    if (id != 0)
    {
        quint32 pos = id - 1;
        if (pos >= data->childCount())
        {
            aData->logError() << "attempting to access out of bounds child: index was" << pos
                    << ", length is" << data->childCount();
            return engine()->currentContext()->throwError(QScriptContext::RangeError,
                QString(QLatin1String("Attempting to access array index %1, but length is %2")).arg(
                    QString::number(pos), QString::number(data->childCount())));
        }
        else
        {
            return aData->childToScriptValue(pos, engine(), mHandlerInfo);
        }
    }
    else if (name == s_length)
        return aData->length();
    else if (name == s_type)
        return aData->childType();
    else if (name == s_childType)
    {
        aData->logWarn() << "Using property 'childType' is deprecated, use the new name 'type' instead";
        return aData->childType();
    }
    return QScriptValue();
}

bool ArrayScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    ArrayDataInformation* aData = data->asArray();
    if (name == s_length)
    {
        ParsedNumber<uint> newLength = ParserUtils::uintFromScriptValue(value);

        if (!newLength.isValid)
        {
            aData->logError() << "new length of array is invalid:" << newLength.string;
            aData->setArrayLength(0);
        }
        else
        {
            aData->setArrayLength(newLength.value);
        }
        return true;
    }
    else if (name == s_type || name == s_childType)
    {
        if (name == s_childType)
            aData->logWarn() << "Using property 'childType' is deprecated, use the new name 'type' instead";

        DataInformation* newChildType = ScriptValueConverter::convert(value,
                aData->name(), aData->logger(), aData);

        if (!newChildType)
            aData->logError() << "Failed to parse new child type:" << value.toString();
        else
            aData->setArrayType(newChildType);
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
    DataInformation* data = toDataInformation(ctx->thisObject());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    return data->typeName();
}

