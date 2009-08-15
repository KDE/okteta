/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "bytearrayjanusview.h"

// Okteta gui
#include <bytearraycolumnview.h>
#include <bytearrayrowview.h>
// Qt
#include <QtGui/QLayout>


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

void ByteArrayJanusView::setByteArrayModel( Okteta::AbstractByteArrayModel* byteArrayModel )
{
    mView->setByteArrayModel( byteArrayModel );
}

void ByteArrayJanusView::setViewModus( int viewModus )
{
    if( viewModus == mViewModus )
        return;

    AbstractByteArrayView* newView = ( viewModus == ColumnViewId ) ?
        (AbstractByteArrayView*)new Okteta::ByteArrayColumnView( 0, this ) :
        (AbstractByteArrayView*)new Okteta::ByteArrayRowView( this );

    const bool hasFocus = mView ? mView->hasFocus() : false;
    if( mView )
    {
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
        newView->setResizeStyle( mView->resizeStyle() );
        newView->setCursorPosition( mView->cursorPosition() );
        KDE::Section selection = mView->selection();
        newView->setSelection( selection.start(), selection.end() );

        mLayout->removeWidget( mView );
        delete mView;
    }

    mView = newView;

    mLayout->addWidget( mView );
    setFocusProxy( mView );
    if( hasFocus )
        mView->setFocus();
    mViewModus = viewModus;

    connect( mView, SIGNAL(selectionChanged( bool )), SIGNAL(selectionChanged( bool )) );
    connect( mView, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mView, SIGNAL(overwriteModeChanged( bool )), SIGNAL(overwriteModeChanged( bool )) );
    connect( mView, SIGNAL(cursorPositionChanged( int )), SIGNAL(cursorPositionChanged( int )) );
    connect( mView, SIGNAL(valueCodingChanged( int )), SIGNAL(valueCodingChanged( int )) );
    connect( mView, SIGNAL(charCodecChanged( const QString& )), SIGNAL(charCodecChanged( const QString& )) );
    connect( mView, SIGNAL(focusChanged( bool )), SIGNAL(focusChanged( bool )) );
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

void ByteArrayJanusView::setCursorPosition( int cursorPosition )
{
    mView->setCursorPosition( cursorPosition );
}
void ByteArrayJanusView::setSelectionCursorPosition( int index )
{
    mView->setSelectionCursorPosition( index );
}
int ByteArrayJanusView::cursorPosition() const
{
    return mView->cursorPosition();
}
QRect ByteArrayJanusView::cursorRect() const
{
    return mView->cursorRect();
}

int ByteArrayJanusView::startOffset() const
{
    return mView->startOffset();
}
int ByteArrayJanusView::firstLineOffset() const
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
    mView->setValueCoding( (Okteta::AbstractByteArrayView::ValueCoding)valueCoding );
}

void ByteArrayJanusView::setCharCoding( const QString& charCodingName )
{
    mView->setCharCoding( charCodingName );
}

KDE::Section ByteArrayJanusView::selection() const
{
    return mView->selection();
}

void ByteArrayJanusView::setSelection( int start, int end )
{
    mView->setSelection( start, end );
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

int ByteArrayJanusView::resizeStyle() const
{
    return (int)mView->resizeStyle();
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

void ByteArrayJanusView::toggleOffsetColumn( bool on )
{
    mView->toggleOffsetColumn( on );
}

void ByteArrayJanusView::setResizeStyle( int resizeStyle )
{
    mView->setResizeStyle( (Okteta::AbstractByteArrayView::ResizeStyle)resizeStyle );
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

ByteArrayJanusView::~ByteArrayJanusView()
{
}

}
