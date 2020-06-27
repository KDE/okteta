/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "primitivescriptclass.hpp"
#include "../../datatypes/primitive/primitivedatainformation.hpp"
#include "../../parsers/parserutils.hpp"
#include "../../allprimitivetypes.hpp"
#include "../../structlogging.hpp"

PrimitiveScriptClass::PrimitiveScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(engine, handlerInfo)
{
    mIterableProperties.reserve(mIterableProperties.size() + 20);
    s_value = engine->toStringHandle(QStringLiteral("value"));
    mIterableProperties.append(qMakePair(s_value, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_type = engine->toStringHandle(QStringLiteral("type"));
    mIterableProperties.append(qMakePair(s_type, QScriptValue::ReadOnly | QScriptValue::Undeletable));

    s_bool = engine->toStringHandle(QStringLiteral("bool"));
    mIterableProperties.append(qMakePair(s_bool, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_char = engine->toStringHandle(QStringLiteral("char"));
    mIterableProperties.append(qMakePair(s_char, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_double = engine->toStringHandle(QStringLiteral("double"));
    mIterableProperties.append(qMakePair(s_double, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_float = engine->toStringHandle(QStringLiteral("float"));
    mIterableProperties.append(qMakePair(s_float, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int = engine->toStringHandle(QStringLiteral("int"));
    mIterableProperties.append(qMakePair(s_int, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int8 = engine->toStringHandle(QStringLiteral("int8"));
    mIterableProperties.append(qMakePair(s_int8, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int16 = engine->toStringHandle(QStringLiteral("int16"));
    mIterableProperties.append(qMakePair(s_int16, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int32 = engine->toStringHandle(QStringLiteral("int32"));
    mIterableProperties.append(qMakePair(s_int32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int64low32 = engine->toStringHandle(QStringLiteral("int64low32"));
    mIterableProperties.append(qMakePair(s_int64low32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int64high32 = engine->toStringHandle(QStringLiteral("int64high32"));
    mIterableProperties.append(qMakePair(s_int64high32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_int64 = engine->toStringHandle(QStringLiteral("int64"));
    mIterableProperties.append(qMakePair(s_int64, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint = engine->toStringHandle(QStringLiteral("uint"));
    mIterableProperties.append(qMakePair(s_uint, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint8 = engine->toStringHandle(QStringLiteral("uint8"));
    mIterableProperties.append(qMakePair(s_uint8, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint16 = engine->toStringHandle(QStringLiteral("uint16"));
    mIterableProperties.append(qMakePair(s_uint16, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint32 = engine->toStringHandle(QStringLiteral("uint32"));
    mIterableProperties.append(qMakePair(s_uint32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint64low32 = engine->toStringHandle(QStringLiteral("uint64low32"));
    mIterableProperties.append(qMakePair(s_uint64low32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint64high32 = engine->toStringHandle(QStringLiteral("uint64high32"));
    mIterableProperties.append(qMakePair(s_uint64high32, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_uint64 = engine->toStringHandle(QStringLiteral("uint64"));
    mIterableProperties.append(qMakePair(s_uint64, QScriptValue::ReadOnly | QScriptValue::Undeletable));

    mPrimitivePrototype = engine->newObject();
    mPrimitivePrototype.setProperty(QStringLiteral("toString"), engine->newFunction(Primitive_proto_toString));
}

PrimitiveScriptClass::~PrimitiveScriptClass() = default;

bool PrimitiveScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint*)
{
    Q_UNUSED(data)
    // TODO assign ids to improve speed? cant have children, so it won't conflict
    // no need to modify flags since both read and write are handled
    if (name == s_value || name == s_type) {
        *flags &= ~HandlesWriteAccess;
        return true;
    }
    if (name == s_bool || name == s_char || name == s_int || name == s_uint || name == s_float
               || name == s_double || name == s_int64 || name == s_uint64 || name == s_int64low32
               || name == s_int64high32 || name == s_uint64low32 || name == s_uint64high32 || name == s_int8
               || name == s_int16 || name == s_int32 || name == s_uint8 || name == s_uint16 || name == s_uint32) {
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

    if (name == s_value) {
        if (pData->wasAbleToRead()) {
            return pData->valueAsQScriptValue();
        }
        QScriptValue callee = engine()->currentContext()->thisObject();
        DataInformation* cause = toDataInformation(callee);
        if (cause) {
            pData->logError() << "Attempting to read from uninitialized value. Callee was " << cause->fullObjectPath();
        } else {
            pData->logError() << "Attempting to read from uninitialized value. Callee could not be determined";
        }
        return engine()->undefinedValue();
    }
    if (name == s_type) {
        // bitfields are handled by own scriptclass and NotPrimitive indicates an error
        Q_ASSERT(!pData->isBitfield());
        Q_ASSERT(pData->type() != PrimitiveDataType::Invalid);
        return PrimitiveType::standardTypeName(pData->type());
    }

    AllPrimitiveTypes value = pData->value();
    if (name == s_bool) {
        return value.value<quint64>() != 0;
    }
    if (name == s_char) {
        return QString(value.value<quint8>() > 127 ? QChar::ReplacementCharacter : QChar(value.value<qint8>(), 0));
    }
    if (name == s_float) {
        return value.value<float>();
    }
    if (name == s_double) {
        return value.value<double>();
    }
    if (name == s_int || name == s_int32 || name == s_int64low32) {
        return value.value<qint32>();
    }
    if (name == s_uint || name == s_uint32 || name == s_uint64low32) {
        return value.value<quint32>();
    }
    if (name == s_int64) {
        return QString::number(value.value<qint64>());
    }
    if (name == s_uint64) {
        return QString::number(value.value<quint64>());
    }
    if (name == s_int64high32) {
        return qint32(value.value<qint64>() >> 32);
    }
    if (name == s_uint64high32) {
        return quint32(value.value<quint64>() >> 32);
    }
    if (name == s_int8) {
        return qint32(value.value<qint8>());
    }
    if (name == s_int16) {
        return qint32(value.value<qint16>());
    }
    if (name == s_uint8) {
        return quint32(value.value<quint8>());
    }
    if (name == s_uint16) {
        return quint32(value.value<quint16>());
    }

    return {};
}

bool PrimitiveScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    Q_UNUSED(value);
    Q_UNUSED(data);
    Q_UNUSED(name);
    // TODO allow changing type
    return false;
}

QScriptValue PrimitiveScriptClass::prototype() const
{
    return mPrimitivePrototype;
}

QScriptValue PrimitiveScriptClass::Primitive_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = toDataInformation(ctx->thisObject());
    if (!data) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast data";
        return eng->undefinedValue();
    }
    // this might allow proper comparison between values without having to call .value
    return data->wasAbleToRead() ? data->valueString() : eng->undefinedValue();
}
