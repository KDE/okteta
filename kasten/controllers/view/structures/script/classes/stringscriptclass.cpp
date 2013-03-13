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


#include "stringscriptclass.h"
#include "../../datatypes/strings/stringdatainformation.h"
#include "../../parsers/parserutils.h"

#include <KDebug>

StringScriptClass::StringScriptClass(QScriptEngine* eng, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(eng, handlerInfo)
{
    //read-only properties
    s_lengthInCodepoints = eng->toStringHandle(ParserStrings::PROPERTY_CHAR_COUNT);
    mIterableProperties.append(qMakePair(s_lengthInCodepoints, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_lengthInBytes = eng->toStringHandle(ParserStrings::PROPERTY_BYTE_COUNT);
    mIterableProperties.append(qMakePair(s_lengthInBytes, QScriptValue::ReadOnly | QScriptValue::Undeletable));

    //read-write properties
    s_maxByteCount = eng->toStringHandle(ParserStrings::PROPERTY_MAX_BYTE_COUNT);
    mIterableProperties.append(qMakePair(s_maxByteCount, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_maxCharCount = eng->toStringHandle(ParserStrings::PROPERTY_MAX_CHAR_COUNT);
    mIterableProperties.append(qMakePair(s_maxCharCount, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_terminatedBy = eng->toStringHandle(ParserStrings::PROPERTY_TERMINATED_BY);
    mIterableProperties.append(qMakePair(s_terminatedBy, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_encoding = eng->toStringHandle(ParserStrings::PROPERTY_ENCODING);
    mIterableProperties.append(qMakePair(s_encoding, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));

    mStringPrototype = eng->newObject();
    mStringPrototype.setProperty(QLatin1String("toString"), eng->newFunction(String_proto_toString));
}

StringScriptClass::~StringScriptClass()
{
}

bool StringScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    Q_UNUSED(data)
    //no need to modify flags since both read and write are handled
    if (name == s_maxByteCount || name == s_maxCharCount || name == s_terminatedBy
            || name == s_encoding)
        return true;
    else if (name == s_lengthInCodepoints || name == s_lengthInBytes)
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
    Q_UNUSED(name)
    if (id != 0 )
    {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    return false;
}


QScriptValue StringScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    const StringDataInformation* sData = data->asString();

    if (id != 0)
    {
        quint32 pos = id - 1;
        if (pos >= uint(sData->stringLength()))
        {
            return engine()->currentContext()->throwError(QScriptContext::RangeError,
                QString(QLatin1String("Attempting to access string index %1, but length is %2")).arg(
                    QString::number(pos), QString::number(sData->stringLength())));
        }
        else
        {
            return sData->valueAt(pos);
        }
    }
    else if (name == s_lengthInCodepoints)
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
    return QScriptValue();
}

bool StringScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    StringDataInformation* sData = data->asString();

    if (name == s_maxCharCount)
    {
        if (value.isNull())
        {
            sData->logInfo() << "Unsetting max char count.";
            sData->unsetTerminationMode(StringData::CharCount);
        }
        else
        {
            ParsedNumber<uint> result = ParserUtils::uintFromScriptValue(value);
            if (result.isValid)
                sData->setMaxCharCount(result.value);
            else
                sData->logError() << "Could not set maximum char count, invalid argument: " << value.toString();
        }
        return true;
    }
    else if (name == s_maxByteCount)
    {
        if (value.isNull())
        {
            sData->logInfo() << "Unsetting max byte count.";
            sData->unsetTerminationMode(StringData::ByteCount);
        }
        else
        {
            ParsedNumber<uint> result = ParserUtils::uintFromScriptValue(value);
            if (result.isValid)
                sData->setMaxByteCount(result.value);
            else
                sData->logError() << "Could not set maximum byte count, invalid argument: " << value.toString();
        }
        return true;
    }
    else if (name == s_terminatedBy)
    {
        if (value.isNull())
        {
            sData->logInfo() << "Unsetting termination character.";
            sData->unsetTerminationMode(StringData::Sequence);
        }
        else
        {
            if (value.isString())
            {
                QString str = value.toString();
                //we don't handle surrogate pairs, if you want to set that use a number instead.
                if (str.length() != 1)
                    sData->logError() << "Setting termination char: expected one char or a code point number"
                            ", got a string with length " << str.length();
                else
                    sData->setTerminationCodePoint(str[0].unicode());
            }
            else
            {
                ParsedNumber<uint> result = ParserUtils::uintFromScriptValue(value);
                if (result.isValid)
                    sData->setTerminationCodePoint(result.value);
                else
                    sData->logError() << "Could not set maximum byte count, invalid argument: " << value.toString();
            }
        }
        return true;
    }
    else if (name == s_encoding)
    {
        QString enc = value.toString();
        StringDataInformation::StringType encoding = ParserUtils::toStringEncoding(enc,
                LoggerWithContext(sData->logger(), sData->fullObjectPath()));
        if (encoding == StringDataInformation::InvalidEncoding)
            sData->logError() << "Attempting to set invalid encoding:" << enc;
        else
            sData->setEncoding(encoding);
        return true;
    }
    return false;
}

QScriptValue StringScriptClass::prototype() const
{
    return mStringPrototype;
}

QScriptValue StringScriptClass::String_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = toDataInformation(ctx->thisObject().data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return eng->undefinedValue();
    }
    return data->wasAbleToRead() ? data->valueString() : eng->undefinedValue();
}







