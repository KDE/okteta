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

#include "kcharcolumn.h"

// lib
#include "kcolumnsview.h"
#include "kdatacursor.h"
#include "kdatalayout.h"
#include "kdataranges.h"
// KDE
#include <kcharsets.h>
#include <klocale.h>
#include <kglobal.h>
// Qt
#include <QtGui/QPainter>
// C
#include <ctype.h>


namespace KHEUI {

static const bool      DefaultShowsNonprinting = false;
static const QChar     DefaultSubstituteChar =  (char)'.';
static const QChar     DefaultUndefinedChar =   (char)'?';


KCharColumn::KCharColumn( KColumnsView *CV, KHECore::KAbstractByteArrayModel *B, KDataLayout *L, KDataRanges *R )
 : KDataColumn( CV, B, L, R ),
   ShowsNonprinting( DefaultShowsNonprinting ),
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
              !(ShowsNonprinting || B.isPrint()) ? KHECore::KChar(SubstituteChar) :
                B );

  Painter->setPen( Color );
  Painter->drawText( 0, DigitBaseLine, BS );
}

}
