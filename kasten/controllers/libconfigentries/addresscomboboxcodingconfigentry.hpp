/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ADDRESSCOMBOBOXCODINGCONFIGENTRY_HPP
#define KASTEN_ADDRESSCOMBOBOXCODINGCONFIGENTRY_HPP

// KF
#include <KConfigGroup>
// Okteta Kasten gui
#include <Kasten/Okteta/AddressComboBox>

template <>
[[nodiscard]]
Okteta::AddressComboBox::Coding
KConfigGroup::readEntry(const char *key,
                        const Okteta::AddressComboBox::Coding &defaultValue) const;

template <>
void KConfigGroup::writeEntry(const char *key,
                              const Okteta::AddressComboBox::Coding &value,
                              KConfigBase::WriteConfigFlags flags);

#endif
