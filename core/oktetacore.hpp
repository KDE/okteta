/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETACORE_HPP
#define OKTETACORE_HPP

// Qt
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LOG_OKTETA_CORE)

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

/** */
enum CharCoding
{
    /** the coding of your shell */
    LocalEncoding = 0,
    /** ASCII encoding, also known as Latin1 */
    ISO8859_1Encoding,
    /**  */
    ISO8859_2Encoding,
    /**  */
    ISO8859_3Encoding,
    /**  */
    ISO8859_4Encoding,
    /**  */
    ISO8859_5Encoding,
    /**  */
    ISO8859_6Encoding,
    /**  */
    ISO8859_7Encoding,
    /**  */
    ISO8859_8Encoding,
    /**  */
    ISO8859_8_IEncoding,
    /**  */
    ISO8859_9Encoding,
    /**  */
    ISO8859_10Encoding,
    /**  */
    ISO8859_11Encoding,
    /**  */
    ISO8859_13Encoding,
    /**  */
    ISO8859_14Encoding,
    /**  */
    ISO8859_15Encoding,
    /**  */
    ISO8859_16Encoding,
    /**  */
    CP1250Encoding,
    /**  */
    CP1251Encoding,
    /**  */
    CP1252Encoding,
    /**  */
    CP1253Encoding,
    /**  */
    CP1254Encoding,
    /**  */
    CP1255Encoding,
    /**  */
    CP1256Encoding,
    /**  */
    CP1257Encoding,
    /**  */
    CP1258Encoding,
    /**  */
    IBM850Encoding,
    /**  */
    IBM866Encoding,
    /**  */
    IBM874Encoding,
    /**  */
    KOI8_REncoding,
    /**  */
    KOI8_UEncoding,
    /** the most common EBCDIC codepage */
    EBCDIC1047Encoding,
    /** Offset for own encodings which are bound in by plugins */
    StartOfOwnEncoding = 0x8000,
    InvalidEncoding = 0xFFFE,
    /** this should enable extension without breaking binary compatibility */
    MaxEncodingId = 0xFFFF
};

}

#endif
