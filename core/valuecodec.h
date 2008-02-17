/***************************************************************************
                          valuecodec.h  -  description
                             -------------------
    begin                : Sam Mai 17 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_VALUECODEC_H
#define KHE_CORE_VALUECODEC_H


// lib
#include "khe.h"
#include "khe_export.h"

class QString;


namespace KHECore
{

/** class that is able to convert codings to and from
  * hexadecimal, decimal, octal, and binary
  *
  * the buffer will be always filled up to CodingWidth, if not using shortCodingFunction
  * a closing '\0' will be always added
  *
  *@author Friedrich W. H. Kossebau
  */

class KHECORE_EXPORT ValueCodec
{
  public:
    static ValueCodec *createCodec( KCoding coding );

  public:
    virtual ~ValueCodec();

  protected:
    ValueCodec();

  public: // API to be implemented
    /** */
    virtual unsigned int encodingWidth() const = 0;
    /** */
    virtual unsigned char digitsFilledLimit() const = 0;

    /** encodes the byte and writes the result to */
    virtual void encode( QString &digits, unsigned int pos, const unsigned char byte ) const = 0;
    /** */
    virtual void encodeShort( QString &digits, unsigned int pos, const unsigned char byte ) const = 0;

    /** */
    virtual bool appendDigit( unsigned char *byte, const unsigned char digit ) const = 0;
    /** */
    virtual void removeLastDigit( unsigned char *byte ) const = 0;
    /** */
    virtual bool isValidDigit( const unsigned char digit ) const = 0;
    /** */
    virtual bool turnToValue( unsigned char *digit ) const = 0;


  public:
    /** */
    unsigned int decode( unsigned char *byte, const QString &digits, unsigned int pos ) const;
};

inline ValueCodec::ValueCodec() {}

}

#endif
