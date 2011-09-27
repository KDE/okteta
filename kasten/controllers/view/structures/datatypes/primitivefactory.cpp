/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "primitivefactory.h"
#include "primitive/primitivetemplateinfo.h"

PrimitiveDataType PrimitiveFactory::typeStringToType(QString& string)
{
    QString typeStr = string.trimmed().toLower();
    if (typeStr == QLatin1String("bool8"))
        return Type_Bool8;
    if (typeStr == QLatin1String("bool16"))
        return Type_Bool16;
    if (typeStr == QLatin1String("bool32"))
        return Type_Bool32;
    if (typeStr == QLatin1String("bool64"))
        return Type_Bool64;
    if (typeStr == QLatin1String("int8"))
        return Type_Int8;
    if (typeStr == QLatin1String("uint8"))
        return Type_UInt8;
    if (typeStr == QLatin1String("int16"))
        return Type_Int16;
    if (typeStr == QLatin1String("uint16"))
        return Type_UInt16;
    if (typeStr == QLatin1String("int32"))
        return Type_Int32;
    if (typeStr == QLatin1String("uint32"))
        return Type_UInt32;
    if (typeStr == QLatin1String("int64"))
        return Type_Int64;
    if (typeStr == QLatin1String("uint64"))
        return Type_UInt64;
    if (typeStr == QLatin1String("char"))
        return Type_Char;
    if (typeStr == QLatin1String("float"))
        return Type_Float;
    if (typeStr == QLatin1String("double"))
        return Type_Double;

    kWarning() << "PrimitiveDataInformation::typeStringToType(): could not find"
        " correct value (typeStr=" << typeStr << ")";
    return Type_NotPrimitive; //just return a default value
}
PrimitiveDataInformation* PrimitiveFactory::newInstance(QString name,
        PrimitiveDataType type, DataInformation* parent)
{
    switch (type)
    {
    case Type_Char:
        return new PrimitiveInfo<Type_Char>::Class(name, parent);
    case Type_Int8:
        return new PrimitiveInfo<Type_Int8>::Class(name, parent);
    case Type_Int16:
        return new PrimitiveInfo<Type_Int16>::Class(name, parent);
    case Type_Int32:
        return new PrimitiveInfo<Type_Int32>::Class(name, parent);
    case Type_Int64:
        return new PrimitiveInfo<Type_Int64>::Class(name, parent);
    case Type_UInt8:
        return new PrimitiveInfo<Type_UInt8>::Class(name, parent);
    case Type_UInt16:
        return new PrimitiveInfo<Type_UInt16>::Class(name, parent);
    case Type_UInt32:
        return new PrimitiveInfo<Type_UInt32>::Class(name, parent);
    case Type_UInt64:
        return new PrimitiveInfo<Type_UInt64>::Class(name, parent);
    case Type_Bool8:
        return new PrimitiveInfo<Type_Bool8>::Class(name, parent);
    case Type_Bool16:
        return new PrimitiveInfo<Type_Bool16>::Class(name, parent);
    case Type_Bool32:
        return new PrimitiveInfo<Type_Bool32>::Class(name, parent);
    case Type_Bool64:
        return new PrimitiveInfo<Type_Bool64>::Class(name, parent);
    case Type_Float:
        return new PrimitiveInfo<Type_Float>::Class(name, parent);
    case Type_Double:
        return new PrimitiveInfo<Type_Double>::Class(name, parent);
    default:
        return NULL;
    }
}
PrimitiveDataInformation* PrimitiveFactory::newInstance(QString name,
        QString typeName, DataInformation* parent)
{
    PrimitiveDataType type = typeStringToType(typeName);
    if (type == Type_NotPrimitive) {
        kDebug() << "could not convert " << typeName << " to a primitive type";
        return NULL; //invalid type
    }
    return newInstance(name, type, parent);
}
