/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCOMBOBOXCODINGCONFIGENTRY_HPP
#define KASTEN_BYTEARRAYCOMBOBOXCODINGCONFIGENTRY_HPP

// lib
#include "bytearraycodingconfigentry.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>

template <>
Okteta::ByteArrayComboBox::Coding
inline KConfigGroup::readEntry(const char *key,
                               const Okteta::ByteArrayComboBox::Coding &defaultValue) const
{
    return static_cast<Okteta::ByteArrayComboBox::Coding>(KConfigGroup::readEntry(key, static_cast<Kasten::ByteArrayCoding>(defaultValue)));
}


template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Okteta::ByteArrayComboBox::Coding &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    writeEntry(key, static_cast<Kasten::ByteArrayCoding>(value), flags);
}

#endif
