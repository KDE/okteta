/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DIRECTIONCONFIGENTRY_HPP
#define KASTEN_DIRECTIONCONFIGENTRY_HPP

// KF
#include <KConfigGroup>

namespace Kasten {

enum Direction
{
    Forward = 0,
    Backward = 1
};

}

template <>
[[nodiscard]]
inline Kasten::Direction KConfigGroup::readEntry(const char *key, const Kasten::Direction &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Kasten::Direction direction =
        (entry == QLatin1String("Forward")) ?  Kasten::Forward :
        (entry == QLatin1String("Backward")) ? Kasten::Backward :
        /* else */                             defaultValue;
    return direction;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Kasten::Direction &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Kasten::Forward) ? QLatin1String("Forward") : QLatin1String("Backward");
    writeEntry(key, valueString, flags);
}

#endif
