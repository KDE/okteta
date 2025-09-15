/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CASESENSITIVITYCONFIGENTRY_HPP
#define KASTEN_CASESENSITIVITYCONFIGENTRY_HPP

// KF
#include <KConfigGroup>
// Qt
#include <Qt>

template <>
[[nodiscard]]
inline Qt::CaseSensitivity KConfigGroup::readEntry(const char *key, const Qt::CaseSensitivity &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Qt::CaseSensitivity caseSensitivity =
        (entry == QLatin1String("Sensitive")) ?   Qt::CaseSensitive :
        (entry == QLatin1String("Insensitive")) ? Qt::CaseInsensitive :
        /* else */                                defaultValue;
    return caseSensitivity;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Qt::CaseSensitivity &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Qt::CaseSensitive) ? QLatin1String("Sensitive") : QLatin1String("Insensitive");
    writeEntry(key, valueString, flags);
}

#endif
