/***************************************************************************
                          khe.h  -  description
                             -------------------
    begin                : Mon Jul 14 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/

 
#ifndef KHE_KHE_H
#define KHE_KHE_H

// here we collect all general data
// let's see how much it gets ;)

namespace KHE
{
  enum KCoding { HexadecimalCoding=0, DecimalCoding=1, OctalCoding=2, BinaryCoding=3, MaxCodingId=0xFFFF };
  static const int NoOfCodings = 4;
  
  /** */
  enum KEncoding
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
    ISO8859_15Encoding,
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
    KOI8_REncoding,
    /**  */
    KOI8_UEncoding,
    /** the most common EBCDIC codepage */
    EBCDIC1047Encoding,
    /** Offset for own encodings which are bound in by plugins */
    StartOfOwnEncoding=0x8000,
    /** this should enable extension without breaking binary compatibility */
    MaxEncodingId=0xFFFF
  };
}

#endif
