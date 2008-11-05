/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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


namespace KHEUI
{

ByteArrayRowView::ByteArrayRowView( QWidget* parent )
 : AbstractByteArrayView( new ByteArrayRowViewPrivate(this), parent )
{
    Q_D( ByteArrayRowView );
    d->init();
}

void ByteArrayRowView::setByteArrayModel( KHECore::AbstractByteArrayModel* byteArrayModel )
{
    Q_D( ByteArrayRowView );
    d->setByteArrayModel( byteArrayModel );
}

KPixelX ByteArrayRowView::byteSpacingWidth() const
{
    Q_D( const ByteArrayRowView );
    return d->byteSpacingWidth();
}
int ByteArrayRowView::noOfGroupedBytes() const
{
    Q_D( const ByteArrayRowView );
    return d->noOfGroupedBytes();
}
KPixelX ByteArrayRowView::groupSpacingWidth() const
{
    Q_D( const ByteArrayRowView );
    return d->groupSpacingWidth();
}
KPixelX ByteArrayRowView::binaryGapWidth() const
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
bool ByteArrayRowView::offsetColumnVisible() const
{
    Q_D( const ByteArrayRowView );
    return d->offsetColumnVisible();
}
void ByteArrayRowView::setBufferSpacing( /*KPixelX*/int byteSpacing, int noOfGroupedBytes, /*KPixelX*/int groupSpacing )
{
    Q_D( ByteArrayRowView );
    d->setBufferSpacing( byteSpacing, noOfGroupedBytes, groupSpacing );
}


void ByteArrayRowView::setValueCoding( ValueCoding valueCoding )
{
    Q_D( ByteArrayRowView );
    d->setValueCoding( valueCoding );
}

void ByteArrayRowView::setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth )
{
    Q_D( ByteArrayRowView );
    d->setByteSpacingWidth( byteSpacingWidth );
}

void ByteArrayRowView::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    Q_D( ByteArrayRowView );
    d->setNoOfGroupedBytes( noOfGroupedBytes );
}


void ByteArrayRowView::setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth )
{
    Q_D( ByteArrayRowView );
    d->setGroupSpacingWidth( groupSpacingWidth );
}


void ByteArrayRowView::setBinaryGapWidth( int/*KPixelX*/ binaryGapWidth )
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


void ByteArrayRowView::fontChange( const QFont& oldFont )
{
    Q_D( ByteArrayRowView );

    AbstractByteArrayView::fontChange( oldFont );

    d->handleFontChange( oldFont );
}

void ByteArrayRowView::toggleOffsetColumn( bool showOffsetColumn )
{
    Q_D( ByteArrayRowView );
    d->toggleOffsetColumn( showOffsetColumn );
}


QSize ByteArrayRowView::minimumSizeHint() const
{
    Q_D( const ByteArrayRowView );
    // TODO: better minimal width (visibility!)
    return d->minimumSizeHint();
}


int ByteArrayRowView::indexByPoint( const QPoint& point ) const
{
    Q_D( const ByteArrayRowView );
    return d->indexByPoint( point );
}


void ByteArrayRowView::updateCursors()
{
    Q_D( ByteArrayRowView );
    d->updateCursors();
}


void ByteArrayRowView::renderColumns( QPainter* painter, int cx, int cy, int cw, int ch )
{
    Q_D( ByteArrayRowView );
    d->renderColumns( painter, cx, cy, cw, ch );
}


void ByteArrayRowView::mousePressEvent( QMouseEvent* mouseEvent )
{
    Q_D( ByteArrayRowView );
    d->mousePressEvent( mouseEvent );
}


void ByteArrayRowView::mouseMoveEvent( QMouseEvent* mouseEvent )
{
    Q_D( ByteArrayRowView );
    d->mouseMoveEvent( mouseEvent );
}


void ByteArrayRowView::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
    Q_D( ByteArrayRowView );
    d->mouseReleaseEvent( mouseEvent );
}


// gets called after press and release instead of a plain press event (?)
void ByteArrayRowView::mouseDoubleClickEvent( QMouseEvent* mouseEvent )
{
    Q_D( ByteArrayRowView );
    d->mouseDoubleClickEvent( mouseEvent );
}


ByteArrayRowView::~ByteArrayRowView()
{
}

}
