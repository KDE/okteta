/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "primitivedatatype.hpp"
#include "structureviewpreferences.h"

#include <ki18n_version.h>
#if KI18N_VERSION >= QT_VERSION_CHECK(5, 89, 0)
#include <KLazyLocalizedString>
#endif
#include <KLocalizedString>
#include <QDebug>

#if KI18N_VERSION >= QT_VERSION_CHECK(5, 89, 0)
#undef I18N_NOOP2
#define I18N_NOOP2 kli18nc
#endif

#if KI18N_VERSION >= QT_VERSION_CHECK(5, 89, 0)
static constexpr const KLazyLocalizedString longTypeNames[static_cast<int>(PrimitiveDataType::END) + 1] = {
#else
static constexpr const char* longTypeNames[static_cast<int>(PrimitiveDataType::END) + 1] = {
#endif
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
#if KI18N_VERSION >= QT_VERSION_CHECK(5, 89, 0)
        return longTypeNames[static_cast<int>(type)].toString();
#else
        return i18nc("data type", longTypeNames[static_cast<int>(type)]);
#endif
    }

    return i18n("invalid type");
}

QDebug operator<<(QDebug dbg, PrimitiveDataType type)
{
    dbg.nospace() << "primitive type(" << PrimitiveType::standardTypeName(type) << ")";
    return dbg.space();
}
