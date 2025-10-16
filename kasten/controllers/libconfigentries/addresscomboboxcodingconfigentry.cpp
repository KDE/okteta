/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "addresscomboboxcodingconfigentry.hpp"

// Std
#include <algorithm>
#include <array>
#include <iterator>

// Matching Okteta::AddressComboBox::Coding
static constexpr std::size_t AddressCcodingCount = 4;
static const std::array<QString, AddressCcodingCount> addressCodingConfigValueList = {
    QStringLiteral("Hexadecimal"),
    QStringLiteral("Decimal"),
    QStringLiteral("Octal"),
    QStringLiteral("Expression"),
};

template <>
Okteta::AddressComboBox::Coding
KConfigGroup::readEntry(const char *key,
                        const Okteta::AddressComboBox::Coding &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    const auto it = std::find(addressCodingConfigValueList.cbegin(), addressCodingConfigValueList.cend(), entry);
    if (it == addressCodingConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(addressCodingConfigValueList.cbegin(), it);
    return static_cast<Okteta::AddressComboBox::Coding>(listIndex);
}

template <>
void KConfigGroup::writeEntry(const char *key,
                              const Okteta::AddressComboBox::Coding &value,
                              KConfigBase::WriteConfigFlags flags)
{
    QString configValue;
    if (value == Okteta::AddressComboBox::InvalidCoding) {
        configValue = QStringLiteral("Invalid");
    } else {
        const int listIndex = static_cast<int>(value);
        configValue = addressCodingConfigValueList[listIndex];
    }
    writeEntry(key, configValue, flags);
}
