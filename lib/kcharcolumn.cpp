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

static const bool      DefaultShowUnprintable = false;
static const QChar     DefaultSubstituteChar =  (char)'.';
static const QChar     DefaultUndefinedChar =   (char)'?';


KCharColumn::KCharColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R )
 : KBufferColumn( CV, B, L, R ),
   ShowUnprintable( DefaultShowUnprintable ),
   SubstituteChar( DefaultSubstituteChar ),
   UndefinedChar( DefaultUndefinedChar )
{
  setSpacing( 0, 0, 0 );
}


KCharColumn::~KCharColumn()
{
}

void KCharColumn::drawByte( QPainter *P, char /*Byte*/, KHEChar B, const QColor &Color ) const
{
  // make a drawable String out of it
  QString BS( B.isUndefined() ? KHEChar(UndefinedChar) : ( !(ShowUnprintable || B.isPrint()) ? KHEChar(SubstituteChar) : B ));

  P->setPen( Color );
  P->drawText( 0, DigitBaseLine, BS );
}
