/***************************************************************************
                          kvaluecolumn.h  -  description
                             -------------------
    begin                : Mit Sep 3 2003
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


#ifndef KHE_KVALUECOLUMN_H
#define KHE_KVALUECOLUMN_H

#include "kbytecodec.h"
#include "kbuffercolumn.h"


namespace KHE
{

// class KValueEdit;
class KBufferRanges;


/** buffer column which displays the numerical values of the bytes
  *
  *@author Friedrich W. H. Kossebau
  */
class KValueColumn : public KBufferColumn
{
  public:
    KValueColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R );
    virtual ~KValueColumn();

  public:
    void paintEditedByte( QPainter *P, char Byte, const char *EditBuffer );

  public: // modification access
    /**
      * returns true if there was a change
      */
    bool setCoding( KCoding C );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    bool setBinaryGapWidth( KPixelX BGW );


  public: // value access
    KPixelX binaryGapWidth()                 const;
    KCoding coding()                         const;
    int codingWidth()                        const;

    KByteCodec::coding codingFunction()              const;
    KByteCodec::appending appendingFunction()        const;
    KByteCodec::removingLastDigit removingFunction() const;

  public: // service
    /** returns true if there cannot be any digit appended to Value in the actual coding */
    bool digitsFilled( unsigned char Value ) const;

  protected: // KBufferColumn API
    virtual void drawByte( QPainter *P, char Byte, KHEChar B, const QColor &Color ) const;
    virtual void recalcByteWidth();

  protected:
    void drawCode( QPainter *P, const char *Code, const QColor &Color ) const;


  protected:
    /** buffer to hold the formatted coding */
    mutable char CodedByte[KByteCodec::MaxCodingWidth+1];

  protected:  // individual data
    /***/
    KCoding Coding;
    /** buffers coding width */
    int CodingWidth;
    unsigned char DigitsFilledLimit;
    KByteCodec::coding CodingFunction;
    KByteCodec::appending AppendingFunction;
    KByteCodec::removingLastDigit RemovingLastDigitFunction;

    /** */
    KPixelX BinaryGapWidth;
    /** calculated: Offset in pixels of the second half of the binary */
    KPixelX BinaryHalfOffset;
};


inline KPixelX KValueColumn::binaryGapWidth()             const { return BinaryGapWidth; }
inline int KValueColumn::codingWidth()                    const { return CodingWidth; }
inline bool KValueColumn::digitsFilled( unsigned char V ) const { return V >= DigitsFilledLimit; }
inline KCoding KValueColumn::coding()                     const { return Coding; }

inline KByteCodec::coding KValueColumn::codingFunction()              const { return CodingFunction; }
inline KByteCodec::appending KValueColumn::appendingFunction()        const { return AppendingFunction; }
inline KByteCodec::removingLastDigit KValueColumn::removingFunction() const { return RemovingLastDigitFunction; }

}

#endif
