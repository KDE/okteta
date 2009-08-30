/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearraydisplay.h"

// lib
#include "bytearrayjanusview.h"
#include <kbytearraydocument.h>
// Okteta gui
#include <abstractbytearrayview.h>
// Okteta core
#include <abstractbytearraymodel.h>


namespace Kasten
{

KByteArrayDisplay::KByteArrayDisplay( KByteArrayDocument* document )
  : AbstractView( document ),
    mDocument( document )
{
    init();
}

KByteArrayDisplay::KByteArrayDisplay( KByteArrayDisplay* other, Qt::Alignment alignment )
  : AbstractView( static_cast<KByteArrayDocument*>(other->baseModel()) ),
    mDocument( static_cast<KByteArrayDocument*>(other->baseModel()) )
{
    init();

    setViewModus( other->viewModus() );
    setShowsNonprinting( other->showsNonprinting() );
    setVisibleByteArrayCodings( other->visibleByteArrayCodings() );
    toggleOffsetColumn( other->offsetColumnVisible() );
    setValueCoding( other->valueCoding() );
    setCharCoding( other->charCodingName() );
    setOverwriteMode( other->isOverwriteMode() );
    setCursorPosition( other->cursorPosition() );
    mWidget->setStartOffset( other->startOffset() );
    mWidget->setFirstLineOffset( other->firstLineOffset() );
    mWidget->setNoOfBytesPerLine( other->noOfBytesPerLine() );
    // TODO: this can lead to different layouts due to possible one-pixel difference in width!
    setResizeStyle( other->resizeStyle() );
    const Okteta::AddressRange selection = other->selection();
    setSelection( selection.start(), selection.end() );
    setZoomLevel( other->zoomLevel() );
    setReadOnly( other->isReadOnly() );
    // TODO: substituteChar, undefinedChar, all width, groupedBytes

    const QRect otherViewRect = other->mWidget->viewRect();

    QPoint viewPos = otherViewRect.topLeft();
    if( alignment == Qt::AlignBottom )
    {
        viewPos.setY( otherViewRect.bottom() + 1 );
    }
    // TODO: care for resize style
    else if( alignment == Qt::AlignRight )
    {
        viewPos.setX( otherViewRect.right() + 1 );
    }
    // TODO: doesn't really work at this stage, because the widget will get resized when inserted
    // and then ensureCursorVisible destroys the fun
    mWidget->setViewPos( viewPos );
}

void KByteArrayDisplay::init()
{
    Okteta::AbstractByteArrayModel *content = mDocument->content();
    mWidget = new Okteta::ByteArrayJanusView();
    mWidget->setByteArrayModel( content );

    const bool useOverwriteAsDefault = ( content->size() > 0 );

    mWidget->setOverwriteMode( useOverwriteAsDefault );

    // propagate signals
    connect( mDocument, SIGNAL(titleChanged( QString )), SIGNAL(titleChanged( QString )) );
    connect( mDocument, SIGNAL(syncStatesChanged( Kasten::AbstractDocument::SyncStates )),
                        SIGNAL(syncStatesChanged( Kasten::AbstractDocument::SyncStates )) );
    connect( mWidget, SIGNAL(hasSelectedDataChanged( bool )), SIGNAL(hasSelectedDataChanged( bool )) );
    connect( mWidget, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mWidget, SIGNAL(overwriteModeChanged( bool )), SIGNAL(overwriteModeChanged( bool )) );
    connect( mWidget, SIGNAL(selectionChanged( const Okteta::AddressRange& )), SLOT(onSelectionChanged( const Okteta::AddressRange& )) );
    connect( mWidget, SIGNAL(cursorPositionChanged( Okteta::Address )), SIGNAL(cursorPositionChanged( Okteta::Address )) );
    connect( mWidget, SIGNAL(valueCodingChanged( int )), SIGNAL(valueCodingChanged( int )) );
    connect( mWidget, SIGNAL(charCodecChanged( const QString& )), SIGNAL(charCodecChanged( const QString& )) );
    connect( mWidget, SIGNAL(focusChanged( bool )), SIGNAL(focusChanged( bool )) );
}

const AbstractModelSelection* KByteArrayDisplay::modelSelection() const { return &mSelection; }

QWidget* KByteArrayDisplay::widget()             const { return mWidget; }
bool KByteArrayDisplay::hasFocus()               const { return mWidget->focusWidget()->hasFocus(); } // TODO: does this work?

QString KByteArrayDisplay::title()               const { return mDocument->title(); }
bool KByteArrayDisplay::isModifiable()           const { return true; }
bool KByteArrayDisplay::isReadOnly()             const { return mWidget->isReadOnly(); }


void KByteArrayDisplay::setFocus()
{
    mWidget->setFocus();
}

void KByteArrayDisplay::setReadOnly( bool isReadOnly ) { mWidget->setReadOnly( isReadOnly ); }

void KByteArrayDisplay::setZoomLevel( double Level )
{
    mWidget->setZoomLevel( Level );
    emit zoomLevelChanged( mWidget->zoomLevel() );
}

double KByteArrayDisplay::zoomLevel() const
{
    return mWidget->zoomLevel();
}

void KByteArrayDisplay::selectAllData( bool selectAll )
{
    mWidget->selectAll( selectAll );
}

bool KByteArrayDisplay::hasSelectedData() const
{
    return mWidget->hasSelectedData();
}

QMimeData *KByteArrayDisplay::copySelectedData() const
{
    return mWidget->selectionAsMimeData();
}

void KByteArrayDisplay::insertData( const QMimeData *data )
{
    mWidget->pasteData( data );
}

QMimeData *KByteArrayDisplay::cutSelectedData()
{
    QMimeData *result = mWidget->selectionAsMimeData();
    mWidget->removeSelectedData();
    return result;
}

void KByteArrayDisplay::deleteSelectedData()
{
    mWidget->removeSelectedData();
}

bool KByteArrayDisplay::canReadData( const QMimeData *data ) const
{
    return mWidget->canReadData( data );
}

void KByteArrayDisplay::onSelectionChanged( const Okteta::AddressRange& selection )
{
    // TODO: how to make sure the signal hasSelectedDataChanged() is not emitted before?
    mSelection.setRange( selection );
    emit selectedDataChanged( &mSelection );
}

void KByteArrayDisplay::setCursorPosition( Okteta::Address cursorPosition )
{
    mWidget->setCursorPosition( cursorPosition );
}

void KByteArrayDisplay::setSelectionCursorPosition( Okteta::Address index )
{
    mWidget->setSelectionCursorPosition( index );
}

Okteta::Address KByteArrayDisplay::cursorPosition() const
{
    return mWidget->cursorPosition();
}
QRect KByteArrayDisplay::cursorRect() const
{
    return mWidget->cursorRect();
}

Okteta::Address KByteArrayDisplay::startOffset() const
{
    return mWidget->startOffset();
}
Okteta::Address KByteArrayDisplay::firstLineOffset() const
{
    return mWidget->firstLineOffset();
}
int KByteArrayDisplay::noOfBytesPerLine() const
{
    return mWidget->noOfBytesPerLine();
}


int KByteArrayDisplay::valueCoding() const
{
    return mWidget->valueCoding();
}

QString KByteArrayDisplay::charCodingName() const
{
    return mWidget->charCodingName();
}

void KByteArrayDisplay::setValueCoding( int valueCoding )
{
    mWidget->setValueCoding( (Okteta::AbstractByteArrayView::ValueCoding)valueCoding );
}

void KByteArrayDisplay::setCharCoding( const QString& charCodingName )
{
    mWidget->setCharCoding( charCodingName );
}


Okteta::AddressRange KByteArrayDisplay::selection() const
{
    return mWidget->selection();
}

void KByteArrayDisplay::setSelection( Okteta::Address start, Okteta::Address end )
{
    mWidget->setSelection( start, end );
}

void KByteArrayDisplay::insert( const QByteArray& byteArray )
{
    mWidget->insert( byteArray );
}

bool KByteArrayDisplay::showsNonprinting() const
{
    return mWidget->showsNonprinting();
}

bool KByteArrayDisplay::offsetColumnVisible() const
{
    return mWidget->offsetColumnVisible();
}

int KByteArrayDisplay::resizeStyle() const
{
    return (int)mWidget->resizeStyle();
}

int KByteArrayDisplay::visibleByteArrayCodings() const
{
    return (int)mWidget->visibleCodings();
}

bool KByteArrayDisplay::isOverwriteMode() const
{
    return mWidget->isOverwriteMode();
}

void KByteArrayDisplay::setShowsNonprinting( bool on )
{
    mWidget->setShowsNonprinting( on );
}

void KByteArrayDisplay::toggleOffsetColumn( bool on )
{
    mWidget->toggleOffsetColumn( on );
}

void KByteArrayDisplay::setResizeStyle( int resizeStyle )
{
    mWidget->setResizeStyle( (Okteta::AbstractByteArrayView::ResizeStyle)resizeStyle );
}

void KByteArrayDisplay::setVisibleByteArrayCodings( int visibleColumns )
{
    mWidget->setVisibleCodings( visibleColumns );
}

QChar KByteArrayDisplay::substituteChar() const
{
    return mWidget->substituteChar();
}
QChar KByteArrayDisplay::undefinedChar() const
{
    return mWidget->undefinedChar();
}

int KByteArrayDisplay::byteSpacingWidth() const
{
    return mWidget->byteSpacingWidth();
}
int KByteArrayDisplay::noOfGroupedBytes() const
{
    return mWidget->noOfGroupedBytes();
}
int KByteArrayDisplay::groupSpacingWidth() const
{
    return mWidget->groupSpacingWidth();
}
int KByteArrayDisplay::binaryGapWidth() const
{
    return mWidget->binaryGapWidth();
}

bool KByteArrayDisplay::isOverwriteOnly() const
{
    return mWidget->isOverwriteOnly();
}

void KByteArrayDisplay::setOverwriteMode( bool overwriteMode )
{
    mWidget->setOverwriteMode( overwriteMode );
}

void KByteArrayDisplay::setViewModus( int viewModus )
{
    mWidget->setViewModus( viewModus );
}
int KByteArrayDisplay::viewModus() const
{
    return mWidget->viewModus();
}

KByteArrayDisplay::~KByteArrayDisplay()
{
    delete mWidget;
}

}
