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

#include "pointerscriptclass.h"

#include "../../datatypes/primitive/pointerdatainformation.h"
#include "../../parsers/parserutils.h"
#include "../../parsers/scriptvalueconverter.h"

PointerScriptClass::PointerScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
        : PrimitiveScriptClass(engine, handlerInfo)
{
    s_type = engine->toStringHandle(ParserStrings::PROPERTY_TYPE);
    mIterableProperties.append(qMakePair(s_type, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    s_target = engine->toStringHandle(ParserStrings::PROPERTY_TARGET);
    mIterableProperties.append(qMakePair(s_type, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

PointerScriptClass::~PointerScriptClass()
{
}

QScriptValue PointerScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name,
        uint id)
{
    Q_ASSERT(data->isPointer());
    if (name == s_type)
    {
        return PrimitiveType::standardTypeName(data->asPointer()->pointerType()->type());
    }
    else if (name == s_target)
    {
        return data->asPointer()->pointerTarget()->toScriptValue(engine(), mHandlerInfo);
    }
    return PrimitiveScriptClass::additionalProperty(data, name, id);
}

bool PointerScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name,
        QScriptClass::QueryFlags* flags, uint* id)
{
    if (name == s_type || name == s_target)
    {
        *flags = QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;
        return true;
    }
    return PrimitiveScriptClass::queryAdditionalProperty(data, name, flags, id);
}

bool PointerScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id,
        const QScriptValue& value)
{
    if (name == s_type)
    {
        DataInformation* newType = ScriptValueConverter::convert(value, QLatin1String("(pointer value)"),
                data->logger(), data);
        if (!newType)
            data->logError() << "Could not set new pointer type.";
        else if (!data->asPointer()->setPointerType(newType))
            delete newType;

        return true;
    }
    if (name == s_target)
    {
        DataInformation* newTarget = ScriptValueConverter::convert(value, QLatin1String("(pointer value)"),
                data->logger(), data);
        if (!newTarget)
            data->logError() << "Could not set new pointer target.";
        else
            data->asPointer()->setPointerTarget(newTarget);
        return true;
    }
    return PrimitiveScriptClass::setAdditionalProperty(data, name, id, value);
}

