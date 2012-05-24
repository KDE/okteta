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
#ifndef PRIMTIVEDATATYPE_H_
#define PRIMTIVEDATATYPE_H_

class QDebug;
class QString;

enum PrimitiveDataTypeEnum
{
//!!! DO NOT CHANGE ORDER OF ITEMS !!!
    Type_Invalid = -1,
    Type_START = 0,
    Type_Bool8 = 0,
    Type_Int8 = 1,
    Type_UInt8 = 2,
    Type_Char = 3,
    Type_Bool16 = 4,
    Type_Int16 = 5,
    Type_UInt16 = 6,
    Type_Bool32 = 7,
    Type_Int32 = 8,
    Type_UInt32 = 9,
    Type_Bool64 = 10,
    Type_Int64 = 11,
    Type_UInt64 = 12,
    Type_Float = 13,
    Type_Double = 14,
    Type_Bitfield = 15,
    Type_END = Type_Bitfield
};

//a wrapper struct to prevent implicit conversion to int/bool
//TODO remove once we compile with C++11 (enum class) and rename PrimitiveDataTypeEnum again
struct PrimitiveDataType
{
    PrimitiveDataTypeEnum value;

    //allow implicit conversion
    PrimitiveDataType(PrimitiveDataTypeEnum e) : value(e) {}

    inline bool operator==(const PrimitiveDataType& other) const
    {
        return value == other.value;
    }

    inline bool operator!=(const PrimitiveDataType& other) const
    {
        return value != other.value;
    }
};

namespace PrimitiveType
{
    /** @return either PrimitiveType::longTypename() or
     * PrimitiveType::standardTypeName() depending on settings */
    QString typeName(PrimitiveDataType type);
    /** @return the name of the type, e.g. "uint8", "double", etc. */
    QString standardTypeName(PrimitiveDataType type);
    /** @return the localized type name */
    QString longTypeName(PrimitiveDataType type);
}

QDebug operator<<(QDebug dbg, PrimitiveDataType type);

#endif /* PRIMTIVEDATATYPE_H_ */
