/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "primitivedatatype.hpp"

// lib
#include <structureviewpreferences.hpp>
// KF
#include <KLazyLocalizedString>
#include <KLocalizedString>
// Qt
#include <QDebug>

static constexpr const KLazyLocalizedString longTypeNames[static_cast<int>(PrimitiveDataType::END) + 1] = {
    kli18nc("data type", "bool (1 byte)"),
    kli18nc("data type", "signed byte"),
    kli18nc("data type", "unsigned byte"),
    kli18nc("data type", "char"),
    kli18nc("data type", "bool (2 bytes)"),
    kli18nc("data type", "signed short"),
    kli18nc("data type", "unsigned short"),
    kli18nc("data type", "bool (4 bytes)"),
    kli18nc("data type", "signed int"),
    kli18nc("data type", "unsigned int"),
    kli18nc("data type", "bool (8 bytes)"),
    kli18nc("data type", "signed long"),
    kli18nc("data type", "unsigned long"),
    kli18nc("data type", "float"),
    kli18nc("data type", "double"),
    kli18nc("data type", "bitfield"),
};

static const QString typeNames[static_cast<int>(PrimitiveDataType::END) + 1] = {
    QStringLiteral("bool8"),
    QStringLiteral("int8"),
    QStringLiteral("uint8"),
    QStringLiteral("char"),
    QStringLiteral("bool16"),
    QStringLiteral("int16"),
    QStringLiteral("uint16"),
    QStringLiteral("bool32"),
    QStringLiteral("int32"),
    QStringLiteral("uint32"),
    QStringLiteral("bool64"),
    QStringLiteral("int64"),
    QStringLiteral("uint64"),
    QStringLiteral("float"),
    QStringLiteral("double"),
    QStringLiteral("bitfield"),
};

QString PrimitiveType::typeName(PrimitiveDataType type)
{
    if (Kasten::StructureViewPreferences::shortTypeNames()) {
        return standardTypeName(type);
    }

    return longTypeName(type);
}

QString PrimitiveType::standardTypeName(PrimitiveDataType type)
{
    if (type >= PrimitiveDataType::START && type <= PrimitiveDataType::END) {
        return typeNames[static_cast<int>(type)];
    }

    return QStringLiteral("invalid");
}

QString PrimitiveType::longTypeName(PrimitiveDataType type)
{
    if (type >= PrimitiveDataType::START && type <= PrimitiveDataType::END) {
        return longTypeNames[static_cast<int>(type)].toString();
    }

    return i18n("invalid type");
}

QDebug operator<<(QDebug dbg, PrimitiveDataType type)
{
    dbg.nospace() << "primitive type(" << PrimitiveType::standardTypeName(type) << ")";
    return dbg.space();
}
