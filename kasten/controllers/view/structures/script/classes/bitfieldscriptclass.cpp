/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bitfieldscriptclass.hpp"
#include "../../datatypes/primitive/bitfield/abstractbitfielddatainformation.hpp"
#include "../../parsers/parserutils.hpp"

BitfieldScriptClass::BitfieldScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : PrimitiveScriptClass(engine, handlerInfo)
{
    s_width = engine->toStringHandle(ParserStrings::PROPERTY_WIDTH());
    mIterableProperties.append(qMakePair(s_width, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

BitfieldScriptClass::~BitfieldScriptClass() = default;

QScriptValue BitfieldScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    const AbstractBitfieldDataInformation* pData = data->asBitfield();
    if (name == s_width) {
        return pData->width();
    }
    if (name == s_type) {
        return pData->typeName();
    }
    return PrimitiveScriptClass::additionalProperty(data, name, id);
}

bool BitfieldScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    if (name == s_width) {
        *flags = QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;
        return true;
    }
    return PrimitiveScriptClass::queryAdditionalProperty(data, name, flags, id);
}

bool BitfieldScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value)
{
    if (name == s_width) {
        if (!value.isNumber()) {
            engine()->currentContext()->throwError(QScriptContext::TypeError, QStringLiteral("bitfield.width must be an integer!"));
            return true;
        }
        BitCount32 width = value.toUInt32();
        if (width <= 0 || width > 64) {
            engine()->currentContext()->throwError(QScriptContext::RangeError,
                                                   QStringLiteral("bitfield.width must be between 1 and 64! Given: %1").arg(width));
            return true;
        }
        AbstractBitfieldDataInformation* pData = data->asBitfield();
        pData->setWidth(width);
        return true;
    }
    return PrimitiveScriptClass::setAdditionalProperty(data, name, id, value);
}
