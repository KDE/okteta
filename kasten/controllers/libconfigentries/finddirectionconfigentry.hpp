/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FINDDIRECTIONCONFIGENTRY_HPP
#define KASTEN_FINDDIRECTIONCONFIGENTRY_HPP

// lib
#include "directionconfigentry.hpp"
// libfinddialog
#include <finddirection.hpp>

template <>
[[nodiscard]]
inline Kasten::FindDirection KConfigGroup::readEntry(const char *key, const Kasten::FindDirection &defaultValue) const
{
    return static_cast<Kasten::FindDirection>(KConfigGroup::readEntry(key, static_cast<Kasten::Direction>(defaultValue)));
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Kasten::FindDirection &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    writeEntry(key, static_cast<Kasten::Direction>(value), flags);
}

#endif
