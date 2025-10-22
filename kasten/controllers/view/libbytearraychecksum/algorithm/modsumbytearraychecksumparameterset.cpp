/*
    SPDX-FileCopyrightText: 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modsumbytearraychecksumparameterset.hpp"

// libconfigentries
#include <endiannessconfigentry.hpp>
// KF
#include <KConfigGroup>

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
