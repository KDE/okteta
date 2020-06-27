/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENDIANNESS_HPP
#define KASTEN_ENDIANNESS_HPP

// Qt
#include <Qt>

enum Endianness
{
    LittleEndian = 0, // Intel, Alpha, ...
    BigEndian =    1 // Sun, Motorola, ...
};

static constexpr Endianness ThisMachineEndianness =
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    LittleEndian;
#else
    BigEndian;
#endif

#endif
