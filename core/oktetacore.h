/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OKTETACORE_H
#define OKTETACORE_H

// Qt
#include <QtCore/Qt>

// here we collect all general data
// let's see how much it gets ;)

namespace Okteta
{
  enum ValueCoding { HexadecimalCoding=0, DecimalCoding=1, OctalCoding=2, BinaryCoding=3,
                 InvalidCoding=0xFFFE,  MaxCodingId=0xFFFF };
  static const int NoOfCodings = 4;

  /** */
  enum CharCoding
  {
    /** the coding of your shell */
    LocalEncoding=0,
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
    StartOfOwnEncoding=0x8000,
    InvalidEncoding=0xFFFE,
    /** this should enable extension without breaking binary compatibility */
    MaxEncodingId=0xFFFF
  };

    // TODO: add PDP endianess
    enum ByteOrder
    {
        LittleEndian = 0,
        BigEndian =    1
    };
    static const ByteOrder thisMachineByteOrder =
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        LittleEndian;
#else
        BigEndian;
#endif

}

#endif
