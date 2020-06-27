/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringscriptclass.hpp"
#include "../../datatypes/strings/stringdatainformation.hpp"
#include "../../parsers/parserutils.hpp"
#include "../../structlogging.hpp"

StringScriptClass::StringScriptClass(QScriptEngine* eng, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(eng, handlerInfo)
{
    mIterableProperties.reserve(mIterableProperties.size() + 6);
    // read-only properties
    s_lengthInCodepoints = eng->toStringHandle(ParserStrings::PROPERTY_CHAR_COUNT());
    mIterableProperties.append(qMakePair(s_lengthInCodepoints, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_lengthInBytes = eng->toStringHandle(ParserStrings::PROPERTY_BYTE_COUNT());
    mIterableProperties.append(qMakePair(s_lengthInBytes, QScriptValue::ReadOnly | QScriptValue::Undeletable));

    // read-write properties
    s_maxByteCount = eng->toStringHandle(ParserStrings::PROPERTY_MAX_BYTE_COUNT());
    mIterableProperties.append(qMakePair(s_maxByteCount, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_maxCharCount = eng->toStringHandle(ParserStrings::PROPERTY_MAX_CHAR_COUNT());
    mIterableProperties.append(qMakePair(s_maxCharCount, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_terminatedBy = eng->toStringHandle(ParserStrings::PROPERTY_TERMINATED_BY());
    mIterableProperties.append(qMakePair(s_terminatedBy, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_encoding = eng->toStringHandle(ParserStrings::PROPERTY_ENCODING());
    mIterableProperties.append(qMakePair(s_encoding, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));

    mStringPrototype = eng->newObject();
    mStringPrototype.setProperty(QStringLiteral("toString"), eng->newFunction(String_proto_toString));
}

StringScriptClass::~StringScriptClass() = default;

bool StringScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    Q_UNUSED(data)
    // no need to modify flags since both read and write are handled
    if (name == s_maxByteCount || name == s_maxCharCount || name == s_terminatedBy
        || name == s_encoding) {
        return true;
    }
    if (name == s_lengthInCodepoints || name == s_lengthInBytes) {
        *flags &= ~HandlesWriteAccess;
        return true;
    }

    bool isArrayIndex;
    quint32 pos = name.toArrayIndex(&isArrayIndex);
    if (isArrayIndex && pos <= uint(data->asString()->stringLength())) {
        *id = pos + 1; // add 1 to distinguish from the default value of 0
        *flags &= ~HandlesWriteAccess; // writing is not yet supported
        return true;
    }
    return false; // not found
}

bool StringScriptClass::additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint id, QScriptValue::PropertyFlags* flags)
{
    Q_UNUSED(data)
    Q_UNUSED(name)
    if (id != 0) {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    return false;
}

QScriptValue StringScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    const StringDataInformation* sData = data->asString();

    if (id != 0) {
        quint32 pos = id - 1;
        if (pos >= uint(sData->stringLength())) {
            return engine()->currentContext()->throwError(QScriptContext::RangeError,
                                                          QStringLiteral("Attempting to access string index %1, but length is %2").arg(
                                                              QString::number(pos), QString::number(sData->stringLength())));
        }
        return sData->valueAt(pos);
    }
    if (name == s_lengthInCodepoints) {
        return sData->stringLength();
    }
    if (name == s_lengthInBytes) {
        return sData->stringByteLength();
    }
    if (name == s_encoding) {
        return sData->encodingName();
    }
    if (name == s_maxCharCount) {
        return sData->maxCharCount();
    }
    if (name == s_maxByteCount) {
        return sData->maxByteCount();
    }
    if (name == s_terminatedBy) {
        return sData->terminationCodePoint();
    }
    return {};
}

bool StringScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    StringDataInformation* sData = data->asString();

    if (name == s_maxCharCount) {
        if (value.isNull()) {
            sData->logInfo() << "Unsetting max char count.";
            sData->unsetTerminationMode(StringData::CharCount);
        } else {
            ParsedNumber<uint> result = ParserUtils::uintFromScriptValue(value);
            if (result.isValid) {
                sData->setMaxCharCount(result.value);
            } else {
                sData->logError() << "Could not set maximum char count, invalid argument: " << value.toString();
            }
        }
        return true;
    }
    if (name == s_maxByteCount) {
        if (value.isNull()) {
            sData->logInfo() << "Unsetting max byte count.";
            sData->unsetTerminationMode(StringData::ByteCount);
        } else {
            ParsedNumber<uint> result = ParserUtils::uintFromScriptValue(value);
            if (result.isValid) {
                sData->setMaxByteCount(result.value);
            } else {
                sData->logError() << "Could not set maximum byte count, invalid argument: " << value.toString();
            }
        }
        return true;
    }
    if (name == s_terminatedBy) {
        if (value.isNull()) {
            sData->logInfo() << "Unsetting termination character.";
            sData->unsetTerminationMode(StringData::Sequence);
        } else {
            if (value.isString()) {
                QString str = value.toString();
                // we don't handle surrogate pairs, if you want to set that use a number instead.
                if (str.length() != 1) {
                    sData->logError() << "Setting termination char: expected one char or a code point number"
                        ", got a string with length " << str.length();
                } else {
                    sData->setTerminationCodePoint(str[0].unicode());
                }
            } else {
                ParsedNumber<uint> result = ParserUtils::uintFromScriptValue(value);
                if (result.isValid) {
                    sData->setTerminationCodePoint(result.value);
                } else {
                    sData->logError() << "Could not set maximum byte count, invalid argument: " << value.toString();
                }
            }
        }
        return true;
    }
    if (name == s_encoding) {
        QString enc = value.toString();
        StringDataInformation::StringType encoding = ParserUtils::toStringEncoding(enc,
                                                                                   LoggerWithContext(sData->logger(), sData->fullObjectPath()));
        if (encoding == StringDataInformation::StringType::InvalidEncoding) {
            sData->logError() << "Attempting to set invalid encoding:" << enc;
        } else {
            sData->setEncoding(encoding);
        }
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
    if (!data) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast data";
        return eng->undefinedValue();
    }
    return data->wasAbleToRead() ? data->valueString() : eng->undefinedValue();
}
