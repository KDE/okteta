/***************************************************************************
                          kvaluecolumn.cpp  -  description
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


// qt specific
#include <qpainter.h>
// lib specific
#include "kcolumnsview.h"
#include "kbuffercursor.h"
#include "kbufferlayout.h"
#include "kbufferranges.h"
#include "kvaluecolumn.h"
#include "helper.h"
#include "kcharcodec.h"

using namespace KHE;

static const KCoding DefaultCoding =         HexadecimalCoding;
static const KCoding NotDefaultCoding =      DecimalCoding;
static const int     DefaultBinaryGapWidth = 1;


KValueColumn::KValueColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R )
 : KBufferColumn( CV, B, L, R ),
   Coding( NotDefaultCoding ),
   ByteCodec( 0 ),
   BinaryGapWidth( DefaultBinaryGapWidth )
{
  setCoding( DefaultCoding );
}


KValueColumn::~KValueColumn()
{
}



bool KValueColumn::setCoding( KCoding C )
{
  // no changes?
  if( Coding == C )
    return false;

  delete ByteCodec;

  Coding = C;
  ByteCodec = KByteCodec::createCodec( Coding );
  CodedByte.setLength( ByteCodec->encodingWidth() );

  // recalculate depend sizes
  recalcByteWidth();

  if( PosX )
    recalcX();
  return true;
}


bool KValueColumn::setBinaryGapWidth( KPixelX BGW )
{
  // no changes?
  if( BinaryGapWidth == BGW )
     return false;

  BinaryGapWidth = BGW;

  // recalculate depend sizes
  recalcByteWidth();

  if( PosX )
    recalcX();
  return true;
}


void KValueColumn::recalcByteWidth()
{
  ByteWidth = ByteCodec->encodingWidth() * DigitWidth;

  if( Coding == BinaryCoding )
  {
    BinaryHalfOffset = 4 * DigitWidth + BinaryGapWidth;
    ByteWidth += BinaryGapWidth;
  }
  recalcVerticalGridX();
}


// perhaps sometimes there will be a grammar
void KValueColumn::paintEditedByte( QPainter *P, char Byte, const QString &EditBuffer )
{
  KHEChar B = Codec->decode( Byte );

  const QColorGroup &CG = View->colorGroup();

  P->fillRect( 0,0,ByteWidth,LineHeight, QBrush(colorForChar(B),Qt::SolidPattern) );

  drawCode( P, EditBuffer, CG.base() );
}


void KValueColumn::drawByte( QPainter *P, char Byte, KHEChar /*B*/, const QColor &Color ) const
{
  ByteCodec->encode( CodedByte, 0, Byte );
  drawCode( P, CodedByte, Color );
}


void KValueColumn::drawCode( QPainter *P, const QString &Code, const QColor &Color ) const
{
  P->setPen( Color );
  if( Coding == BinaryCoding )
  {
    // leave a gap in the middle
    P->drawText( 0, DigitBaseLine, Code.left(4) );
    P->drawText( BinaryHalfOffset, DigitBaseLine, Code.right(4) );
  }
  else
    P->drawText( 0, DigitBaseLine, Code );
}
