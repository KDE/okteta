/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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

#include "enumdefinition.h"

#include <QScriptValue>
#include <QScriptValueIterator>
#include <limits>

#include "../../script/scriptlogger.h"

QMap<AllPrimitiveTypes, QString> EnumDefinition::parseEnumValues(const QScriptValue& val,
        const LoggerWithContext& logger, PrimitiveDataType type)
{
    QMap<AllPrimitiveTypes, QString> enumValues;

    QScriptValueIterator it(val);
    while (it.hasNext())
    {
        it.next();
        QScriptValue val = it.value();
        QPair<AllPrimitiveTypes, QString> conv;
        if (val.isNumber())
        {
            double num = val.toNumber();
            conv = convertToEnumEntry(it.name(), num, logger, type);
        }
        else
        {
            QString numStr = val.toString();
            conv = convertToEnumEntry(it.name(), numStr, logger, type);
        }
        if (conv == QPair<AllPrimitiveTypes, QString>())
            continue;
        enumValues.insert(conv.first, conv.second);
    }
    return enumValues;
}

QPair<AllPrimitiveTypes, QString> EnumDefinition::convertToEnumEntry(const QString& name,
        const QVariant& value, const LoggerWithContext& logger, PrimitiveDataType type)
{
    Q_ASSERT(!name.isEmpty());
    //name must not be empty, else default constructed return would be valid!
    quint64 maxValue = 0.0;
    qint64 minValue;
    switch (type.value)
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
        logger.warn() << type << "is an invalid type for an enumeration, no values were parsed";
        return QPair<AllPrimitiveTypes, QString>();
    }

    AllPrimitiveTypes intValue;
    if (value.type() == QVariant::Double)
    {
        const double num = value.toDouble();
        //this is the largest double which maps to an integer exactly, ...993 is not representable anymore
        //...992 would still be representable, however it may be that ...993 was rounded down to that, be safe
        //and force the user to write such large numbers as strings
        if (num <= 9007199254740991.0)
        {
            intValue = qint64(num);
        }
        else
        {
            logger.warn() << "The value" << num << "in enum" << name
                    << " is larger than the biggest double value that can represent"
                            " any smaller integer exactly, skipping it.\n"
                            "Write the value as a string so it can be converted"
                            "to an integer exactly.";
            return QPair<AllPrimitiveTypes, QString>();
        }
    }
    else
    {
        const QString valueString = value.toString();
        bool ok = false;
        if (valueString.startsWith(QLatin1String("0x")))
        {
            intValue = valueString.mid(2).toULongLong(&ok, 16);
        }
        else
        {
            if (type == Type_UInt64 || type == Type_Bool64)
                intValue = valueString.toULongLong(&ok, 10);
            else
                intValue = valueString.toLongLong(&ok, 10);
        }
        if (!ok)
        {
            QString errMessage = QString(QLatin1String("Could not convert '%1' to an enum "
                    "constant, name was: %2")).arg(valueString, name);
            logger.warn() << errMessage;
            return QPair<AllPrimitiveTypes, QString>();
        }
    }
    quint64 asUnsigned = intValue.ulongValue;
    if (asUnsigned > maxValue)
    {
        QString errMessage = QLatin1String("Enumerator %1: %2 is larger than the maximum "
                "possible for type %3 (%4)");
        errMessage = errMessage.arg(name, QString::number(asUnsigned),
                PrimitiveType::standardTypeName(type), QString::number(maxValue));
        logger.warn() << errMessage;
        return QPair<AllPrimitiveTypes, QString>();
    }
    qint64 asSigned = intValue.longValue;
    if (minValue != 0 && asSigned < minValue)
    {
        QString errMessage = QLatin1String("Enumerator %1: %2 is smaller than the minimum "
                "possible for type %3 (%4)");
        errMessage = errMessage.arg(name, QString::number(asSigned),
                PrimitiveType::standardTypeName(type), QString::number(minValue));
        logger.warn() << errMessage;
        return QPair<AllPrimitiveTypes, QString>();
    }
    return QPair<AllPrimitiveTypes, QString>(intValue, name);

}
