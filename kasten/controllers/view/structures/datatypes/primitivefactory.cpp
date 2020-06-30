/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "primitivefactory.hpp"
#include "primitive/primitivetemplateinfo.hpp"
#include "../script/scriptlogger.hpp"

namespace PrimitiveFactory {

PrimitiveDataType typeStringToType(const QString& string, const LoggerWithContext& logger)
{
    const QString typeStr = string.trimmed().toLower();
    if (typeStr == QLatin1String("bool8")) {
        return PrimitiveDataType::Bool8;
    }
    if (typeStr == QLatin1String("bool16")) {
        return PrimitiveDataType::Bool16;
    }
    if (typeStr == QLatin1String("bool32")) {
        return PrimitiveDataType::Bool32;
    }
    if (typeStr == QLatin1String("bool64")) {
        return PrimitiveDataType::Bool64;
    }
    if (typeStr == QLatin1String("int8")) {
        return PrimitiveDataType::Int8;
    }
    if (typeStr == QLatin1String("uint8")) {
        return PrimitiveDataType::UInt8;
    }
    if (typeStr == QLatin1String("int16")) {
        return PrimitiveDataType::Int16;
    }
    if (typeStr == QLatin1String("uint16")) {
        return PrimitiveDataType::UInt16;
    }
    if (typeStr == QLatin1String("int32")) {
        return PrimitiveDataType::Int32;
    }
    if (typeStr == QLatin1String("uint32")) {
        return PrimitiveDataType::UInt32;
    }
    if (typeStr == QLatin1String("int64")) {
        return PrimitiveDataType::Int64;
    }
    if (typeStr == QLatin1String("uint64")) {
        return PrimitiveDataType::UInt64;
    }
    if (typeStr == QLatin1String("char")) {
        return PrimitiveDataType::Char;
    }
    if (typeStr == QLatin1String("float")) {
        return PrimitiveDataType::Float;
    }
    if (typeStr == QLatin1String("double")) {
        return PrimitiveDataType::Double;
    }
    logger.warn() << typeStr << "does not name a valid primitive type";
    return PrimitiveDataType::Invalid; // just return a default value
}
PrimitiveDataInformation* newInstance(const QString& name, PrimitiveDataType type,
                                      const LoggerWithContext& logger, DataInformation* parent)
{
    switch (type)
    {
    case PrimitiveDataType::Char:
        return new PrimitiveInfo<PrimitiveDataType::Char>::Class(name, parent);
    case PrimitiveDataType::Int8:
        return new PrimitiveInfo<PrimitiveDataType::Int8>::Class(name, parent);
    case PrimitiveDataType::Int16:
        return new PrimitiveInfo<PrimitiveDataType::Int16>::Class(name, parent);
    case PrimitiveDataType::Int32:
        return new PrimitiveInfo<PrimitiveDataType::Int32>::Class(name, parent);
    case PrimitiveDataType::Int64:
        return new PrimitiveInfo<PrimitiveDataType::Int64>::Class(name, parent);
    case PrimitiveDataType::UInt8:
        return new PrimitiveInfo<PrimitiveDataType::UInt8>::Class(name, parent);
    case PrimitiveDataType::UInt16:
        return new PrimitiveInfo<PrimitiveDataType::UInt16>::Class(name, parent);
    case PrimitiveDataType::UInt32:
        return new PrimitiveInfo<PrimitiveDataType::UInt32>::Class(name, parent);
    case PrimitiveDataType::UInt64:
        return new PrimitiveInfo<PrimitiveDataType::UInt64>::Class(name, parent);
    case PrimitiveDataType::Bool8:
        return new PrimitiveInfo<PrimitiveDataType::Bool8>::Class(name, parent);
    case PrimitiveDataType::Bool16:
        return new PrimitiveInfo<PrimitiveDataType::Bool16>::Class(name, parent);
    case PrimitiveDataType::Bool32:
        return new PrimitiveInfo<PrimitiveDataType::Bool32>::Class(name, parent);
    case PrimitiveDataType::Bool64:
        return new PrimitiveInfo<PrimitiveDataType::Bool64>::Class(name, parent);
    case PrimitiveDataType::Float:
        return new PrimitiveInfo<PrimitiveDataType::Float>::Class(name, parent);
    case PrimitiveDataType::Double:
        return new PrimitiveInfo<PrimitiveDataType::Double>::Class(name, parent);
    default:
        logger.error().nospace() << "could not convert '" << type << "' to a primitive type";
        return nullptr; // invalid type
    }
}

}
