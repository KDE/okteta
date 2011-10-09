
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


#include "stringscriptclass.h"
#include "../../datatypes/strings/stringdatainformation.h"

StringScriptClass::StringScriptClass(QScriptEngine* eng, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(eng, handlerInfo)
{
    s_length = eng->toStringHandle(QLatin1String("charCount"));
    s_lengthInBytes = eng->toStringHandle(QLatin1String("byteCount"));
    s_maxByteCount = eng->toStringHandle(QLatin1String("maxByteCount"));
    s_maxCharCount = eng->toStringHandle(QLatin1String("maxCharCount"));
    s_terminatedBy = eng->toStringHandle(QLatin1String("terminatedBy"));
    s_encoding = eng->toStringHandle(QLatin1String("encoding"));
    s_termMode = eng->toStringHandle(QLatin1String("termMod"));

    mStringPrototype = eng->newObject();
    mStringPrototype.setProperty(QLatin1String("toString"),
                                 eng->newFunction(String_proto_toString));
}

StringScriptClass::~StringScriptClass()
{
}

bool StringScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (name == s_maxByteCount || name == s_maxCharCount || name == s_terminatedBy
            || name == s_encoding || name == s_termMode)
        return true;
    else if (name == s_length || name == s_lengthInBytes)
    {
        *flags &= ~HandlesWriteAccess;
        return true;
    }
    else
    {
        bool isArrayIndex;
        quint32 pos = name.toArrayIndex(&isArrayIndex);
        if (isArrayIndex && pos <= uint(data->asString()->stringLength()))
        {
            *id = pos + 1; //add 1 to distinguish from the default value of 0
            *flags &= ~HandlesWriteAccess; //writing is not yet supported
            return true;
        }
    }
    return false; //not found
}

bool StringScriptClass::additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint id, QScriptValue::PropertyFlags* flags)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (id != 0 || name == s_length || name == s_lengthInBytes)
    {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    else if (name == s_maxByteCount || name == s_maxCharCount || name == s_terminatedBy
            || name == s_encoding || name == s_termMode)
        return true;
    return false;
}


QScriptValue StringScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    const StringDataInformation* sData = data->asString();

    if (id != 0)
    {
        quint32 pos = id - 1;
#ifdef OKTETA_DEBUG_SCRIPT
        kDebug() << "accessing property with id=" << id << "and name=" << name.toString();
#endif
        if (pos >= uint(sData->stringLength()))
            return engine()->undefinedValue();
        else
        {
            return sData->valueAt(pos);
        }
    }
    else if (name == s_length)
        return sData->stringLength();
    else if (name == s_lengthInBytes)
        return sData->stringByteLength();
    else if (name == s_encoding)
        return stringEncodings[sData->encoding()];
    else if (name == s_maxCharCount)
        return sData->maxCharCount();
    else if (name == s_maxByteCount)
        return sData->maxByteCount();
    else if (name == s_terminatedBy)
        return sData->terminationCodePoint();
    else if (name == s_termMode)
        return sData->terminationMode();
    return QScriptValue();
}

bool StringScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    StringDataInformation* sData = data->asString();

    if (name == s_maxCharCount)
    {
        sData->setMaxCharCount(value, engine());
        return true;
    }
    else if (name == s_maxByteCount)
    {
        sData->setMaxByteCount(value, engine());
        return true;
    }
    else if (name == s_terminatedBy)
    {
        sData->setTerminationCodePoint(value, engine());
        return true;
    }
    else if (name == s_encoding)
    {
        sData->setEncoding(value.toString());
        return true;
    }
    else if (name == s_termMode)
    {
        if (!value.isNumber())
        {
            engine()->currentContext()->throwError(QScriptContext::TypeError,
                QLatin1String("Value is not a number: ") + value.toString());
        }
        else
        {
            sData->setTerminationCodePoint(value.toUInt32());
        }
    }
    return false;
}

QScriptValue StringScriptClass::prototype() const
{
    return mStringPrototype;
}

QScriptValue StringScriptClass::String_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(ctx->thisObject().data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    return data->valueString();
}







