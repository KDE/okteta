/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "kvaluecolumn.h"

// lib
#include "kcolumnsview.h"
#include "kdatacursor.h"
#include "kdatalayout.h"
#include "kdataranges.h"
#include "helper.h"
// Okteta core
#include <kcharcodec.h>
// Qt
#include <QtGui/QPainter>


namespace KHEUI {

static const KHECore::KCoding DefaultCoding =    KHECore::HexadecimalCoding;
static const KHECore::KCoding NotDefaultCoding = KHECore::DecimalCoding;

static const int DefaultBinaryGapWidth = 1;


KValueColumn::KValueColumn( KColumnsView *CV, KHECore::KAbstractByteArrayModel *B, KDataLayout *L, KDataRanges *R )
 : KDataColumn( CV, B, L, R ),
   Coding( NotDefaultCoding ),
   ByteCodec( 0 ),
   BinaryGapWidth( DefaultBinaryGapWidth )
{
  setCoding( DefaultCoding );
}


KValueColumn::~KValueColumn()
{
  delete ByteCodec;
}



bool KValueColumn::setCoding( KHECore::KCoding C )
{
  // no changes?
  if( Coding == C )
    return false;

  delete ByteCodec;

  Coding = C;
  ByteCodec = KHECore::ValueCodec::createCodec( Coding );
  CodedByte.resize( ByteCodec->encodingWidth() );

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

  if( Coding == KHECore::BinaryCoding )
  {
    BinaryHalfOffset = 4 * DigitWidth + BinaryGapWidth;
    ByteWidth += BinaryGapWidth;
  }
}


// perhaps sometimes there will be a grammar
void KValueColumn::paintEditedByte( QPainter *Painter, char Byte, const QString &EditBuffer )
{
  KHECore::KChar B = Codec->decode( Byte );

  Painter->fillRect( 0,0, ByteWidth,lineHeight(), QBrush(colorForChar(B),Qt::SolidPattern) );

  drawCode( Painter, EditBuffer, columnsView()->viewport()->palette().base().color() );
}


void KValueColumn::drawByte( QPainter *Painter, char Byte, KHECore::KChar /*B*/, const QColor &Color ) const
{
  ByteCodec->encode( CodedByte, 0, Byte );
  drawCode( Painter, CodedByte, Color );
}


void KValueColumn::drawCode( QPainter *Painter, const QString &Code, const QColor &Color ) const
{
  Painter->setPen( Color );
  if( Coding == KHECore::BinaryCoding )
  {
    // leave a gap in the middle
    Painter->drawText( 0, DigitBaseLine, Code.left(4) );
    Painter->drawText( BinaryHalfOffset, DigitBaseLine, Code.right(4) );
  }
  else
    Painter->drawText( 0, DigitBaseLine, Code );
}

}
