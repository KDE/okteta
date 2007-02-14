/***************************************************************************
                          kbytearrayedit.cpp  -  description
                             -------------------
    begin                : Fri Aug 29 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// qt specific
#include <QtGui/QLayout>
#include <QtGui/QWidget>
// corelib specific
#include <kbytearraymodel.h>
// uilib specific
#include <kbytearrayview.h>
// part specific
#include "kbytearrayedit.h"



KByteArrayEdit::KByteArrayEdit( QWidget *parent, const QStringList & )
  : QWidget( parent )
{
    Model = new KHECore::KByteArrayModel;
    HexEdit = new KHEUI::KByteArrayView( Model, this );

    QHBoxLayout* Layout = new QHBoxLayout( this );
    Layout->addWidget( HexEdit );

    connect( HexEdit, SIGNAL(copyAvailable( bool )), SIGNAL(copyAvailable( bool )) );
}



void KByteArrayEdit::setData( char *D, int S, int RS, bool KM )
{
    KHECore::KByteArrayModel *OldModel = Model;
    Model = new KHECore::KByteArrayModel( D, S, RS, KM );

    // get attributes
    Model->setReadOnly( OldModel->isReadOnly() );
    Model->setAutoDelete( OldModel->autoDelete() );

    HexEdit->setByteArrayModel( Model );

    delete OldModel;
}


void KByteArrayEdit::setReadOnly( bool RO )
{
    Model->setReadOnly( RO );
    HexEdit->setReadOnly( RO );
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
    HexEdit->setCursorPosition( Index );
}

void KByteArrayEdit::setCoding( KCoding C )
{
    HexEdit->setCoding( (KHEUI::KByteArrayView::KCoding) C );
}

void KByteArrayEdit::setResizeStyle( KResizeStyle Style )
{
    HexEdit->setResizeStyle( (KHEUI::KByteArrayView::KResizeStyle) Style );
}
int KByteArrayEdit::noOfBytesPerLine() const
{
    return HexEdit->noOfBytesPerLine();
}

KByteArrayEdit::KResizeStyle KByteArrayEdit::resizeStyle() const
{
    return (KResizeStyle)HexEdit->resizeStyle();
}
void KByteArrayEdit::setNoOfBytesPerLine( int NoCpL )
{
    HexEdit->setNoOfBytesPerLine( NoCpL );
}



void KByteArrayEdit::setOverwriteOnly( bool b )
{
    HexEdit->setOverwriteOnly( b );
}


void KByteArrayEdit::setOverwriteMode( bool b )
{
    HexEdit->setOverwriteMode( b );
}


void KByteArrayEdit::setModified( bool b )
{
    HexEdit->setModified( b );
}


void KByteArrayEdit::setByteSpacingWidth( int BSW )
{
    HexEdit->setByteSpacingWidth( BSW );
}

void KByteArrayEdit::setNoOfGroupedBytes( int NoGB )
{
    HexEdit->setNoOfGroupedBytes( NoGB );
}

void KByteArrayEdit::setGroupSpacingWidth( int GSW )
{
    HexEdit->setGroupSpacingWidth( GSW );
}

void KByteArrayEdit::setBinaryGapWidth( int BGW )
{
    HexEdit->setBinaryGapWidth( BGW );
}

void KByteArrayEdit::setEncoding( KEncoding C )
{
    HexEdit->setEncoding( (KHEUI::KByteArrayView::KEncoding)C );
}

void KByteArrayEdit::setShowUnprintable( bool SU )
{
    HexEdit->setShowUnprintable( SU );
}

void KByteArrayEdit::setSubstituteChar( QChar SC )
{
    HexEdit->setSubstituteChar( SC );
}


char *KByteArrayEdit::data() const
{
    return Model->data();
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
    return HexEdit->isOverwriteMode();
}

bool KByteArrayEdit::isOverwriteOnly() const
{
    return HexEdit->isOverwriteOnly();
}

bool KByteArrayEdit::isModified() const
{
    return HexEdit->isModified();
}

bool KByteArrayEdit::isReadOnly() const
{
    return HexEdit->isReadOnly();
}


KByteArrayEdit::KCoding KByteArrayEdit::coding() const
{
    return (KCoding)HexEdit->coding();
}

int KByteArrayEdit::byteSpacingWidth() const
{
    return HexEdit->byteSpacingWidth();
}

int KByteArrayEdit::noOfGroupedBytes() const
{
    return HexEdit->noOfGroupedBytes();
}

int KByteArrayEdit::groupSpacingWidth() const
{
    return HexEdit->groupSpacingWidth();
}

int KByteArrayEdit::binaryGapWidth() const
{
    return HexEdit->binaryGapWidth();
}

bool KByteArrayEdit::showUnprintable() const
{
    return HexEdit->showUnprintable();
}

QChar KByteArrayEdit::substituteChar() const
{
    return HexEdit->substituteChar();
}

KByteArrayEdit::KEncoding KByteArrayEdit::encoding() const
{
    return (KEncoding)HexEdit->encoding();
}


bool KByteArrayEdit::hasSelectedData() const
{
    return HexEdit->hasSelectedData();
}


void KByteArrayEdit::repaintRange( int i1, int i2 )
{
    Model->signalContentsChanged( i1, i2 );
}


void KByteArrayEdit::insert( const QByteArray &D )
{
    HexEdit->insert( D );
}


void KByteArrayEdit::selectAll( bool Select )
{
    HexEdit->selectAll( Select );
}

  // clipboard interface
void KByteArrayEdit::copy()
{
    HexEdit->copy();
}

void KByteArrayEdit::cut()
{
    HexEdit->cut();
}

void KByteArrayEdit::paste()
{
    HexEdit->paste();
}

// zooming interface
void KByteArrayEdit::zoomIn( int PointInc )
{
    HexEdit->zoomIn( PointInc );
}

void KByteArrayEdit::zoomIn()
{
    HexEdit->zoomIn();
}

void KByteArrayEdit::zoomOut( int PointDec )
{
    HexEdit->zoomOut( PointDec );
}

void KByteArrayEdit::zoomOut()
{
    HexEdit->zoomOut();
}

void KByteArrayEdit::zoomTo( int PointSize )
{
    HexEdit->zoomTo( PointSize );
}

void KByteArrayEdit::unZoom()
{
    HexEdit->unZoom();
}

KByteArrayEdit::~KByteArrayEdit()
{
    delete Model;
}


#include "kbytearrayedit.moc"
