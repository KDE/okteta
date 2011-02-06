/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
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

PrimitiveDataType PrimitiveFactory::typeStringToType(QString& string)
{
    QString typeStr = string.trimmed().toLower();
    if (typeStr == "bool8")
        return Type_Bool8;
    if (typeStr == "bool16")
        return Type_Bool16;
    if (typeStr == "bool32")
        return Type_Bool32;
    if (typeStr == "bool64")
        return Type_Bool64;
    if (typeStr == "int8")
        return Type_Int8;
    if (typeStr == "uint8")
        return Type_UInt8;
    if (typeStr == "int16")
        return Type_Int16;
    if (typeStr == "uint16")
        return Type_UInt16;
    if (typeStr == "int32")
        return Type_Int32;
    if (typeStr == "uint32")
        return Type_UInt32;
    if (typeStr == "int64")
        return Type_Int64;
    if (typeStr == "uint64")
        return Type_UInt64;
    if (typeStr == "char")
        return Type_Char;
    if (typeStr == "float")
        return Type_Float;
    if (typeStr == "double")
        return Type_Double;

    kWarning() << "PrimitiveDataInformation::typeStringToType(): could not find"
        " correct value (typeStr=" << typeStr << ")";
    return Type_NotPrimitive; //just return a default value
}

PrimitiveDataInformation* PrimitiveFactory::newInstance(QString name,
        PrimitiveDataType type, int index, DataInformation* parent)
{
    switch (type)
    {
    case Type_Char:
        return new CharDataInformation(name, parent);
    case Type_Int8:
        return new Int8DataInformation(name, parent);
    case Type_Int16:
        return new Int16DataInformation(name, parent);
    case Type_Int32:
        return new Int32DataInformation(name, parent);
    case Type_Int64:
        return new Int64DataInformation(name, parent);
    case Type_UInt8:
        return new UInt8DataInformation(name, parent);
    case Type_UInt16:
        return new UInt16DataInformation(name, parent);
    case Type_UInt32:
        return new UInt32DataInformation(name, parent);
    case Type_UInt64:
        return new UInt64DataInformation(name, parent);
    case Type_Bool8:
        return new Bool8DataInformation(name, parent);
    case Type_Bool16:
        return new Bool16DataInformation(name, parent);
    case Type_Bool32:
        return new Bool32DataInformation(name, parent);
    case Type_Bool64:
        return new Bool64DataInformation(name, parent);
    case Type_Float:
        return new FloatDataInformation(name, parent);
    case Type_Double:
        return new DoubleDataInformation(name, parent);
    default:
        return NULL;
    }
}
PrimitiveDataInformation* PrimitiveFactory::newInstance(QString name,
        QString typeName, int index, DataInformation* parent)
{
    PrimitiveDataType type = typeStringToType(typeName);
    if (type == Type_NotPrimitive)
        return NULL; //invalid type
    return newInstance(name, type, index, parent);
}
