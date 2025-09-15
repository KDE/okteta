/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCODINGCONFIGENTRY_HPP
#define KASTEN_BYTEARRAYCODINGCONFIGENTRY_HPP

// KF
#include <KConfigGroup>

namespace Kasten {

enum ByteArrayCoding
{
    ByteArrayInvalidCoding = -1,
    ByteArrayHexadecimalCoding = 0,
    ByteArrayDecimalCoding = 1,
    ByteArrayOctalCoding = 2,
    ByteArrayBinaryCoding = 3,
    ByteArrayCharCoding = 4,
    ByteArrayUtf8Coding = 5
};

}

template <>
[[nodiscard]]
Kasten::ByteArrayCoding
KConfigGroup::readEntry(const char *key,
                        const Kasten::ByteArrayCoding &defaultValue) const;

template <>
void KConfigGroup::writeEntry(const char *key,
                              const Kasten::ByteArrayCoding &value,
                              KConfigBase::WriteConfigFlags flags);

#endif
