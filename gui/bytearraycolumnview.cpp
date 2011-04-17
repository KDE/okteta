/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2007-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraycolumnview.h"
#include "bytearraycolumnview_p.h"
#include "bytearraycolumnview.moc" // due to Q_PRIVATE_SLOT


namespace Okteta
{

ByteArrayColumnView::ByteArrayColumnView( QWidget* parent )
 : AbstractByteArrayView( new ByteArrayColumnViewPrivate(this), parent )
{
    Q_D( ByteArrayColumnView );

    d->init();
}

PixelX ByteArrayColumnView::byteSpacingWidth() const
{
    Q_D( const ByteArrayColumnView );
    return d->byteSpacingWidth();
}
int ByteArrayColumnView::noOfGroupedBytes() const
{
    Q_D( const ByteArrayColumnView );
    return d->noOfGroupedBytes();
}
PixelX ByteArrayColumnView::groupSpacingWidth() const
{
    Q_D( const ByteArrayColumnView );
    return d->groupSpacingWidth();
}
PixelX ByteArrayColumnView::binaryGapWidth() const
{
    Q_D( const ByteArrayColumnView );
    return d->binaryGapWidth();
}
bool ByteArrayColumnView::showsNonprinting() const
{
    Q_D( const ByteArrayColumnView );
    return d->showsNonprinting();
}
QChar ByteArrayColumnView::substituteChar() const
{
    Q_D( const ByteArrayColumnView );
    return d->substituteChar();
}
QChar ByteArrayColumnView::undefinedChar() const
{
    Q_D( const ByteArrayColumnView );
    return d->undefinedChar();
}
bool ByteArrayColumnView::isByteTypeColored() const
{
    Q_D( const ByteArrayColumnView );
    return d->isByteTypeColored();
}


void ByteArrayColumnView::setByteArrayModel( AbstractByteArrayModel* _byteArrayModel )
{
    Q_D( ByteArrayColumnView );
    d->setByteArrayModel( _byteArrayModel );
}

void ByteArrayColumnView::setBufferSpacing( /*PixelX*/int byteSpacing, int noOfGroupedBytes, /*PixelX*/int groupSpacing )
{
    Q_D( ByteArrayColumnView );
    d->setBufferSpacing( byteSpacing, noOfGroupedBytes, groupSpacing );
}


void ByteArrayColumnView::setValueCoding( ValueCoding valueCoding )
{
    Q_D( ByteArrayColumnView );
    d->setValueCoding( valueCoding );
}

void ByteArrayColumnView::setByteSpacingWidth( int/*PixelX*/ byteSpacingWidth )
{
    Q_D( ByteArrayColumnView );
    d->setByteSpacingWidth( byteSpacingWidth );
}

void ByteArrayColumnView::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    Q_D( ByteArrayColumnView );
    d->setNoOfGroupedBytes( noOfGroupedBytes );
}


void ByteArrayColumnView::setGroupSpacingWidth( int/*PixelX*/ groupSpacingWidth )
{
    Q_D( ByteArrayColumnView );
    d->setGroupSpacingWidth( groupSpacingWidth );
}


void ByteArrayColumnView::setBinaryGapWidth( int/*PixelX*/ binaryGapWidth )
{
    Q_D( ByteArrayColumnView );
    d->setBinaryGapWidth( binaryGapWidth );
}


void ByteArrayColumnView::setSubstituteChar( QChar substituteChar )
{
    Q_D( ByteArrayColumnView );
    d->setSubstituteChar( substituteChar );
}

void ByteArrayColumnView::setUndefinedChar( QChar undefinedChar )
{
    Q_D( ByteArrayColumnView );
    d->setUndefinedChar( undefinedChar );
}

void ByteArrayColumnView::setShowsNonprinting( bool showingNonprinting )
{
    Q_D( ByteArrayColumnView );
    d->setShowsNonprinting( showingNonprinting );
}


void ByteArrayColumnView::setCharCoding( CharCoding charCoding )
{
    Q_D( ByteArrayColumnView );
    d->setCharCoding( charCoding );
}

// TODO: join with function above!
void ByteArrayColumnView::setCharCoding( const QString& charCodingName )
{
    Q_D( ByteArrayColumnView );
    d->setCharCoding( charCodingName );
}

void ByteArrayColumnView::setByteTypeColored( bool isColored )
{
    Q_D( ByteArrayColumnView );
    d->setByteTypeColored( isColored );
}


void ByteArrayColumnView::changeEvent( QEvent* event )
{
    Q_D( ByteArrayColumnView );

    d->changeEvent( event );
}


QSize ByteArrayColumnView::minimumSizeHint() const
{
    Q_D( const ByteArrayColumnView );
    // TODO: better minimal width (visibility!)
    return d->minimumSizeHint();
}


void ByteArrayColumnView::renderColumns( QPainter* painter, int cx, int cy, int cw, int ch )
{
    Q_D( ByteArrayColumnView );
    d->renderColumns( painter, cx, cy, cw, ch );
}


ByteArrayColumnView::~ByteArrayColumnView()
{
}

}
