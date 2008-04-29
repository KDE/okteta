/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "charbytearraycolumnrenderer.h"

// lib
#include "columnsview.h"
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

static const bool      DefaultShowingNonprinting = false;
static const QChar     DefaultSubstituteChar =  (char)'.';
static const QChar     DefaultUndefinedChar =   (char)'?';


CharByteArrayColumnRenderer::CharByteArrayColumnRenderer( ColumnsView *columnsView,
        KHECore::KAbstractByteArrayModel *byteArrayModel, KDataLayout *layout, KDataRanges *ranges )
 : AbstractByteArrayColumnRenderer( columnsView, byteArrayModel, layout, ranges ),
   mShowingNonprinting( DefaultShowingNonprinting ),
   mSubstituteChar( DefaultSubstituteChar ),
   mUndefinedChar( DefaultUndefinedChar )
{
    setSpacing( 0, 0, 0 );
}


void CharByteArrayColumnRenderer::renderByteText( QPainter *painter, char byte, KHECore::KChar byteChar, const QColor &color ) const
{
Q_UNUSED( byte )

    // turn into a drawable String
    const QString text( byteChar.isUndefined() ?                   KHECore::KChar(mUndefinedChar) :
                       !(mShowingNonprinting || byteChar.isPrint()) ? KHECore::KChar(mSubstituteChar) :
                                                                   byteChar );

    painter->setPen( color );
    painter->drawText( 0, mDigitBaseLine, text );
}

CharByteArrayColumnRenderer::~CharByteArrayColumnRenderer() {}

}
