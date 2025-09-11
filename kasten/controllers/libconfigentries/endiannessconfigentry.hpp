/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2022-2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENDIANNESSCONFIGENTRY_HPP
#define KASTEN_ENDIANNESSCONFIGENTRY_HPP

// KF
#include <KConfigGroup>
// Qt
#include <QSysInfo>

template <>
[[nodiscard]]
inline QSysInfo::Endian KConfigGroup::readEntry(const char *key, const QSysInfo::Endian &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const QSysInfo::Endian endianness =
        (entry == QLatin1String("BigEndian")) ?    QSysInfo::BigEndian :
        (entry == QLatin1String("LittleEndian")) ? QSysInfo::LittleEndian :
        /* else */                                 defaultValue;
    return endianness;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const QSysInfo::Endian &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == QSysInfo::BigEndian) ? QLatin1String("BigEndian") : QLatin1String("LittleEndian");
    writeEntry(key, valueString, flags);
}

#endif
