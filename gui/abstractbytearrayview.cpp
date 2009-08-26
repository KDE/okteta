/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractbytearrayview.h"
#include "abstractbytearrayview_p.h"
#include "abstractbytearrayview.moc"

// lib
#include "controller/kcontroller.h"
#include "controller/abstractwheelcontroller.h"
#include "bytearraytablelayout.h"


namespace Okteta
{

AbstractByteArrayView::AbstractByteArrayView( AbstractByteArrayViewPrivate* dd, QWidget* parent )
 : ColumnsView( parent ),
    d_ptr( dd )
{
    Q_D( AbstractByteArrayView );
    d->init();
}

Okteta::AbstractByteArrayModel* AbstractByteArrayView::byteArrayModel() const
{
    Q_D( const AbstractByteArrayView );
    return d->byteArrayModel();
}
bool AbstractByteArrayView::isOverwriteMode() const
{
    Q_D( const AbstractByteArrayView );
    return d->isOverwriteMode();
}
bool AbstractByteArrayView::isOverwriteOnly() const
{
    Q_D( const AbstractByteArrayView );
    return d->isOverwriteOnly();
}
bool AbstractByteArrayView::isReadOnly() const
{
    Q_D( const AbstractByteArrayView );
    return d->isEffectiveReadOnly();
}
bool AbstractByteArrayView::isModified() const
{
    Q_D( const AbstractByteArrayView );
    return d->isModified();
}

double AbstractByteArrayView::zoomLevel() const
{
    Q_D( const AbstractByteArrayView );
    return d->zoomLevel();
}
ByteArrayTableLayout* AbstractByteArrayView::layout() const
{
    Q_D( const AbstractByteArrayView );
     return d->layout();
}
AbstractByteArrayView::ValueCoding AbstractByteArrayView::valueCoding() const
{
    Q_D( const AbstractByteArrayView );
    return d->valueCoding();
}
const Okteta::ValueCodec* AbstractByteArrayView::valueCodec() const
{
    Q_D( const AbstractByteArrayView );
    return d->valueCodec();
}
AbstractByteArrayView::CharCoding AbstractByteArrayView::charCoding() const
{
    Q_D( const AbstractByteArrayView );
    return d->charCoding();
}
const QString& AbstractByteArrayView::charCodingName() const
{
    Q_D( const AbstractByteArrayView );
    return d->charCodingName();
}
const Okteta::CharCodec* AbstractByteArrayView::charCodec() const
{
    Q_D( const AbstractByteArrayView );
    return d->charCodec();
}

ByteArrayTableCursor* AbstractByteArrayView::tableCursor() const
{
    Q_D( const AbstractByteArrayView );
    return d->tableCursor();
}
ByteArrayTableRanges* AbstractByteArrayView::tableRanges() const
{
    Q_D( const AbstractByteArrayView );
    return d->tableRanges();
}

int AbstractByteArrayView::noOfBytesPerLine() const
{
    Q_D( const AbstractByteArrayView );
    return d->noOfBytesPerLine();
}
int AbstractByteArrayView::firstLineOffset()  const
{
    Q_D( const AbstractByteArrayView );
    return d->firstLineOffset();
}
int AbstractByteArrayView::startOffset() const
{
    Q_D( const AbstractByteArrayView );
    return d->startOffset();
}

int AbstractByteArrayView::cursorPosition() const
{
    Q_D( const AbstractByteArrayView );
    return d->cursorPosition();
}
bool AbstractByteArrayView::isCursorBehind() const
{
    Q_D( const AbstractByteArrayView );
    return d->isCursorBehind();
}
AbstractByteArrayView::ResizeStyle AbstractByteArrayView::resizeStyle() const
{
    Q_D( const AbstractByteArrayView );
    return d->resizeStyle();
}

KDE::Section AbstractByteArrayView::selection() const
{
    Q_D( const AbstractByteArrayView );
    return d->selection();
}
bool AbstractByteArrayView::hasSelectedData() const
{
    Q_D( const AbstractByteArrayView );
    return d->hasSelectedData();
}

QByteArray AbstractByteArrayView::selectedData() const
{
    Q_D( const AbstractByteArrayView );
    return d->selectedData();
}
QMimeData* AbstractByteArrayView::selectionAsMimeData() const
{
    Q_D( const AbstractByteArrayView );
    return d->selectionAsMimeData();
}


void AbstractByteArrayView::pasteData( const QMimeData* data )
{
    Q_D( AbstractByteArrayView );
    d->pasteData( data );
}

bool AbstractByteArrayView::canReadData( const QMimeData* data ) const
{
    Q_D( const AbstractByteArrayView );
    return d->canReadData( data );
}

void AbstractByteArrayView::cut()
{
    Q_D( AbstractByteArrayView );
    d->cut();
}

void AbstractByteArrayView::copy()
{
    Q_D( AbstractByteArrayView );
    d->copy();
}

void AbstractByteArrayView::paste()
{
    Q_D( AbstractByteArrayView );
    d->paste();
}

void AbstractByteArrayView::insert( const QByteArray& data )
{
    Q_D( AbstractByteArrayView );
    d->insert( data );
}

void AbstractByteArrayView::removeSelectedData()
{
    Q_D( AbstractByteArrayView );
    d->removeSelectedData();
}

void AbstractByteArrayView::setByteArrayModel( Okteta::AbstractByteArrayModel* byteArrayModel )
{
    Q_D( AbstractByteArrayView );
    d->setByteArrayModel( byteArrayModel );
}

void AbstractByteArrayView::setModified( bool modified )
{
    Q_D( AbstractByteArrayView );
    d->setModified( modified );
}

void AbstractByteArrayView::setOverwriteMode( bool overwriteMode )
{
    Q_D( AbstractByteArrayView );
    d->setOverwriteMode( overwriteMode );
}

void AbstractByteArrayView::setOverwriteOnly( bool overwriteOnly )
{
    Q_D( AbstractByteArrayView );
    d->setOverwriteOnly( overwriteOnly );
}

void AbstractByteArrayView::setResizeStyle( ResizeStyle resizeStyle )
{
    Q_D( AbstractByteArrayView );
    d->setResizeStyle( resizeStyle );
}

void AbstractByteArrayView::setNoOfBytesPerLine( int noOfBytesPerLine )
{
    Q_D( AbstractByteArrayView );
    d->setNoOfBytesPerLine( noOfBytesPerLine );
}


void AbstractByteArrayView::setStartOffset( int startOffset )
{
    Q_D( AbstractByteArrayView );
    d->setStartOffset( startOffset );
}

void AbstractByteArrayView::setFirstLineOffset( int firstLineOffset )
{
    Q_D( AbstractByteArrayView );
    d->setFirstLineOffset( firstLineOffset );
}

void AbstractByteArrayView::setReadOnly( bool readOnly )
{
    Q_D( AbstractByteArrayView );
    d->setReadOnly( readOnly );
}

int AbstractByteArrayView::visibleCodings() const
{
    Q_D( const AbstractByteArrayView );
    return d->visibleCodings();
}
bool AbstractByteArrayView::tabChangesFocus() const
{
    Q_D( const AbstractByteArrayView );
    return d->tabChangesFocus();
}
AbstractByteArrayView::CodingTypeId AbstractByteArrayView::activeCoding() const
{
    Q_D( const AbstractByteArrayView );
    return d->activeCoding();
}

void AbstractByteArrayView::setVisibleCodings( int visibleByteArrayCodings )
{
    Q_D( AbstractByteArrayView );
    d->setVisibleCodings( visibleByteArrayCodings );
}
void AbstractByteArrayView::setActiveCoding( CodingTypeId codingId )
{
    Q_D( AbstractByteArrayView );
    d->setActiveCoding( codingId );
}

void AbstractByteArrayView::setTabChangesFocus( bool tabChangesFocus )
{
    Q_D( AbstractByteArrayView );
    d->setTabChangesFocus( tabChangesFocus );
}


void AbstractByteArrayView::selectAll( bool select )
{
    Q_D( AbstractByteArrayView );
    d->selectAll( select );
}

bool AbstractByteArrayView::selectWord( /*unsigned TODO:change all unneeded signed into unsigned!*/ int index )
{
    Q_D( AbstractByteArrayView );
    return d->selectWord( index );
}

void AbstractByteArrayView::setCursorPosition( int index, bool isBehind )
{
    Q_D( AbstractByteArrayView );
    d->setCursorPosition( index, isBehind );
}

void AbstractByteArrayView::setSelectionCursorPosition( int index )
{
    Q_D( AbstractByteArrayView );
    d->setSelectionCursorPosition( index );
}

void AbstractByteArrayView::setSelection( int start, int end )
{
    Q_D( AbstractByteArrayView );
    d->setSelection( KDE::Section(start,end) );
}

void AbstractByteArrayView::setSelection( const KDE::Section& selection )
{
    Q_D( AbstractByteArrayView );
    d->setSelection( selection );
}


void AbstractByteArrayView::ensureCursorVisible()
{
    Q_D( AbstractByteArrayView );
    d->ensureCursorVisible();
}


void AbstractByteArrayView::placeCursor( const QPoint& point )
{
    Q_D( AbstractByteArrayView );
    d->placeCursor( point );
}


void AbstractByteArrayView::startCursor()
{
    Q_D( AbstractByteArrayView );
    d->startCursor();
}

void AbstractByteArrayView::unpauseCursor()
{
    Q_D( AbstractByteArrayView );
    d->unpauseCursor();
}

void AbstractByteArrayView::stopCursor()
{
    Q_D( AbstractByteArrayView );
    d->stopCursor();
}

void AbstractByteArrayView::pauseCursor()
{
    Q_D( AbstractByteArrayView );
    d->pauseCursor();
}

void AbstractByteArrayView::zoomIn( int pointInc )
{
    Q_D( AbstractByteArrayView );
    d->zoomIn( pointInc );
}
void AbstractByteArrayView::zoomIn()
{
    Q_D( AbstractByteArrayView );
    d->zoomIn();
}
void AbstractByteArrayView::zoomOut( int pointDec )
{
    Q_D( AbstractByteArrayView );
    d->zoomOut( pointDec );
}
void AbstractByteArrayView::zoomOut()
{
    Q_D( AbstractByteArrayView );
    d->zoomOut();
}
void AbstractByteArrayView::zoomTo( int pointSize )
{
    Q_D( AbstractByteArrayView );
    d->zoomTo( pointSize );
}
void AbstractByteArrayView::unZoom()
{
    Q_D( AbstractByteArrayView );
    d->unZoom();
}
void AbstractByteArrayView::setZoomLevel( double zoomLevel )
{
    Q_D( AbstractByteArrayView );
    d->setZoomLevel( zoomLevel );
}

void AbstractByteArrayView::setNoOfLines( int newNoOfLines )
{
    ColumnsView::setNoOfLines( newNoOfLines>1 ? newNoOfLines : 1 );
}

void AbstractByteArrayView::fontChange( const QFont& oldFont )
{
    Q_D( AbstractByteArrayView );
    d->fontChange( oldFont );
}

QSize AbstractByteArrayView::sizeHint() const
{
    return QSize( columnsWidth(), columnsHeight() );
}

void AbstractByteArrayView::finishByteEdit()
{
    Q_D( AbstractByteArrayView );
    d->finishByteEditor();
}

void AbstractByteArrayView::emitSelectionSignals()
{
    Q_D( AbstractByteArrayView );

    const bool hasSelection = tableRanges()->hasSelection();
    if( !d->isOverwriteMode() ) emit cutAvailable( hasSelection );
    emit copyAvailable( hasSelection );
    emit selectionChanged( hasSelection );
}

void AbstractByteArrayView::updateChanged()
{
    Q_D( AbstractByteArrayView );
    d->updateChanged();
}

QRect AbstractByteArrayView::cursorRect() const
{
    Q_D( const AbstractByteArrayView );
    return d->cursorRect();
}


bool AbstractByteArrayView::event( QEvent* event )
{
    Q_D( AbstractByteArrayView );
    return d->event( event );
}

void AbstractByteArrayView::showEvent( QShowEvent* showEvent )
{
    ColumnsView::showEvent( showEvent );
    // TODO: why is this needed?
    layout()->setNoOfLinesPerPage( noOfLinesPerPage() );
}

void AbstractByteArrayView::resizeEvent( QResizeEvent* resizeEvent )
{
    Q_D( AbstractByteArrayView );
    d->resizeEvent( resizeEvent );
}

void AbstractByteArrayView::focusInEvent( QFocusEvent* focusEvent )
{
    Q_D( AbstractByteArrayView );

    d->focusInEvent( focusEvent );
}

void AbstractByteArrayView::focusOutEvent( QFocusEvent* focusEvent )
{
    Q_D( AbstractByteArrayView );

    d->focusOutEvent( focusEvent );
}

void AbstractByteArrayView::keyPressEvent( QKeyEvent* keyEvent )
{
    Q_D( AbstractByteArrayView );
    if( !d->controller()->handleKeyPress(keyEvent) )
        ColumnsView::keyPressEvent( keyEvent );
}

void AbstractByteArrayView::wheelEvent( QWheelEvent* wheelEvent )
{
    Q_D( AbstractByteArrayView );
    if( !d->wheelController()->handleWheelEvent(wheelEvent) )
        ColumnsView::wheelEvent( wheelEvent );
}

void AbstractByteArrayView::dragEnterEvent( QDragEnterEvent* dragEnterEvent )
{
    Q_D( AbstractByteArrayView );
    d->dragEnterEvent( dragEnterEvent );
}
void AbstractByteArrayView::dragMoveEvent( QDragMoveEvent* dragMoveEvent )
{
    Q_D( AbstractByteArrayView );
    d->dragMoveEvent( dragMoveEvent );
}
void AbstractByteArrayView::dragLeaveEvent( QDragLeaveEvent* dragLeaveEvent )
{
    Q_D( AbstractByteArrayView );
    d->dragLeaveEvent( dragLeaveEvent );
}
void AbstractByteArrayView::dropEvent( QDropEvent* dropEvent )
{
    Q_D( AbstractByteArrayView );
    d->dropEvent( dropEvent );
}

AbstractByteArrayView::~AbstractByteArrayView()
{
    delete d_ptr;
}

}
