/*
    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_OFFSETFORMAT_HPP
#define OKTETA_OFFSETFORMAT_HPP

// lib
#include "oktetagui_export.hpp"
// Std
#include <array>

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
        Decimal,
        Octal,
    };
    static constexpr auto FormatCount = static_cast<std::size_t>(Octal) + 1;
    /** */
    static constexpr int MaxFormatWidth = 11;

public:
    OffsetFormat() = delete;
    OffsetFormat(const OffsetFormat&) = delete;
    OffsetFormat(OffsetFormat&&) = delete;
    ~OffsetFormat() = delete;

    OffsetFormat& operator=(const OffsetFormat&) = delete;
    OffsetFormat& operator=(OffsetFormat&&) = delete;

public:
    /** */
    [[nodiscard]]
    static unsigned int codingWidth(Format format);
    /** */
    [[nodiscard]]
    static print printFunction(Format format);

public:
    static void printHexadecimalOffset(char* Buffer, unsigned int Offset);
    static void printHexadecimalSmallOffset(char* Buffer, unsigned int Offset);
    static void printDecimalOffset(char* Buffer, unsigned int Offset);
    static void printOctalOffset(char* Buffer, unsigned int Offset);

private:
    /** */
    static const std::array<unsigned int, FormatCount> CodingWidth;
    /** */
    static const std::array<print, FormatCount> PrintFunction;
};

inline unsigned int OffsetFormat::codingWidth(Format format)
{
    const auto index = static_cast<std::size_t>(format);
    return CodingWidth[index];
}

inline OffsetFormat::print OffsetFormat::printFunction(Format format)
{
    const auto index = static_cast<std::size_t>(format);
    return PrintFunction[index];
}

}

#endif
