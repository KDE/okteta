/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modsumbytearraychecksumparameterset.hpp"

ModSumByteArrayChecksumParameterSet::ModSumByteArrayChecksumParameterSet() = default;

ModSumByteArrayChecksumParameterSet::~ModSumByteArrayChecksumParameterSet() = default;

const char* ModSumByteArrayChecksumParameterSet::id() const { return "ModSum"; }

Endianness ModSumByteArrayChecksumParameterSet::endianness() const { return mEndianness; }

void ModSumByteArrayChecksumParameterSet::setEndianness(Endianness endianness) { mEndianness = endianness; }
