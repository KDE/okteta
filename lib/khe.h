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
    ISO8859_1Encoding=1,
//    /** don't use; not implemented: the most common EBCDIC codepage */
//    CECP1047Encoding=2,
    EBCDIC1047Encoding=2,
    /** Offset for own encodings which are bound in by plugins */
    StartOfOwnEncoding=0x8000,
    /** this should enable extension without breaking binary compatibility */
    MaxEncodingId=0xFFFF
  };
  static const int NoOfEncodings = 2;
}

#endif
