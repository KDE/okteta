/***************************************************************************
                          khexcolumn.h  -  description
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


#ifndef KHEXCOLUMN_H
#define KHEXCOLUMN_H

#include "kbuffercolumn.h"


namespace KHE
{

// class KHexEdit;
class KBufferRanges;


/** base class of all buffer column displayers
  * holds all information about the vertical layout of a buffer column
  * knows how to paint the data and the editing things (focus, cursor, selection)
  * but does not offer
  *
  *@author Friedrich W. H. Kossebau
  */
class KHexColumn : public KBufferColumn
{
  public:
    KHexColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R );
    virtual ~KHexColumn();

  public:
    void paintEditedByte( QPainter *P, char Byte, const char *EditBuffer );

  public: // modification access
    /**
      * returns true if there was a change
      */
    bool setCoding( KCoding C );
    /** sets the spacing in the middle of a binary byte in the hex column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    bool setBinaryGapWidth( KPixelX BGW );


  public: // value access
    KPixelX binaryGapWidth()                 const;
    KCoding coding()                         const;
    int codingWidth()                        const;
    KByteCodec::coding codingFunction()      const;
    KByteCodec::adding addingFunction()      const;
    KByteCodec::removingLastDigit removingFunction() const;

  public: // service
    /** returns true if there cannot be any digit appended to Value in the actual coding */
    bool digitsFilled( unsigned char Value ) const;

  protected: // KBufferColumn API
    void drawByte( QPainter *P, char Byte, const QColor &Color ) const;
    void recalcByteWidth();

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
    KByteCodec::adding AddingFunction;
    KByteCodec::removingLastDigit RemovingLastDigitFunction;

    /** */
    KPixelX BinaryGapWidth;
    /** calculated: Offset in pixels of the second half of the binary */
    KPixelX BinaryHalfOffset;
};


inline KPixelX KHexColumn::binaryGapWidth()             const { return BinaryGapWidth; }
inline int KHexColumn::codingWidth()                    const { return CodingWidth; }
inline bool KHexColumn::digitsFilled( unsigned char V ) const { return V >= DigitsFilledLimit; }
inline KCoding KHexColumn::coding()                     const { return Coding; }
inline KByteCodec::coding KHexColumn::codingFunction()  const { return CodingFunction; }
inline KByteCodec::adding KHexColumn::addingFunction()  const { return AddingFunction; }
inline KByteCodec::removingLastDigit KHexColumn::removingFunction()      const { return RemovingLastDigitFunction; }

}

#endif
