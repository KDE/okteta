/***************************************************************************
                          koffsetformat.h  -  description
                             -------------------
    begin                : Mit Mai 21 2003
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


#ifndef KHE_KOFFSETFORMAT_H
#define KHE_KOFFSETFORMAT_H

namespace KHE
{

/**
  *@author Friedrich W. H. Kossebau
  */

class KOffsetFormat
{
  public:
    /** */
    typedef void (*print)( char *Buffer, unsigned int Offset );
    /** */
    enum KFormat { Hexadecimal=0, Decimal, None };
    /** */
    static const int MaxFormatWidth = 9;

  private:
    KOffsetFormat();
    ~KOffsetFormat();

  public:
    /** */
    static unsigned int codingWidth( int i );
    /** */
    static print printFunction( int i );

  public:
    static void printHexadecimalOffset( char *Buffer, unsigned int Offset );
    static void printHexadecimalSmallOffset( char *Buffer, unsigned int Offset );
    static void printDecimalOffset( char *Buffer, unsigned int Offset );

  protected:
    /** */
    static const unsigned int CodingWidth[2]; //TODO: would sizeof(Coding} work?
    /** */
    static const print PrintFunction[2];
};


inline unsigned int KOffsetFormat::codingWidth( int i )
{ return CodingWidth[i]; }

inline KOffsetFormat::print KOffsetFormat::printFunction( int i )
{ return PrintFunction[i]; }


}

#endif
