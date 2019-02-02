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
#ifndef PRIMITIVEDATATYPE_H_
#define PRIMITIVEDATATYPE_H_

class QDebug;
class QString;

enum class PrimitiveDataType
{
//! !! DO NOT CHANGE ORDER OF ITEMS !!!
    Invalid = -1,
    START = 0,
    Bool8 = 0,
    Int8 = 1,
    UInt8 = 2,
    Char = 3,
    Bool16 = 4,
    Int16 = 5,
    UInt16 = 6,
    Bool32 = 7,
    Int32 = 8,
    UInt32 = 9,
    Bool64 = 10,
    Int64 = 11,
    UInt64 = 12,
    Float = 13,
    Double = 14,
    Bitfield = 15,
    END = Bitfield
};

namespace PrimitiveType {

/**
 * @return either PrimitiveType::longTypename() or
 * PrimitiveType::standardTypeName() depending on settings
 */
QString typeName(PrimitiveDataType type);
/** @return the name of the type, e.g. "uint8", "double", etc. */
QString standardTypeName(PrimitiveDataType type);
/** @return the localized type name */
QString longTypeName(PrimitiveDataType type);

}

QDebug operator<<(QDebug dbg, PrimitiveDataType type);

#endif /* PRIMITIVEDATATYPE_H_ */
