/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycodingconfigentry.hpp"

// Std
#include <algorithm>
#include <array>
#include <iterator>

// Matching Kasten::ByteArrayCoding
static constexpr std::size_t ByteArrayCodingCount = 6;
static const std::array<QString, ByteArrayCodingCount> byteArrayCodingConfigValueList = {
    QStringLiteral("Hexadecimal"),
    QStringLiteral("Decimal"),
    QStringLiteral("Octal"),
    QStringLiteral("Binary"),
    QStringLiteral("Char"),
    QStringLiteral("UTF-8"),
};

template <>
Kasten::ByteArrayCoding
KConfigGroup::readEntry(const char *key,
                        const Kasten::ByteArrayCoding &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(byteArrayCodingConfigValueList.cbegin(), byteArrayCodingConfigValueList.cend(), entry);
    if (it == byteArrayCodingConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(byteArrayCodingConfigValueList.cbegin(), it);
    return static_cast<Kasten::ByteArrayCoding>(listIndex);
}

template <>
void KConfigGroup::writeEntry(const char *key,
                              const Kasten::ByteArrayCoding &value,
                              KConfigBase::WriteConfigFlags flags)
{
    QString configValue;
    if (value == Kasten::ByteArrayInvalidCoding) {
        configValue = QStringLiteral("Invalid");
    } else {
        const int listIndex = static_cast<int>(value);
        configValue = byteArrayCodingConfigValueList[listIndex];
    }
    writeEntry(key, configValue, flags);
}
