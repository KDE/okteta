/*
    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "offsetformat.hpp"

// Std
#include <cstdio>

namespace Okteta {

const unsigned int OffsetFormat::CodingWidth[3] = { 9, 10, 11 };

const OffsetFormat::print OffsetFormat::PrintFunction[3] = {
    OffsetFormat::printHexadecimalOffset,
    OffsetFormat::printDecimalOffset,
    OffsetFormat::printOctalOffset,
};

void OffsetFormat::printHexadecimalOffset(char* Buffer, unsigned int Offset)
{
    sprintf(Buffer, "%04X:%04X", Offset >> 16, Offset & 0x0000FFFF);
}

void OffsetFormat::printHexadecimalSmallOffset(char* Buffer, unsigned int Offset)
{
    sprintf(Buffer, "%04x:%04x", Offset >> 16, Offset & 0x0000FFFF);
}

void OffsetFormat::printDecimalOffset(char* Buffer, unsigned int Offset)
{
    sprintf(Buffer, "%010u", Offset);
}

void OffsetFormat::printOctalOffset(char* Buffer, unsigned int Offset)
{
    sprintf(Buffer, "%011o", Offset);
}

}
