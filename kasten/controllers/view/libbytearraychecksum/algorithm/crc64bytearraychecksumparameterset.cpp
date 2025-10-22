/*
    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "crc64bytearraychecksumparameterset.hpp"

// KF
#include <KConfigGroup>

template <>
inline Crc64ByteArrayChecksumParameterSet::Variant KConfigGroup::readEntry(const char *key,
                                                                           const Crc64ByteArrayChecksumParameterSet::Variant &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Crc64ByteArrayChecksumParameterSet::Variant variant =
        (entry == QLatin1String("ECMA182")) ? Crc64ByteArrayChecksumParameterSet::Variant::ECMA182 :
        (entry == QLatin1String("ISO3309")) ? Crc64ByteArrayChecksumParameterSet::Variant::ISO3309 :
        /* else */                            defaultValue;
    return variant;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Crc64ByteArrayChecksumParameterSet::Variant &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Crc64ByteArrayChecksumParameterSet::Variant::ECMA182) ? QLatin1String("ECMA182") : QLatin1String("ISO3309");
    writeEntry(key, valueString, flags);
}

Crc64ByteArrayChecksumParameterSet::Crc64ByteArrayChecksumParameterSet() = default;

Crc64ByteArrayChecksumParameterSet::~Crc64ByteArrayChecksumParameterSet() = default;

const char* Crc64ByteArrayChecksumParameterSet::id() const { return "Crc64"; }

Crc64ByteArrayChecksumParameterSet::Variant Crc64ByteArrayChecksumParameterSet::variant() const { return mVariant; }

void Crc64ByteArrayChecksumParameterSet::setVariant(Variant variant) { mVariant = variant; }

void Crc64ByteArrayChecksumParameterSet::loadConfig(const KConfigGroup& configGroup)
{
    mVariant = configGroup.readEntry(VariantConfigKey, DefaultVariant);
}

void Crc64ByteArrayChecksumParameterSet::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(VariantConfigKey, mVariant);
}
