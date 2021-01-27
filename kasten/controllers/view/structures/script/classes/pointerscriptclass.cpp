/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "pointerscriptclass.hpp"

#include "../../datatypes/primitive/pointerdatainformation.hpp"
#include "../../parsers/parserutils.hpp"
#include "../../parsers/scriptvalueconverter.hpp"

PointerScriptClass::PointerScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : PrimitiveScriptClass(engine, handlerInfo)
{
    s_type = engine->toStringHandle(ParserStrings::PROPERTY_TYPE());
    mIterableProperties.append(qMakePair(s_type, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_target = engine->toStringHandle(ParserStrings::PROPERTY_TARGET());
    mIterableProperties.append(qMakePair(s_target, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_scale = engine->toStringHandle(ParserStrings::PROPERTY_SCALE());
    mIterableProperties.append(qMakePair(s_scale, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_interpretFunc = engine->toStringHandle(ParserStrings::PROPERTY_INTERPRET_FUNC());
    mIterableProperties.append(qMakePair(s_interpretFunc, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

PointerScriptClass::~PointerScriptClass() = default;

QScriptValue PointerScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name,
                                                    uint id)
{
    Q_ASSERT(data->isPointer());
    if (name == s_type) {
        return PrimitiveType::standardTypeName(data->asPointer()->pointerType()->type());
    }
    if (name == s_target) {
        return data->asPointer()->pointerTarget()->toScriptValue(engine(), mHandlerInfo);
    }
    if (name == s_scale) {
        return engine()->toScriptValue(data->asPointer()->pointerScale());
    }
    if (name == s_interpretFunc) {
        return data->asPointer()->interpreterFunction();
    }
    return PrimitiveScriptClass::additionalProperty(data, name, id);
}

bool PointerScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name,
                                                 QScriptClass::QueryFlags* flags, uint* id)
{
    if (name == s_type || name == s_target || name == s_scale || name == s_interpretFunc) {
        *flags = QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;
        return true;
    }
    return PrimitiveScriptClass::queryAdditionalProperty(data, name, flags, id);
}

bool PointerScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id,
                                               const QScriptValue& value)
{
    if (name == s_type) {
        DataInformation* newType = ScriptValueConverter::convert(value, QStringLiteral("(pointer value)"),
                                                                 data->logger(), data);
        if (!newType) {
            data->logError() << "Could not set new pointer type.";
        } else if (!data->asPointer()->setPointerType(newType)) {
            delete newType;
        }

        return true;
    }
    if (name == s_target) {
        DataInformation* newTarget = ScriptValueConverter::convert(value, QStringLiteral("(pointer value)"),
                                                                   data->logger(), data);
        if (!newTarget) {
            data->logError() << "Could not set new pointer target.";
        } else {
            data->asPointer()->setPointerTarget(newTarget);
        }
        return true;
    }
    if (name == s_scale) {
        if (!value.isNumber()) {
            data->logError() << "Could not set new pointer scale.";
        } else {
            data->asPointer()->setPointerScale(value.toInteger());
        }
        return true;
    }
    if (name == s_interpretFunc) {
        if (!value.isFunction()) {
            data->logError() << "Could not set new pointer interpreter function.";
        } else {
            data->asPointer()->setInterpreterFunction(value);
        }
        return true;
    }
    return PrimitiveScriptClass::setAdditionalProperty(data, name, id, value);
}
