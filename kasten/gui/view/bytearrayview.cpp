/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayview.h"

// lib
#include "bytearrayjanusview.h"
#include <bytearrayviewprofilesynchronizer.h>
#include <bytearraydocument.h>
// Okteta gui
#include <abstractbytearrayview.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KGlobalSettings>


namespace Kasten2
{

ByteArrayView::ByteArrayView( ByteArrayDocument* document )
  : AbstractView( document )
  , mDocument( document )
  , mByteArrayViewProfileSynchronizer( 0 )
{
    init();
}

ByteArrayView::ByteArrayView( ByteArrayView* other, Qt::Alignment alignment )
  : AbstractView( static_cast<ByteArrayDocument*>(other->baseModel()) )
  , mDocument( static_cast<ByteArrayDocument*>(other->baseModel()) )
  , mByteArrayViewProfileSynchronizer( 0 )
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
    setLayoutStyle( other->layoutStyle() );
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

void ByteArrayView::init()
{
    Okteta::AbstractByteArrayModel* content = mDocument->content();
    mWidget = new Okteta::ByteArrayJanusView();
    mWidget->setByteArrayModel( content );

    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()),
             SLOT(setFontByGlobalSettings()) );
    setFontByGlobalSettings();

    mWidget->setNoOfBytesPerLine( 16 );

    const bool useOverwriteAsDefault = ( content->size() > 0 );
    mWidget->setOverwriteMode( useOverwriteAsDefault );

    // propagate signals
    connect( mDocument, SIGNAL(titleChanged(QString)), SIGNAL(titleChanged(QString)) );
    connect( mWidget, SIGNAL(hasSelectedDataChanged(bool)), SIGNAL(hasSelectedDataChanged(bool)) );
    connect( mWidget, SIGNAL(readOnlyChanged(bool)), SIGNAL(readOnlyChanged(bool)) );
    connect( mWidget, SIGNAL(overwriteModeChanged(bool)), SIGNAL(overwriteModeChanged(bool)) );
    connect( mWidget, SIGNAL(selectionChanged(Okteta::AddressRange)), SLOT(onSelectionChanged(Okteta::AddressRange)) );
    connect( mWidget, SIGNAL(cursorPositionChanged(Okteta::Address)), SIGNAL(cursorPositionChanged(Okteta::Address)) );
    connect( mWidget, SIGNAL(valueCodingChanged(int)), SIGNAL(valueCodingChanged(int)) );
    connect( mWidget, SIGNAL(charCodecChanged(QString)), SIGNAL(charCodecChanged(QString)) );
    connect( mWidget, SIGNAL(focusChanged(bool)), SIGNAL(focusChanged(bool)) );

    connect( mWidget, SIGNAL(offsetColumnVisibleChanged(bool)), SIGNAL(offsetColumnVisibleChanged(bool)) );
    connect( mWidget, SIGNAL(visibleByteArrayCodingsChanged(int)), SIGNAL(visibleByteArrayCodingsChanged(int)) );
    connect( mWidget, SIGNAL(layoutStyleChanged(int)), SIGNAL(layoutStyleChanged(int)) );
    connect( mWidget, SIGNAL(noOfBytesPerLineChanged(int)), SIGNAL(noOfBytesPerLineChanged(int)) );
    connect( mWidget, SIGNAL(showsNonprintingChanged(bool)), SIGNAL(showsNonprintingChanged(bool)) );
    connect( mWidget, SIGNAL(substituteCharChanged(QChar)), SIGNAL(substituteCharChanged(QChar)) );
    connect( mWidget, SIGNAL(undefinedCharChanged(QChar)), SIGNAL(undefinedCharChanged(QChar)) );
    connect( mWidget, SIGNAL(noOfGroupedBytesChanged(int)), SIGNAL(noOfGroupedBytesChanged(int)) );
    connect( mWidget, SIGNAL(viewModusChanged(int)), SIGNAL(viewModusChanged(int)) );
}

void ByteArrayView::setSynchronizer( ByteArrayViewProfileSynchronizer* synchronizer )
{
    delete mByteArrayViewProfileSynchronizer;
    mByteArrayViewProfileSynchronizer = synchronizer;
}

ByteArrayViewProfileSynchronizer* ByteArrayView::synchronizer() const { return mByteArrayViewProfileSynchronizer; }

const AbstractModelSelection* ByteArrayView::modelSelection() const { return &mSelection; }

QWidget* ByteArrayView::widget()             const { return mWidget; }
bool ByteArrayView::hasFocus()               const { return mWidget->focusWidget()->hasFocus(); } // TODO: does this work?

QString ByteArrayView::title()               const { return mDocument->title(); }
bool ByteArrayView::isModifiable()           const { return true; }
bool ByteArrayView::isReadOnly()             const { return mWidget->isReadOnly(); }


void ByteArrayView::setFocus()
{
    mWidget->setFocus();
}

void ByteArrayView::setReadOnly( bool isReadOnly ) { mWidget->setReadOnly( isReadOnly ); }

void ByteArrayView::setZoomLevel( double Level )
{
    mWidget->setZoomLevel( Level );
    emit zoomLevelChanged( mWidget->zoomLevel() );
}

double ByteArrayView::zoomLevel() const
{
    return mWidget->zoomLevel();
}

void ByteArrayView::selectAllData( bool selectAll )
{
    mWidget->selectAll( selectAll );
}

bool ByteArrayView::hasSelectedData() const
{
    return mWidget->hasSelectedData();
}

QMimeData* ByteArrayView::copySelectedData() const
{
    return mWidget->selectionAsMimeData();
}

void ByteArrayView::insertData( const QMimeData* data )
{
    mWidget->pasteData( data );
}

QMimeData* ByteArrayView::cutSelectedData()
{
    QMimeData* result = mWidget->selectionAsMimeData();
    mWidget->removeSelectedData();
    return result;
}

void ByteArrayView::deleteSelectedData()
{
    mWidget->removeSelectedData();
}

bool ByteArrayView::canReadData( const QMimeData* data ) const
{
    return mWidget->canReadData( data );
}

void ByteArrayView::onSelectionChanged( const Okteta::AddressRange& selection )
{
    // TODO: how to make sure the signal hasSelectedDataChanged() is not emitted before?
    mSelection.setRange( selection );
    emit selectedDataChanged( &mSelection );
}

void ByteArrayView::setCursorPosition( Okteta::Address cursorPosition )
{
    mWidget->setCursorPosition( cursorPosition );
}

void ByteArrayView::setSelectionCursorPosition( Okteta::Address index )
{
    mWidget->setSelectionCursorPosition( index );
}

Okteta::Address ByteArrayView::cursorPosition() const
{
    return mWidget->cursorPosition();
}
QRect ByteArrayView::cursorRect() const
{
    return mWidget->cursorRect();
}

Okteta::Address ByteArrayView::startOffset() const
{
    return mWidget->startOffset();
}
Okteta::Address ByteArrayView::firstLineOffset() const
{
    return mWidget->firstLineOffset();
}
int ByteArrayView::noOfBytesPerLine() const
{
    return mWidget->noOfBytesPerLine();
}


int ByteArrayView::valueCoding() const
{
    return mWidget->valueCoding();
}

QString ByteArrayView::charCodingName() const
{
    return mWidget->charCodingName();
}

void ByteArrayView::setValueCoding( int valueCoding )
{
    mWidget->setValueCoding( (Okteta::AbstractByteArrayView::ValueCoding)valueCoding );
}

void ByteArrayView::setCharCoding( const QString& charCodingName )
{
    mWidget->setCharCoding( charCodingName );
}


Okteta::AddressRange ByteArrayView::selection() const
{
    return mWidget->selection();
}

void ByteArrayView::setSelection( Okteta::Address start, Okteta::Address end )
{
    mWidget->setSelection( start, end );
}

void ByteArrayView::insert( const QByteArray& byteArray )
{
    mWidget->insert( byteArray );
}

bool ByteArrayView::showsNonprinting() const
{
    return mWidget->showsNonprinting();
}

bool ByteArrayView::offsetColumnVisible() const
{
    return mWidget->offsetColumnVisible();
}

int ByteArrayView::layoutStyle() const
{
    return (int)mWidget->layoutStyle();
}

int ByteArrayView::visibleByteArrayCodings() const
{
    return (int)mWidget->visibleCodings();
}

bool ByteArrayView::isOverwriteMode() const
{
    return mWidget->isOverwriteMode();
}

void ByteArrayView::setShowsNonprinting( bool on )
{
    mWidget->setShowsNonprinting( on );
}

void ByteArrayView::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    mWidget->setNoOfGroupedBytes( noOfGroupedBytes );
}

void ByteArrayView::toggleOffsetColumn( bool on )
{
    mWidget->toggleOffsetColumn( on );
}

void ByteArrayView::setLayoutStyle( int layoutStyle )
{
    mWidget->setLayoutStyle( (Okteta::AbstractByteArrayView::LayoutStyle)layoutStyle );
}

void ByteArrayView::setNoOfBytesPerLine( int noOfBytesPerLine )
{
    mWidget->setNoOfBytesPerLine( noOfBytesPerLine );
}

void ByteArrayView::setVisibleByteArrayCodings( int visibleColumns )
{
    mWidget->setVisibleCodings( visibleColumns );
}

void ByteArrayView::setMarking( const Okteta::AddressRange& range, bool ensureVisible )
{
    mWidget->setMarking( range );
    if( ensureVisible )
        mWidget->ensureVisible( range );
}

void ByteArrayView::setSubstituteChar( QChar substituteChar )
{
    mWidget->setSubstituteChar( substituteChar );
}

void ByteArrayView::setUndefinedChar( QChar undefinedChar )
{
    mWidget->setUndefinedChar( undefinedChar );
}


QChar ByteArrayView::substituteChar() const
{
    return mWidget->substituteChar();
}
QChar ByteArrayView::undefinedChar() const
{
    return mWidget->undefinedChar();
}

int ByteArrayView::byteSpacingWidth() const
{
    return mWidget->byteSpacingWidth();
}
int ByteArrayView::noOfGroupedBytes() const
{
    return mWidget->noOfGroupedBytes();
}
int ByteArrayView::groupSpacingWidth() const
{
    return mWidget->groupSpacingWidth();
}
int ByteArrayView::binaryGapWidth() const
{
    return mWidget->binaryGapWidth();
}

bool ByteArrayView::isOverwriteOnly() const
{
    return mWidget->isOverwriteOnly();
}

void ByteArrayView::setOverwriteMode( bool overwriteMode )
{
    mWidget->setOverwriteMode( overwriteMode );
}

void ByteArrayView::setViewModus( int viewModus )
{
    mWidget->setViewModus( viewModus );
}
int ByteArrayView::viewModus() const
{
    return mWidget->viewModus();
}

void ByteArrayView::setFontByGlobalSettings()
{
    mWidget->propagateFont( KGlobalSettings::fixedFont() );
}

ByteArrayView::~ByteArrayView()
{
    delete mByteArrayViewProfileSynchronizer;
    delete mWidget;
}

}
