/***************************************************************************
                          khexcolumn.cpp  -  description
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

 
// c++ specififc
// #include <iostream>
// qt specific
#include <qpainter.h>
// app specific
#include "kcolumnsview.h"
#include "kbuffercursor.h"
#include "kbufferlayout.h"
#include "kbufferranges.h"
#include "khexcolumn.h"
#include "helper.h"

using namespace KHE;

static const KCoding DefaultCoding = HexadecimalCoding;
static const int DefaultBinaryGapWidth = 1;


KHexColumn::KHexColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R )
 : KBufferColumn( CV, B, L, R ),
   Coding( NoCoding ),
   BinaryGapWidth( DefaultBinaryGapWidth )
{
  setCoding( DefaultCoding );
}


KHexColumn::~KHexColumn()
{
}



bool KHexColumn::setCoding( KCoding C )
{
  // no changes?
  if( Coding == C )
    return false;

  Coding = C;
  CodingWidth = KByteCodec::codingWidth( Coding );
  DigitsFilledLimit = KByteCodec::digitsFilledLimit( Coding );
  CodingFunction = KByteCodec::codingFunction( Coding );
  AddingFunction = KByteCodec::addingFunction( Coding );
  RemovingLastDigitFunction = KByteCodec::removingLastDigitFunction( Coding );

  // recalculate depend sizes
  recalcByteWidth();

  if( PosX )
    recalcX();
  return true;
}


bool KHexColumn::setBinaryGapWidth( KPixelX BGW )
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


void KHexColumn::recalcByteWidth()
{
  ByteWidth = CodingWidth * DigitWidth;

  if( Coding == BinaryCoding )
  {
    BinaryHalfOffset = 4 * DigitWidth + BinaryGapWidth;
    ByteWidth += BinaryGapWidth;
  }
  recalcVerticalGridX();
}


// perhaps sometimes there will be a grammar
void KHexColumn::paintEditedByte( QPainter *P, char Byte, const char *EditBuffer )
{
  const QColorGroup &CG = View->colorGroup();

  P->fillRect( 0,0,ByteWidth,LineHeight, QBrush(colorForByte(Byte),Qt::SolidPattern) );

  drawCode( P, EditBuffer, CG.base() );
}


void KHexColumn::drawByte( QPainter *P, char Byte, const QColor &Color ) const
{
  codingFunction()( CodedByte, Byte );
  drawCode( P, CodedByte, Color );
}


void KHexColumn::drawCode( QPainter *P, const char *Code, const QColor &Color ) const
{
  P->setPen( Color );
  if( Coding == BinaryCoding )
  {
    // leave a gap in the middle
    P->drawText( 0, DigitBaseLine, QString::fromLocal8Bit(Code,4) );
    P->drawText( BinaryHalfOffset, DigitBaseLine, QString::fromLocal8Bit(&Code[4],4) );
  }
  else
    P->drawText( 0, DigitBaseLine, QString::fromLocal8Bit(Code) );
}
