/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_OFFSETFORMAT_HPP
#define OKTETA_OFFSETFORMAT_HPP

// lib
#include "oktetagui_export.hpp"

namespace Okteta {

/**
 * @author Friedrich W. H. Kossebau
 */

class OKTETAGUI_EXPORT OffsetFormat
{
public:
    /** */
    using print = void (*)(char* Buffer, unsigned int Offset);
    /** */
    enum Format
    {
        Hexadecimal = 0,
        Decimal
    };
    /** */
    static constexpr int MaxFormatWidth = 10;

public:
    OffsetFormat() = delete;
    ~OffsetFormat() = delete;

public:
    /** */
    static unsigned int codingWidth(int i);
    /** */
    static print printFunction(int i);

public:
    static void printHexadecimalOffset(char* Buffer, unsigned int Offset);
    static void printHexadecimalSmallOffset(char* Buffer, unsigned int Offset);
    static void printDecimalOffset(char* Buffer, unsigned int Offset);

private:
    /** */
    static const unsigned int CodingWidth[2]; // TODO: would sizeof(Coding} work?
    /** */
    static const print PrintFunction[2];
};

inline unsigned int OffsetFormat::codingWidth(int i)
{ return CodingWidth[i]; }

inline OffsetFormat::print OffsetFormat::printFunction(int i)
{ return PrintFunction[i]; }

}

#endif
