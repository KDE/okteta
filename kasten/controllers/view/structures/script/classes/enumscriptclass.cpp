/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alex Richardson <alex.richardson@gmx.de>
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


#include "enumscriptclass.h"

#include "../../datatypes/primitive/enumdatainformation.h"
#include "../../parsers/parserutils.h"
#include "../scriptlogger.h"

EnumScriptClass::EnumScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : PrimitiveScriptClass(engine, handlerInfo)
{
    s_values = engine->toStringHandle(ParserStrings::PROPERTY_ENUM_VALUES);
    mIterableProperties.append(qMakePair(s_values, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

EnumScriptClass::~EnumScriptClass()
{
}

QScriptValue EnumScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    if (name == s_values)
    {
        const EnumDataInformation* pData = data->asEnum();
        QScriptValue ret = engine()->newObject();
        QMapIterator<AllPrimitiveTypes, QString> it(pData-> enumValues()->values());
        while(it.hasNext()) {
            it.next();
            ret.setProperty(it.value(), QString::number(it.key().ulongValue)); //should always work
        }
        return ret;
    }
    else if (name == s_type)
    {
        return data->typeName();
    }
    return PrimitiveScriptClass::additionalProperty(data, name, id);
}

bool EnumScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    if (name == s_values)
    {
        *flags = QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;
        return true;
    }
    return PrimitiveScriptClass::queryAdditionalProperty(data, name, flags, id);
}

bool EnumScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value)
{
    if (name == s_values)
    {
        EnumDataInformation* pData = data->asEnum();
        QMap<AllPrimitiveTypes, QString> newValues = EnumDefinition::parseEnumValues(value,
                LoggerWithContext(pData->logger(), pData->fullObjectPath()) , pData->type());
        if (newValues.isEmpty())
            pData->logWarn() << "attempting to set empty list of enum values!";
        pData->setEnumValues(newValues);
        return true;
    }
    return PrimitiveScriptClass::setAdditionalProperty(data, name, id, value);
}

