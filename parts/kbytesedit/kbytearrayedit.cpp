/*
    This file is part of the Okteta KByteArrayEdit module, made within the KDE community.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearrayedit.h"

// Okteta gui
#include <bytearraycolumnview.h>
// Okteta core
#include <bytearraymodel.h>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QWidget>



KByteArrayEdit::KByteArrayEdit( QWidget* parent, const QList<QVariant>& )
  : QWidget( parent )
{
    Model = new Okteta::ByteArrayModel;
    View = new Okteta::ByteArrayColumnView( this );
    View->setByteArrayModel( Model );

    QHBoxLayout* Layout = new QHBoxLayout( this );
    Layout->addWidget( View );

    connect( View, SIGNAL(copyAvailable(bool)), SIGNAL(copyAvailable(bool)) );
}



void KByteArrayEdit::setData( char *D, int S, int RS, bool KM )
{
    Okteta::ByteArrayModel *OldModel = Model;
    Model = new Okteta::ByteArrayModel( reinterpret_cast<Okteta::Byte*>(D), S, RS, KM );

    // get attributes
    Model->setReadOnly( OldModel->isReadOnly() );
    Model->setAutoDelete( OldModel->autoDelete() );

    View->setByteArrayModel( Model );

    delete OldModel;
}


void KByteArrayEdit::setReadOnly( bool RO )
{
    Model->setReadOnly( RO );
    View->setReadOnly( RO );
}

void KByteArrayEdit::setMaxDataSize( int MS )
{
    Model->setMaxSize( MS );
}

void KByteArrayEdit::setAutoDelete( bool AD )
{
    Model->setAutoDelete( AD );
}


void KByteArrayEdit::setKeepsMemory( bool KM )
{
    Model->setKeepsMemory( KM );
}


void KByteArrayEdit::setCursorPosition( int Index )
{
    View->setCursorPosition( Index );
}

void KByteArrayEdit::setCoding( KCoding C )
{
    View->setValueCoding( (Okteta::ByteArrayColumnView::ValueCoding) C );
}

void KByteArrayEdit::setResizeStyle( KResizeStyle Style )
{
    View->setLayoutStyle( (Okteta::AbstractByteArrayView::LayoutStyle) Style );
}
int KByteArrayEdit::noOfBytesPerLine() const
{
    return View->noOfBytesPerLine();
}

KByteArrayEdit::KResizeStyle KByteArrayEdit::resizeStyle() const
{
    return (KResizeStyle)View->layoutStyle();
}
void KByteArrayEdit::setNoOfBytesPerLine( int NoCpL )
{
    View->setNoOfBytesPerLine( NoCpL );
}



void KByteArrayEdit::setOverwriteOnly( bool b )
{
    View->setOverwriteOnly( b );
}


void KByteArrayEdit::setOverwriteMode( bool b )
{
    View->setOverwriteMode( b );
}


void KByteArrayEdit::setModified( bool b )
{
    View->setModified( b );
}


void KByteArrayEdit::setByteSpacingWidth( int BSW )
{
    View->setByteSpacingWidth( BSW );
}

void KByteArrayEdit::setNoOfGroupedBytes( int NoGB )
{
    View->setNoOfGroupedBytes( NoGB );
}

void KByteArrayEdit::setGroupSpacingWidth( int GSW )
{
    View->setGroupSpacingWidth( GSW );
}

void KByteArrayEdit::setBinaryGapWidth( int BGW )
{
    View->setBinaryGapWidth( BGW );
}

void KByteArrayEdit::setEncoding( KEncoding C )
{
    View->setCharCoding( (Okteta::ByteArrayColumnView::CharCoding)C );
}

void KByteArrayEdit::setShowUnprintable( bool SU )
{
    View->setShowsNonprinting( SU );
}

void KByteArrayEdit::setSubstituteChar( QChar SC )
{
    View->setSubstituteChar( SC );
}


char *KByteArrayEdit::data() const
{
    return reinterpret_cast<char*>( Model->data() );
}

int KByteArrayEdit::dataSize() const
{
    return Model->size();
}
int KByteArrayEdit::maxDataSize () const
{
    return Model->maxSize();
}
bool KByteArrayEdit::isAutoDelete() const
{
    return Model->autoDelete();
}

bool KByteArrayEdit::keepsMemory() const
{
    return Model->keepsMemory();
}

bool KByteArrayEdit::isOverwriteMode() const
{
    return View->isOverwriteMode();
}

bool KByteArrayEdit::isOverwriteOnly() const
{
    return View->isOverwriteOnly();
}

bool KByteArrayEdit::isModified() const
{
    return View->isModified();
}

bool KByteArrayEdit::isReadOnly() const
{
    return View->isReadOnly();
}


KByteArrayEdit::KCoding KByteArrayEdit::coding() const
{
    return (KCoding)View->valueCoding();
}

int KByteArrayEdit::byteSpacingWidth() const
{
    return View->byteSpacingWidth();
}

int KByteArrayEdit::noOfGroupedBytes() const
{
    return View->noOfGroupedBytes();
}

int KByteArrayEdit::groupSpacingWidth() const
{
    return View->groupSpacingWidth();
}

int KByteArrayEdit::binaryGapWidth() const
{
    return View->binaryGapWidth();
}

bool KByteArrayEdit::showUnprintable() const
{
    return View->showsNonprinting();
}

QChar KByteArrayEdit::substituteChar() const
{
    return View->substituteChar();
}

KByteArrayEdit::KEncoding KByteArrayEdit::encoding() const
{
    return (KEncoding)View->charCoding();
}


bool KByteArrayEdit::hasSelectedData() const
{
    return View->hasSelectedData();
}


void KByteArrayEdit::repaintRange( int i1, int i2 )
{
    Model->signalContentsChanged( i1, i2 );
}


void KByteArrayEdit::insert( const QByteArray &D )
{
    View->insert( D );
}


void KByteArrayEdit::selectAll( bool Select )
{
    View->selectAll( Select );
}

  // clipboard interface
void KByteArrayEdit::copy()
{
    View->copy();
}

void KByteArrayEdit::cut()
{
    View->cut();
}

void KByteArrayEdit::paste()
{
    View->paste();
}

// zooming interface
void KByteArrayEdit::zoomIn( int PointInc )
{
    View->zoomIn( PointInc );
}

void KByteArrayEdit::zoomIn()
{
    View->zoomIn();
}

void KByteArrayEdit::zoomOut( int PointDec )
{
    View->zoomOut( PointDec );
}

void KByteArrayEdit::zoomOut()
{
    View->zoomOut();
}

void KByteArrayEdit::zoomTo( int PointSize )
{
    View->zoomTo( PointSize );
}

void KByteArrayEdit::unZoom()
{
    View->unZoom();
}

KByteArrayEdit::~KByteArrayEdit()
{
    delete Model;
}

#include "kbytearrayedit.moc"
