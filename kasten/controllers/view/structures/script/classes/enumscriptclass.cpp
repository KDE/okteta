/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "enumscriptclass.hpp"

#include "../../datatypes/primitive/enumdatainformation.hpp"
#include "../../parsers/parserutils.hpp"
#include "../scriptlogger.hpp"

EnumScriptClass::EnumScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : PrimitiveScriptClass(engine, handlerInfo)
{
    s_values = engine->toStringHandle(ParserStrings::PROPERTY_ENUM_VALUES());
    mIterableProperties.append(qMakePair(s_values, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

EnumScriptClass::~EnumScriptClass() = default;

QScriptValue EnumScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    if (name == s_values) {
        const EnumDataInformation* pData = data->asEnum();
        QScriptValue ret = engine()->newObject();
        QMapIterator<AllPrimitiveTypes, QString> it(pData->enumValues()->values());
        while (it.hasNext()) {
            it.next();
            ret.setProperty(it.value(), QString::number(it.key().value<quint64>())); // should always work
        }
        return ret;
    }
    if (name == s_type) {
        return data->typeName();
    }
    return PrimitiveScriptClass::additionalProperty(data, name, id);
}

bool EnumScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    if (name == s_values) {
        *flags = QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;
        return true;
    }
    return PrimitiveScriptClass::queryAdditionalProperty(data, name, flags, id);
}

bool EnumScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value)
{
    if (name == s_values) {
        EnumDataInformation* pData = data->asEnum();
        QMap<AllPrimitiveTypes, QString> newValues = EnumDefinition::parseEnumValues(value,
                                                                                     LoggerWithContext(pData->logger(), pData->fullObjectPath()), pData->type());
        if (newValues.isEmpty()) {
            pData->logWarn() << "attempting to set empty list of enum values!";
        }
        pData->setEnumValues(newValues);
        return true;
    }
    return PrimitiveScriptClass::setAdditionalProperty(data, name, id, value);
}
