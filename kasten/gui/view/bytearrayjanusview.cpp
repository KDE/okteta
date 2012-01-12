/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayjanusview.h"

// Okteta gui
#include <bytearraycolumnview.h>
#include <bytearrayrowview.h>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QScrollBar>


namespace Okteta
{

ByteArrayJanusView::ByteArrayJanusView( QWidget* parent )
  : QWidget( parent ),
    mView( 0 ),
    mViewModus( -1 )
{
    mLayout = new QHBoxLayout( this );
    mLayout->setMargin( 0 );
    setViewModus( ColumnViewId );
}

void ByteArrayJanusView::setByteArrayModel( AbstractByteArrayModel* byteArrayModel )
{
    mView->setByteArrayModel( byteArrayModel );
}

void ByteArrayJanusView::setViewModus( int viewModus )
{
    if( viewModus == mViewModus )
        return;

    AbstractByteArrayView* newView = ( viewModus == ColumnViewId ) ?
        (AbstractByteArrayView*)new ByteArrayColumnView( this ) :
        (AbstractByteArrayView*)new ByteArrayRowView( this );

    const bool hasFocus = mView ? mView->hasFocus() : false;
    if( mView )
    {
        newView->setFont( mView->font() );

        newView->setByteArrayModel( mView->byteArrayModel() );
        newView->setReadOnly( mView->isReadOnly() );
        newView->setOverwriteMode( mView->isOverwriteMode() );
        newView->setZoomLevel( mView->zoomLevel() );
        newView->setShowsNonprinting( mView->showsNonprinting() );
        newView->setValueCoding( mView->valueCoding() );
        newView->setCharCoding( mView->charCodingName() );
        newView->setVisibleCodings( mView->visibleCodings() );
        newView->setActiveCoding( mView->activeCoding() );
        newView->toggleOffsetColumn( mView->offsetColumnVisible() );
        newView->setStartOffset( mView->startOffset() );
        newView->setFirstLineOffset( mView->firstLineOffset() );
        newView->setNoOfBytesPerLine( mView->noOfBytesPerLine() );
        newView->setNoOfGroupedBytes( mView->noOfGroupedBytes() );
        newView->setLayoutStyle( mView->layoutStyle() );
        newView->setSubstituteChar( mView->substituteChar() );
        newView->setUndefinedChar( mView->undefinedChar() );
        newView->setCursorPosition( mView->cursorPosition() );
        newView->setSelection( mView->selection() );
        newView->setMarking( mView->marking() );

        mLayout->removeWidget( mView );
        delete mView;
    }

    mView = newView;

    mLayout->addWidget( mView );
    setFocusProxy( mView );
    if( hasFocus )
        mView->setFocus();
    mViewModus = viewModus;

    connect( mView, SIGNAL(hasSelectedDataChanged(bool)), SIGNAL(hasSelectedDataChanged(bool)) );
    connect( mView, SIGNAL(selectionChanged(Okteta::AddressRange)), SIGNAL(selectionChanged(Okteta::AddressRange)) );
    connect( mView, SIGNAL(readOnlyChanged(bool)), SIGNAL(readOnlyChanged(bool)) );
    connect( mView, SIGNAL(overwriteModeChanged(bool)), SIGNAL(overwriteModeChanged(bool)) );
    connect( mView, SIGNAL(cursorPositionChanged(Okteta::Address)), SIGNAL(cursorPositionChanged(Okteta::Address)) );
    connect( mView, SIGNAL(valueCodingChanged(int)), SIGNAL(valueCodingChanged(int)) );
    connect( mView, SIGNAL(charCodecChanged(QString)), SIGNAL(charCodecChanged(QString)) );
    connect( mView, SIGNAL(focusChanged(bool)), SIGNAL(focusChanged(bool)) );

    connect( mView, SIGNAL(offsetColumnVisibleChanged(bool)), SIGNAL(offsetColumnVisibleChanged(bool)) );
    connect( mView, SIGNAL(visibleByteArrayCodingsChanged(int)), SIGNAL(visibleByteArrayCodingsChanged(int)) );
    connect( mView, SIGNAL(layoutStyleChanged(int)), SIGNAL(layoutStyleChanged(int)) );
    connect( mView, SIGNAL(noOfBytesPerLineChanged(int)), SIGNAL(noOfBytesPerLineChanged(int)) );
    connect( mView, SIGNAL(showsNonprintingChanged(bool)), SIGNAL(showsNonprintingChanged(bool)) );
    connect( mView, SIGNAL(substituteCharChanged(QChar)), SIGNAL(substituteCharChanged(QChar)) );
    connect( mView, SIGNAL(undefinedCharChanged(QChar)), SIGNAL(undefinedCharChanged(QChar)) );
    connect( mView, SIGNAL(noOfGroupedBytesChanged(int)), SIGNAL(noOfGroupedBytesChanged(int)) );

    emit viewModusChanged( mViewModus );
}


bool ByteArrayJanusView::isReadOnly()             const { return mView->isReadOnly(); }
void ByteArrayJanusView::setReadOnly( bool isReadOnly ) { mView->setReadOnly( isReadOnly ); }

void ByteArrayJanusView::setZoomLevel( double Level )
{
    mView->setZoomLevel( Level );
}

double ByteArrayJanusView::zoomLevel() const
{
    return mView->zoomLevel();
}

void ByteArrayJanusView::selectAll( bool selectAll )
{
    mView->selectAll( selectAll );
}

bool ByteArrayJanusView::hasSelectedData() const
{
    return mView->hasSelectedData();
}

QMimeData* ByteArrayJanusView::selectionAsMimeData() const
{
    return mView->selectionAsMimeData();
}

void ByteArrayJanusView::pasteData( const QMimeData* data )
{
    mView->pasteData( data );
}

void ByteArrayJanusView::removeSelectedData()
{
    mView->removeSelectedData();
}

bool ByteArrayJanusView::canReadData( const QMimeData* data ) const
{
    return mView->canReadData( data );
}

void ByteArrayJanusView::setCursorPosition( Address cursorPosition )
{
    mView->setCursorPosition( cursorPosition );
}
void ByteArrayJanusView::setSelectionCursorPosition( Address index )
{
    mView->setSelectionCursorPosition( index );
}
Address ByteArrayJanusView::cursorPosition() const
{
    return mView->cursorPosition();
}
QRect ByteArrayJanusView::cursorRect() const
{
    return mView->cursorRect();
}

void ByteArrayJanusView::setStartOffset( Address startOffset )
{
    mView->setStartOffset( startOffset );
}
void ByteArrayJanusView::setFirstLineOffset( Address firstLineOffset )
{
    mView->setFirstLineOffset( firstLineOffset );
}
void ByteArrayJanusView::setNoOfBytesPerLine( int noOfBytesPerLine )
{
    mView->setNoOfBytesPerLine( noOfBytesPerLine );
}
Address ByteArrayJanusView::startOffset() const
{
    return mView->startOffset();
}
Address ByteArrayJanusView::firstLineOffset() const
{
    return mView->firstLineOffset();
}
int ByteArrayJanusView::noOfBytesPerLine() const
{
    return mView->noOfBytesPerLine();
}


int ByteArrayJanusView::valueCoding() const
{
    return mView->valueCoding();
}

QString ByteArrayJanusView::charCodingName() const
{
    return mView->charCodingName();
}

void ByteArrayJanusView::setValueCoding( int valueCoding )
{
    mView->setValueCoding( (AbstractByteArrayView::ValueCoding)valueCoding );
}

void ByteArrayJanusView::setCharCoding( const QString& charCodingName )
{
    mView->setCharCoding( charCodingName );
}

AddressRange ByteArrayJanusView::selection() const
{
    return mView->selection();
}

void ByteArrayJanusView::setSelection( Address start, Address end )
{
    mView->setSelection( start, end );
}

void ByteArrayJanusView::setMarking( const AddressRange& marking )
{
    mView->setMarking( marking );
}

void ByteArrayJanusView::ensureVisible( const AddressRange& range )
{
    mView->ensureVisible( range );
}

void ByteArrayJanusView::insert( const QByteArray& byteArray )
{
    mView->insert( byteArray );
}

bool ByteArrayJanusView::showsNonprinting() const
{
    return mView->showsNonprinting();
}

bool ByteArrayJanusView::offsetColumnVisible() const
{
    return mView->offsetColumnVisible();
}

int ByteArrayJanusView::layoutStyle() const
{
    return (int)mView->layoutStyle();
}

int ByteArrayJanusView::visibleCodings() const
{
    return (int)mView->visibleCodings();
}

bool ByteArrayJanusView::isOverwriteMode() const
{
    return mView->isOverwriteMode();
}

void ByteArrayJanusView::setShowsNonprinting( bool on )
{
    mView->setShowsNonprinting( on );
}

void ByteArrayJanusView::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    mView->setNoOfGroupedBytes( noOfGroupedBytes );
}

void ByteArrayJanusView::setSubstituteChar( QChar substituteChar )
{
    mView->setSubstituteChar( substituteChar );
}

void ByteArrayJanusView::setUndefinedChar( QChar undefinedChar )
{
    mView->setUndefinedChar( undefinedChar );
}

void ByteArrayJanusView::toggleOffsetColumn( bool on )
{
    mView->toggleOffsetColumn( on );
}

void ByteArrayJanusView::setLayoutStyle( int layoutStyle )
{
    mView->setLayoutStyle( (AbstractByteArrayView::LayoutStyle)layoutStyle );
}

void ByteArrayJanusView::setVisibleCodings( int visibleColumns )
{
    mView->setVisibleCodings( visibleColumns );
}

QChar ByteArrayJanusView::substituteChar() const
{
    return mView->substituteChar();
}
QChar ByteArrayJanusView::undefinedChar() const
{
    return mView->undefinedChar();
}

int ByteArrayJanusView::byteSpacingWidth() const
{
    return mView->byteSpacingWidth();
}
int ByteArrayJanusView::noOfGroupedBytes() const
{
    return mView->noOfGroupedBytes();
}
int ByteArrayJanusView::groupSpacingWidth() const
{
    return mView->groupSpacingWidth();
}
int ByteArrayJanusView::binaryGapWidth() const
{
    return mView->binaryGapWidth();
}

bool ByteArrayJanusView::isOverwriteOnly() const
{
    return mView->isOverwriteOnly();
}

void ByteArrayJanusView::setOverwriteMode( bool overwriteMode )
{
    mView->setOverwriteMode( overwriteMode );
}

void ByteArrayJanusView::setViewPos( const QPoint& pos )
{
    mView->horizontalScrollBar()->setValue( pos.x() );
    mView->verticalScrollBar()->setValue( pos.y() );
}
QRect ByteArrayJanusView::viewRect() const
{
    // TODO: find why mView->viewport()->rect() doesn't work but is always at pos (0.0)
    const QRect result(
        QPoint(mView->horizontalScrollBar()->value(), mView->verticalScrollBar()->value()),
        mView->viewport()->size() );
    return result;
}

void ByteArrayJanusView::propagateFont( const QFont &font )
{
    setFont( font );
    mView->setFont( font );
}

ByteArrayJanusView::~ByteArrayJanusView()
{
}

}
