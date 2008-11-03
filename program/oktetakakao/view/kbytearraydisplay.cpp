/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <kbytearraydocument.h>
// Okteta gui
#include <bytearraycolumnview.h>
// Okteta core
#include <kabstractbytearraymodel.h>


KByteArrayDisplay::KByteArrayDisplay( KByteArrayDocument *document )
 : mWidget( 0 ), mDocument( document )
{
    setBaseModel( mDocument );

    KHECore::KAbstractByteArrayModel *content = mDocument->content();
    mWidget = new KHEUI::ByteArrayColumnView();// content );
    mWidget->setByteArrayModel( content );

    const bool useOverwriteAsDefault = ( content->size() > 0 );

    mWidget->setOverwriteMode( useOverwriteAsDefault );

    // propagate signals
    connect( mDocument, SIGNAL(titleChanged( QString )), SIGNAL(titleChanged( QString )) );
    connect( mDocument, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
                        SIGNAL(modified( KAbstractDocument::SynchronizationStates )) );
//     connect( mWidget, SIGNAL(selectionChanged( bool )), SIGNAL(hasSelectedDataChanged( bool )) );
    connect( mWidget, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mWidget, SIGNAL(overwriteModeChanged( bool )), SIGNAL(overwriteModeChanged( bool )) );
    connect( mWidget, SIGNAL(selectionChanged( bool )), SLOT(onSelectionChange( bool )) );
    connect( mWidget, SIGNAL(cursorPositionChanged( int )), SIGNAL(cursorPositionChanged( int )) );
    connect( mWidget, SIGNAL(valueCodingChanged( int )), SIGNAL(valueCodingChanged( int )) );
    connect( mWidget, SIGNAL(charCodecChanged( const QString& )), SIGNAL(charCodecChanged( const QString& )) );
}

const AbstractModelSelection* KByteArrayDisplay::modelSelection() const { return &mSelection; }

KAbstractDocument *KByteArrayDisplay::document() const { return mDocument; }
QWidget* KByteArrayDisplay::widget()             const { return mWidget; }
QString KByteArrayDisplay::title()               const { return mDocument->title(); }
bool KByteArrayDisplay::isModifiable()           const { return true; }
bool KByteArrayDisplay::isReadOnly()             const { return mWidget->isReadOnly(); }
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

void KByteArrayDisplay::onSelectionChange( bool selected )
{
    mSelection.setSection( mWidget->selection() );
    emit hasSelectedDataChanged( selected );
}

void KByteArrayDisplay::setCursorPosition( int cursorPosition )
{
    mWidget->setCursorPosition( cursorPosition );
}

int KByteArrayDisplay::cursorPosition() const
{
    return mWidget->cursorPosition();
}

int KByteArrayDisplay::startOffset() const
{
    return mWidget->startOffset();
}
int KByteArrayDisplay::firstLineOffset() const
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
    mWidget->setValueCoding( (KHEUI::AbstractByteArrayView::ValueCoding)valueCoding );
}

void KByteArrayDisplay::setCharCoding( const QString& charCodingName )
{
    mWidget->setCharCoding( charCodingName );
}

void KByteArrayDisplay::setFocus()
{
    mWidget->setFocus();
}

KHE::KSection KByteArrayDisplay::selection() const
{
    return mWidget->selection();
}

void KByteArrayDisplay::setSelection( int start, int end )
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
    mWidget->setResizeStyle( (KHEUI::AbstractByteArrayView::ResizeStyle)resizeStyle );
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

KByteArrayDisplay::~KByteArrayDisplay()
{
    delete mWidget;
}
