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

 
#ifndef KHE_H
#define KHE_H

// here we collect all general data
// let's see how much it gets ;)

namespace KHE
{
  enum KCoding { HexadecimalCoding=0, DecimalCoding, OctalCoding, BinaryCoding, NoCoding };
  static const int NoOfCodings = 5; 
}

#endif
