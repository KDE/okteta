/***************************************************************************
                          ktextcolumn.cpp  -  description
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
// c++ specififc
// #include <iostream>
// qt specific
#include <qpainter.h>
// app specific
//#include "khexedit.h"
#include "kcolumnsview.h"
#include "kbuffercursor.h"
#include "kbufferlayout.h"
#include "kbufferranges.h"
#include "ktextcolumn.h"

using namespace KHE;

static bool DefaultShowUnprintable = false;
static QChar DefaultSubstituteChar = '.';

KTextColumn::KTextColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R )
 : KBufferColumn( CV, B, L, R ),
   ShowUnprintable( DefaultShowUnprintable ),
   SubstituteChar( DefaultSubstituteChar )
{
  setSpacing( 0, 0, 0 );
}


KTextColumn::~KTextColumn()
{
}



void KTextColumn::drawByte( QPainter *P, char Byte, const QColor &Color ) const
{
  QString BS = ( (unsigned char)Byte < 32 && !ShowUnprintable ) ?
                 SubstituteChar :
                 QString::fromAscii(&Byte,1);

  P->setPen( Color );
  P->drawText( 0, DigitBaseLine, BS );
}
