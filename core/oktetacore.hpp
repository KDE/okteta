/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETACORE_HPP
#define OKTETACORE_HPP

// here we collect all general data
// let's see how much it gets ;)

namespace Okteta {

enum ValueCoding
{
    HexadecimalCoding = 0,
    DecimalCoding = 1,
    OctalCoding = 2,
    BinaryCoding = 3,
    InvalidCoding = 0xFFFE,
    MaxCodingId = 0xFFFF
};
static constexpr int NoOfCodings = 4;

}

#endif
