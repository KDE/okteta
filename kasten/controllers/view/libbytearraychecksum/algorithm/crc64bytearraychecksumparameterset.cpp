/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "crc64bytearraychecksumparameterset.hpp"

// KF
#include <KConfigGroup>

static constexpr Crc64Variant DefaultCrc64Variant = Crc64Variant::ECMA182;

static constexpr char VariantConfigKey[] = "Variant";

template <>
inline Crc64Variant KConfigGroup::readEntry(const char *key, const Crc64Variant &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Crc64Variant endianess =
        (entry == QLatin1String("ECMA182")) ? Crc64Variant::ECMA182 :
        (entry == QLatin1String("ISO3309")) ? Crc64Variant::ISO3309 :
        /* else */                            defaultValue;
    return endianess;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Crc64Variant &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Crc64Variant::ECMA182) ? QLatin1String("ECMA182") : QLatin1String("ISO3309");
    writeEntry(key, valueString, flags);
}

Crc64ByteArrayChecksumParameterSet::Crc64ByteArrayChecksumParameterSet() = default;

Crc64ByteArrayChecksumParameterSet::~Crc64ByteArrayChecksumParameterSet() = default;

const char* Crc64ByteArrayChecksumParameterSet::id() const { return "Crc64"; }

Crc64Variant Crc64ByteArrayChecksumParameterSet::crc64Variant() const { return mVariant; }

void Crc64ByteArrayChecksumParameterSet::setCrc64Variant(Crc64Variant variant) { mVariant = variant; }

void Crc64ByteArrayChecksumParameterSet::loadConfig(const KConfigGroup& configGroup)
{
    mVariant = configGroup.readEntry(VariantConfigKey, DefaultCrc64Variant);
}

void Crc64ByteArrayChecksumParameterSet::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(VariantConfigKey, mVariant);
}
