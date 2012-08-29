/*
 *   This file is part of the Okteta Kasten module, made within the KDE community.
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
#include "primitivedatatype.h"
#include "structviewpreferences.h"

#include <KLocalizedString>
#include <QDebug>

static const char* longTypeNames[Type_END + 1] =
        {
                I18N_NOOP2("data type", "bool (1 byte)"),
                I18N_NOOP2("data type", "signed byte"),
                I18N_NOOP2("data type", "unsigned byte"),
                I18N_NOOP2("data type", "char"),
                I18N_NOOP2("data type", "bool (2 bytes)"),
                I18N_NOOP2("data type", "signed short"),
                I18N_NOOP2("data type", "unsigned short"),
                I18N_NOOP2("data type", "bool (4 bytes)"),
                I18N_NOOP2("data type", "signed int"),
                I18N_NOOP2("data type", "unsigned int"),
                I18N_NOOP2("data type", "bool (8 bytes)"),
                I18N_NOOP2("data type", "signed long"),
                I18N_NOOP2("data type", "unsigned long"),
                I18N_NOOP2("data type", "float"),
                I18N_NOOP2("data type", "double"),
                I18N_NOOP2("data type", "bitfield"),
        };

static const char* typeNames[Type_END + 1] =
        {
                "bool8",
                "int8",
                "uint8",
                "char",
                "bool16",
                "int16",
                "uint16",
                "bool32",
                "int32",
                "uint32",
                "bool64",
                "int64",
                "uint64",
                "float",
                "double",
                "bitfield",
        };

QString PrimitiveType::typeName(PrimitiveDataType type)
{
    if (Kasten2::StructViewPreferences::shortTypeNames())
        return standardTypeName(type);
    else
        return longTypeName(type);
}

QString PrimitiveType::standardTypeName(PrimitiveDataType type_)
{
    PrimitiveDataTypeEnum type = type_.value;
    if (type >= Type_START && type <= Type_END)
        return QLatin1String(typeNames[type]);
    else
        return QLatin1String("invalid");
}

QString PrimitiveType::longTypeName(PrimitiveDataType type_)
{
    PrimitiveDataTypeEnum type = type_.value;
    if (type >= Type_START && type <= Type_END)
        return i18n(longTypeNames[type]);
    else
        return i18n("invalid type");
}

QDebug operator<<(QDebug dbg, PrimitiveDataType type)
{
    dbg.nospace() << "primitive type(" << PrimitiveType::standardTypeName(type) << ")";
    return dbg.space();
}

