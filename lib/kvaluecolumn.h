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

// lib specific
#include "kbytecodec.h"
#include "kbuffercolumn.h"


namespace KHE
{

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
    void paintEditedByte( QPainter *P, char Byte, const QString &EditBuffer );

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
    KPixelX binaryGapWidth()      const;
    KCoding coding()              const;
    const KByteCodec *byteCodec() const;


  protected: // KBufferColumn API
    virtual void drawByte( QPainter *P, char Byte, KHEChar B, const QColor &Color ) const;
    virtual void recalcByteWidth();

  protected:
    void drawCode( QPainter *P, const QString &Code, const QColor &Color ) const;

  protected: // set data
    /** */
    KCoding Coding;
    /** */
    KByteCodec *ByteCodec;
    /** */
    KPixelX BinaryGapWidth;

  protected: // buffered data
    /** buffer to hold the formatted coding */
    mutable QString CodedByte;
    /** calculated: Offset in pixels of the second half of the binary */
    KPixelX BinaryHalfOffset;
};


inline KPixelX KValueColumn::binaryGapWidth()        const { return BinaryGapWidth; }
inline KCoding KValueColumn::coding()                const { return Coding; }
inline const KByteCodec *KValueColumn::byteCodec()   const { return ByteCodec; }

}

#endif
