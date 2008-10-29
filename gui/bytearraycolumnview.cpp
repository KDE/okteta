/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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


namespace KHEUI
{

ByteArrayColumnView::ByteArrayColumnView( KHECore::KAbstractByteArrayModel* _byteArrayModel, QWidget *parent )
 : AbstractByteArrayView( new ByteArrayColumnViewPrivate(this), parent )
{
    Q_D( ByteArrayColumnView );

    d->init();

    setByteArrayModel( _byteArrayModel );
}

KPixelX ByteArrayColumnView::byteSpacingWidth() const
{
    Q_D( const ByteArrayColumnView );
    return d->byteSpacingWidth();
}
int ByteArrayColumnView::noOfGroupedBytes() const
{
    Q_D( const ByteArrayColumnView );
    return d->noOfGroupedBytes();
}
KPixelX ByteArrayColumnView::groupSpacingWidth() const
{
    Q_D( const ByteArrayColumnView );
    return d->groupSpacingWidth();
}
KPixelX ByteArrayColumnView::binaryGapWidth() const
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
bool ByteArrayColumnView::offsetColumnVisible() const
{
    Q_D( const ByteArrayColumnView );
    return d->offsetColumnVisible();
}
int ByteArrayColumnView::visibleByteArrayCodings() const
{
    Q_D( const ByteArrayColumnView );
    return d->visibleByteArrayCodings();
}
bool ByteArrayColumnView::tabChangesFocus() const
{
    Q_D( const ByteArrayColumnView );
    return d->tabChangesFocus();
}
KValueEditor* ByteArrayColumnView::valueEditor() const
{
    Q_D( const ByteArrayColumnView );
    return d->valueEditor();
}
ByteArrayColumnView::CodingTypeId ByteArrayColumnView::activeCoding() const
{
    Q_D( const ByteArrayColumnView );
    return d->activeCoding();
}

void ByteArrayColumnView::setTabChangesFocus( bool tabChangesFocus )
{
    Q_D( ByteArrayColumnView );
    d->setTabChangesFocus( tabChangesFocus );
}


void ByteArrayColumnView::setByteArrayModel( KHECore::KAbstractByteArrayModel* _byteArrayModel )
{
    Q_D( ByteArrayColumnView );
    d->setByteArrayModel( _byteArrayModel );
}

void ByteArrayColumnView::setBufferSpacing( /*KPixelX*/int byteSpacing, int noOfGroupedBytes, /*KPixelX*/int groupSpacing )
{
    Q_D( ByteArrayColumnView );
    d->setBufferSpacing( byteSpacing, noOfGroupedBytes, groupSpacing );
}


void ByteArrayColumnView::setValueCoding( ValueCoding valueCoding )
{
    Q_D( ByteArrayColumnView );
    d->setValueCoding( valueCoding );
}

void ByteArrayColumnView::setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth )
{
    Q_D( ByteArrayColumnView );
    d->setByteSpacingWidth( byteSpacingWidth );
}

void ByteArrayColumnView::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    Q_D( ByteArrayColumnView );
    d->setNoOfGroupedBytes( noOfGroupedBytes );
}


void ByteArrayColumnView::setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth )
{
    Q_D( ByteArrayColumnView );
    d->setGroupSpacingWidth( groupSpacingWidth );
}


void ByteArrayColumnView::setBinaryGapWidth( int/*KPixelX*/ binaryGapWidth )
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


void ByteArrayColumnView::fontChange( const QFont& oldFont )
{
    Q_D( ByteArrayColumnView );

    AbstractByteArrayView::fontChange( oldFont );

    d->handleFontChange( oldFont );
}

void ByteArrayColumnView::toggleOffsetColumn( bool showOffsetColumn )
{
    Q_D( ByteArrayColumnView );
    d->toggleOffsetColumn( showOffsetColumn );
}


QSize ByteArrayColumnView::minimumSizeHint() const
{
    Q_D( const ByteArrayColumnView );
    // TODO: better minimal width (visibility!)
    return d->minimumSizeHint();
}


void ByteArrayColumnView::setVisibleByteArrayCodings( int visibleByteArrayCodings )
{
    Q_D( ByteArrayColumnView );
    d->setVisibleByteArrayCodings( visibleByteArrayCodings );
}


void ByteArrayColumnView::setActiveCoding( CodingTypeId codingId )
{
    Q_D( ByteArrayColumnView );
    d->setActiveCoding( codingId );
}


void ByteArrayColumnView::placeCursor( const QPoint& point )
{
    Q_D( ByteArrayColumnView );
    d->placeCursor( point );
}


int ByteArrayColumnView::indexByPoint( const QPoint& point ) const
{
    Q_D( const ByteArrayColumnView );
    return d->indexByPoint( point );
}


void ByteArrayColumnView::startCursor()
{
    Q_D( ByteArrayColumnView );
    d->startCursor();
}

void ByteArrayColumnView::unpauseCursor()
{
    Q_D( ByteArrayColumnView );
    d->unpauseCursor();
}

void ByteArrayColumnView::stopCursor()
{
    Q_D( ByteArrayColumnView );
    d->stopCursor();
}

void ByteArrayColumnView::pauseCursor()
{
    Q_D( ByteArrayColumnView );
    d->pauseCursor();
}


void ByteArrayColumnView::updateCursors()
{
    Q_D( ByteArrayColumnView );
    d->updateCursors();
}

void ByteArrayColumnView::ensureCursorVisible()
{
    Q_D( ByteArrayColumnView );
    d->ensureCursorVisible();
}


void ByteArrayColumnView::renderColumns( QPainter* painter, int cx, int cy, int cw, int ch )
{
    Q_D( ByteArrayColumnView );
    d->renderColumns( painter, cx, cy, cw, ch );
}

void ByteArrayColumnView::emitSelectionSignals()
{
    Q_D( ByteArrayColumnView );

    const bool hasSelection = tableRanges()->hasSelection();
    if( !d->isOverwriteMode() ) emit cutAvailable( hasSelection );
    emit copyAvailable( hasSelection );
    emit selectionChanged( hasSelection );
}

void ByteArrayColumnView::updateChanged()
{
    Q_D( ByteArrayColumnView );
    d->updateChanged();
}


void ByteArrayColumnView::mousePressEvent( QMouseEvent* mouseEvent )
{
    Q_D( ByteArrayColumnView );
    d->mousePressEvent( mouseEvent );
}


void ByteArrayColumnView::mouseMoveEvent( QMouseEvent* mouseEvent )
{
    Q_D( ByteArrayColumnView );
    d->mouseMoveEvent( mouseEvent );
}


void ByteArrayColumnView::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
    Q_D( ByteArrayColumnView );
    d->mouseReleaseEvent( mouseEvent );
}


// gets called after press and release instead of a plain press event (?)
void ByteArrayColumnView::mouseDoubleClickEvent( QMouseEvent* mouseEvent )
{
    Q_D( ByteArrayColumnView );
    d->mouseDoubleClickEvent( mouseEvent );
}


void ByteArrayColumnView::dragEnterEvent( QDragEnterEvent* dragEnterEvent )
{
    Q_D( ByteArrayColumnView );
    d->dragEnterEvent( dragEnterEvent );
}


void ByteArrayColumnView::dragMoveEvent( QDragMoveEvent* dragMoveEvent )
{
    Q_D( ByteArrayColumnView );
    d->dragMoveEvent( dragMoveEvent );
}


void ByteArrayColumnView::dragLeaveEvent( QDragLeaveEvent* dragLeaveEvent )
{
    Q_D( ByteArrayColumnView );
    d->dragLeaveEvent( dragLeaveEvent );
}


void ByteArrayColumnView::dropEvent( QDropEvent* dropEvent )
{
    Q_D( ByteArrayColumnView );
    d->dropEvent( dropEvent );
}


ByteArrayColumnView::~ByteArrayColumnView()
{
}

}
