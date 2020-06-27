/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRIMITIVEDATATYPE_HPP
#define KASTEN_PRIMITIVEDATATYPE_HPP

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

#endif /* KASTEN_PRIMITIVEDATATYPE_HPP */
