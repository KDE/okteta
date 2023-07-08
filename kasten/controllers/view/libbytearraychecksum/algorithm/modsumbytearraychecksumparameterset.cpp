/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modsumbytearraychecksumparameterset.hpp"

// KF
#include <KConfigGroup>

template <>
inline QSysInfo::Endian KConfigGroup::readEntry(const char *key, const QSysInfo::Endian &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const QSysInfo::Endian endianess =
        (entry == QLatin1String("BigEndian")) ?    QSysInfo::BigEndian :
        (entry == QLatin1String("LittleEndian")) ? QSysInfo::LittleEndian :
        /* else */                                 defaultValue;
    return endianess;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const QSysInfo::Endian &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == QSysInfo::BigEndian) ? QLatin1String("BigEndian") : QLatin1String("LittleEndian");
    writeEntry(key, valueString, flags);
}

static constexpr QSysInfo::Endian DefaultByteOrder = QSysInfo::ByteOrder;

static constexpr char ByteOrderConfigKey[] = "ByteOrder";

ModSumByteArrayChecksumParameterSet::ModSumByteArrayChecksumParameterSet() = default;

ModSumByteArrayChecksumParameterSet::~ModSumByteArrayChecksumParameterSet() = default;

const char* ModSumByteArrayChecksumParameterSet::id() const { return "ModSum"; }

QSysInfo::Endian ModSumByteArrayChecksumParameterSet::endianness() const { return mEndianness; }

void ModSumByteArrayChecksumParameterSet::setEndianness(QSysInfo::Endian endianness) { mEndianness = endianness; }

void ModSumByteArrayChecksumParameterSet::loadConfig(const KConfigGroup& configGroup)
{
    mEndianness = configGroup.readEntry(ByteOrderConfigKey, DefaultByteOrder);
}

void ModSumByteArrayChecksumParameterSet::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(ByteOrderConfigKey, mEndianness);
}
