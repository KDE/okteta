/***************************************************************************
                          kcharcolumn.cpp  -  description
                             -------------------
    begin                : Mit Sep 3 2003
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


// c specific
#include <ctype.h>
// qt specific
#include <QtGui/QPainter>
// kde specific
#include <kcharsets.h>
#include <klocale.h>
#include <kglobal.h>
// lib specific
#include "kcolumnsview.h"
#include "kdatacursor.h"
#include "kdatalayout.h"
#include "kdataranges.h"
#include "kcharcolumn.h"


namespace KHEUI {

static const bool      DefaultShowUnprintable = false;
static const QChar     DefaultSubstituteChar =  (char)'.';
static const QChar     DefaultUndefinedChar =   (char)'?';


KCharColumn::KCharColumn( KColumnsView *CV, KHECore::KAbstractByteArrayModel *B, KDataLayout *L, KDataRanges *R )
 : KDataColumn( CV, B, L, R ),
   ShowUnprintable( DefaultShowUnprintable ),
   SubstituteChar( DefaultSubstituteChar ),
   UndefinedChar( DefaultUndefinedChar )
{
  setSpacing( 0, 0, 0 );
}


KCharColumn::~KCharColumn()
{
}


void KCharColumn::drawByte( QPainter *Painter, char /*Byte*/, KHECore::KChar B, const QColor &Color ) const
{
  // turn into a drawable String
  QString BS( B.isUndefined() ?                   KHECore::KChar(UndefinedChar) :
              !(ShowUnprintable || B.isPrint()) ? KHECore::KChar(SubstituteChar) :
                B );

  Painter->setPen( Color );
  Painter->drawText( 0, DigitBaseLine, BS );
}

}
