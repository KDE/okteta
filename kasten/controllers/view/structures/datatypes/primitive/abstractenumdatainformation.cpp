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
#include "abstractenumdatainformation.h"

#include <QtScript/QScriptValueIterator>

#include <limits>

#include "../../script/classes/enumscriptclass.h"
#include "../../script/scripthandlerinfo.h"
#include "../../script/scriptlogger.h"

AbstractEnumDataInformation::AbstractEnumDataInformation(QString name, EnumDefinition::Ptr enumDef,
        DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mEnum(enumDef)
{
}

AbstractEnumDataInformation::AbstractEnumDataInformation(const AbstractEnumDataInformation& e)
        : PrimitiveDataInformation(e), mEnum(e.mEnum)
{
}

AbstractEnumDataInformation::~AbstractEnumDataInformation()
{
}

QMap<AllPrimitiveTypes, QString> AbstractEnumDataInformation::parseEnumValues(
        const QScriptValue& val, ScriptLogger* logger, PrimitiveDataType type)
{
    QMap<AllPrimitiveTypes, QString> enumValues;
    quint64 maxValue = 0.0;
    qint64 minValue;
    switch (type)
    {
    case Type_Bitfield:
        //assume all 64 bits are used, we do not have the necessary information here
        maxValue = std::numeric_limits<quint64>::max();
        minValue = std::numeric_limits<qint64>::min();
        break;
    case Type_Bool8:
        //fall through
    case Type_UInt8:
        maxValue = std::numeric_limits<quint8>::max();
        minValue = 0;
        break;
    case Type_Bool16:
        //fall through
    case Type_UInt16:
        maxValue = std::numeric_limits<quint16>::max();
        minValue = 0;
        break;
    case Type_Bool32:
        //fall through
    case Type_UInt32:
        maxValue = std::numeric_limits<quint32>::max();
        minValue = 0;
        break;
    case Type_Bool64:
        //fall through
    case Type_UInt64:
        maxValue = std::numeric_limits<quint64>::max();
        minValue = 0;
        break;
    case Type_Int8:
        maxValue = std::numeric_limits<qint8>::max();
        minValue = std::numeric_limits<qint8>::min();
        break;
    case Type_Int16:
        maxValue = std::numeric_limits<qint16>::max();
        minValue = std::numeric_limits<qint16>::min();
        break;
    case Type_Int32:
        maxValue = std::numeric_limits<qint32>::max();
        minValue = std::numeric_limits<qint32>::min();
        break;
    case Type_Int64:
        maxValue = std::numeric_limits<qint64>::max();
        minValue = std::numeric_limits<qint64>::min();
        break;
    default:
        logger->warn(QLatin1String("Invalid type") + PrimitiveDataInformation::typeName(type)
                + QLatin1String("for an enumeration, no values were parsed"));
        return enumValues;
    }
    QScriptValueIterator it(val);
    while (it.hasNext())
    {
        it.next();
        QScriptValue val = it.value();
        AllPrimitiveTypes value;
        if (val.isNumber())
        {
            double num = val.toNumber();
            //this is the largest double which maps to an integer exactly, ...993 is not representable anymore
            //...992 would still be representable, however it may be that ...993 was rounded down to that, be safe
            //and force the user to write such large numbers as strings
            if (num <= 9007199254740991.0)
            {
                value = qint64(num);
            }
            else
            {
                logger->warn(QLatin1String("Value in enum ") + it.name() + QLatin1String(" (")
                        + QString::number(num) + QLatin1String(" is larger than the biggest"
                                " double value that can represent any smaller integer exactly,"
                                "skipping it. Write the value as a string so it is exact."));
                continue;
            }
        }
        else
        {
            QString numStr = val.toString();
            bool ok = false;
            if (numStr.startsWith(QLatin1String("0x")))
            {
                value = numStr.mid(2).toULongLong(&ok, 16);
            }
            else
            {
                if (type == Type_UInt64 || type == Type_Bool64)
                    value = numStr.toULongLong(&ok, 10);
                else
                    value = numStr.toLongLong(&ok, 10);
            }
            if (!ok)
            {
                logger->warn(QString(QLatin1String("Could not convert %1 to an enum "
                        "constant, name was: %2")).arg(numStr, it.name()));
                continue;
            }

        }
        quint64 asUnsigned = value.ulongValue;
        if (asUnsigned > maxValue)
        {
            logger->warn(QString(QLatin1String("Enumerator %1: %2 is larger than the maximum "
                    "possible for type %3 (%4)")).arg(it.name(),
                    QString::number(asUnsigned), PrimitiveDataInformation::typeName(type),
                    QString::number(maxValue)));
            continue;
        }
        qint64 asSigned = value.longValue;
        if (minValue != 0 && asSigned < minValue)
        {
            logger->warn(QString(QLatin1String("Enumerator %1: %2 is smaller than the minimum "
                    "possible for type %3 (%4)")).arg(it.name(),
                    QString::number(asSigned), PrimitiveDataInformation::typeName(type),
                    QString::number(minValue)));
            continue;
        }
        enumValues.insert(value, it.name());
    }
    return enumValues;
}

void AbstractEnumDataInformation::setEnumValues(QMap<AllPrimitiveTypes, QString> newValues)
{
    mEnum->setValues(newValues);
}

QScriptValue AbstractEnumDataInformation::toScriptValue(QScriptEngine* engine,
        ScriptHandlerInfo* handlerInfo)
{
    QScriptValue ret = engine->newObject(handlerInfo->mEnumClass.data());
    ret.setData(engine->toScriptValue(static_cast<DataInformation*>(this)));
    return ret;
}

