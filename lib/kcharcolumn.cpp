/***************************************************************************
                          kcharcolumn.cpp  -  description
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


// c specific
#include <ctype.h>
// qt specific
#include <qpainter.h>
#include <qtextcodec.h>
// kde specific
#include <kcharsets.h>
#include <klocale.h>
#include <kglobal.h>
// lib specific
#include "kcolumnsview.h"
#include "kbuffercursor.h"
#include "kbufferlayout.h"
#include "kbufferranges.h"
#include "kcharcolumn.h"

using namespace KHE;

static const KEncoding DefaultEncoding =        LocalEncoding;
static const bool      DefaultShowUnprintable = false;
static const QChar     DefaultSubstituteChar =  '.';

static const unsigned char LowestPrintableChar = 32;

KCharColumn::KCharColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R )
 : KBufferColumn( CV, B, L, R ),
   Encoding( MaxEncodingId ), // forces update
   Codec( 0 ),
   ShowUnprintable( DefaultShowUnprintable ),
   SubstituteChar( DefaultSubstituteChar )
{
  setSpacing( 0, 0, 0 );
  setEncoding( DefaultEncoding );
}


KCharColumn::~KCharColumn()
{
}


bool KCharColumn::setEncoding( KEncoding C )
{
  if( Encoding == C )
    return false;

  switch( C )
  {
    case ISO8859_1Encoding: Codec = KGlobal::charsets()->codecForName( "ISO8859-1" ); break;
    case LocalEncoding: ;
    default: Codec = KGlobal::locale()->codecForEncoding();
  }
  Encoding = C;
  return true;
}



void KCharColumn::drawByte( QPainter *P, char Byte, const QColor &Color ) const
{
//   QString BS = ( (unsigned char)Byte < LowestPrintableChar && !ShowUnprintable ) ?
//                  SubstituteChar :
//                  Encoding == LocalEncoding ? QString::fromLocal8Bit(&Byte,1): QString::fromLatin1(&Byte,1);


  QString BS = Codec->toUnicode( &Byte, 1 );
  if( !ShowUnprintable )
  {
    QCharRef B = BS.at(0);
    if( !B.isPrint() )
      BS.at(0) = SubstituteChar;
  }

  P->setPen( Color );
  P->drawText( 0, DigitBaseLine, BS );
}
