/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayrowview.h"
#include "bytearrayrowview_p.h"
#include "bytearrayrowview.moc" // due to Q_PRIVATE_SLOT


namespace Okteta
{

ByteArrayRowView::ByteArrayRowView( QWidget* parent )
 : AbstractByteArrayView( new ByteArrayRowViewPrivate(this), parent )
{
    Q_D( ByteArrayRowView );
    d->init();
}

void ByteArrayRowView::setByteArrayModel( AbstractByteArrayModel* byteArrayModel )
{
    Q_D( ByteArrayRowView );
    d->setByteArrayModel( byteArrayModel );
}

PixelX ByteArrayRowView::byteSpacingWidth() const
{
    Q_D( const ByteArrayRowView );
    return d->byteSpacingWidth();
}
int ByteArrayRowView::noOfGroupedBytes() const
{
    Q_D( const ByteArrayRowView );
    return d->noOfGroupedBytes();
}
PixelX ByteArrayRowView::groupSpacingWidth() const
{
    Q_D( const ByteArrayRowView );
    return d->groupSpacingWidth();
}
PixelX ByteArrayRowView::binaryGapWidth() const
{
    Q_D( const ByteArrayRowView );
    return d->binaryGapWidth();
}
bool ByteArrayRowView::showsNonprinting() const
{
    Q_D( const ByteArrayRowView );
    return d->showsNonprinting();
}
QChar ByteArrayRowView::substituteChar() const
{
    Q_D( const ByteArrayRowView );
    return d->substituteChar();
}
QChar ByteArrayRowView::undefinedChar() const
{
    Q_D( const ByteArrayRowView );
    return d->undefinedChar();
}
bool ByteArrayRowView::isByteTypeColored() const
{
    Q_D( const ByteArrayRowView );
    return d->isByteTypeColored();
}
void ByteArrayRowView::setBufferSpacing( /*PixelX*/int byteSpacing, int noOfGroupedBytes, /*PixelX*/int groupSpacing )
{
    Q_D( ByteArrayRowView );
    d->setBufferSpacing( byteSpacing, noOfGroupedBytes, groupSpacing );
}


void ByteArrayRowView::setValueCoding( ValueCoding valueCoding )
{
    Q_D( ByteArrayRowView );
    d->setValueCoding( valueCoding );
}

void ByteArrayRowView::setByteSpacingWidth( int/*PixelX*/ byteSpacingWidth )
{
    Q_D( ByteArrayRowView );
    d->setByteSpacingWidth( byteSpacingWidth );
}

void ByteArrayRowView::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    Q_D( ByteArrayRowView );
    d->setNoOfGroupedBytes( noOfGroupedBytes );
}


void ByteArrayRowView::setGroupSpacingWidth( int/*PixelX*/ groupSpacingWidth )
{
    Q_D( ByteArrayRowView );
    d->setGroupSpacingWidth( groupSpacingWidth );
}


void ByteArrayRowView::setBinaryGapWidth( int/*PixelX*/ binaryGapWidth )
{
    Q_D( ByteArrayRowView );
    d->setBinaryGapWidth( binaryGapWidth );
}


void ByteArrayRowView::setSubstituteChar( QChar substituteChar )
{
    Q_D( ByteArrayRowView );
    d->setSubstituteChar( substituteChar );
}

void ByteArrayRowView::setUndefinedChar( QChar undefinedChar )
{
    Q_D( ByteArrayRowView );
    d->setUndefinedChar( undefinedChar );
}

void ByteArrayRowView::setShowsNonprinting( bool showingNonprinting )
{
    Q_D( ByteArrayRowView );
    d->setShowsNonprinting( showingNonprinting );
}


void ByteArrayRowView::setCharCoding( CharCoding charCoding )
{
    Q_D( ByteArrayRowView );
    d->setCharCoding( charCoding );
}

// TODO: join with function above!
void ByteArrayRowView::setCharCoding( const QString& charCodingName )
{
    Q_D( ByteArrayRowView );
    d->setCharCoding( charCodingName );
}

void ByteArrayRowView::setByteTypeColored( bool isColored )
{
    Q_D( ByteArrayRowView );
    d->setByteTypeColored( isColored );
}


void ByteArrayRowView::changeEvent( QEvent* event )
{
    Q_D( ByteArrayRowView );

    d->changeEvent( event );
}


QSize ByteArrayRowView::minimumSizeHint() const
{
    Q_D( const ByteArrayRowView );
    // TODO: better minimal width (visibility!)
    return d->minimumSizeHint();
}


void ByteArrayRowView::renderColumns( QPainter* painter, int cx, int cy, int cw, int ch )
{
    Q_D( ByteArrayRowView );
    d->renderColumns( painter, cx, cy, cw, ch );
}


ByteArrayRowView::~ByteArrayRowView()
{
}

}
