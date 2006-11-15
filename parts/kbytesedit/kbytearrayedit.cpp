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


// part specific
#include "kbytearrayeditprivate.h"
#include "kbytearrayedit.h"


KByteArrayEdit::KByteArrayEdit( QWidget *parent, const QStringList & )
  : QWidget( parent )
{
    d = new KByteArrayEditPrivate( this );
}



void KByteArrayEdit::setData( char *D, int S, int RS, bool KM )
{
  d->setData( D, S, RS, KM );
}


void KByteArrayEdit::setReadOnly( bool RO )
{
  d->setReadOnly( RO );
}

void KByteArrayEdit::setMaxDataSize( int MS )
{
  d->setMaxDataSize( MS );
}

void KByteArrayEdit::setAutoDelete( bool AD )
{
  d->setAutoDelete( AD );
}


void KByteArrayEdit::setKeepsMemory( bool KM )
{
  d->setKeepsMemory( KM );
}


void KByteArrayEdit::setCursorPosition( int Index )
{
  d->HexEdit->setCursorPosition( Index );
}

void KByteArrayEdit::setCoding( KCoding C )
{
  d->HexEdit->setCoding( (KHEUI::KHexEdit::KCoding) C );
}

void KByteArrayEdit::setResizeStyle( KResizeStyle Style )
{
  d->HexEdit->setResizeStyle( (KHEUI::KHexEdit::KResizeStyle) Style );
}
int KByteArrayEdit::noOfBytesPerLine() const
{
  return d->HexEdit->noOfBytesPerLine();
}

KByteArrayEdit::KResizeStyle KByteArrayEdit::resizeStyle() const
{
  return (KResizeStyle)d->HexEdit->resizeStyle();
}
void KByteArrayEdit::setNoOfBytesPerLine( int NoCpL )
{
  d->HexEdit->setNoOfBytesPerLine( NoCpL );
}



void KByteArrayEdit::setOverwriteOnly( bool b )
{
  d->HexEdit->setOverwriteOnly( b );
}


void KByteArrayEdit::setOverwriteMode( bool b )
{
  d->HexEdit->setOverwriteMode( b );
}


void KByteArrayEdit::setModified( bool b )
{
  d->HexEdit->setModified( b );
}


void KByteArrayEdit::setByteSpacingWidth( int BSW )
{
  d->HexEdit->setByteSpacingWidth( BSW );
}

void KByteArrayEdit::setNoOfGroupedBytes( int NoGB )
{
  d->HexEdit->setNoOfGroupedBytes( NoGB );
}

void KByteArrayEdit::setGroupSpacingWidth( int GSW )
{
  d->HexEdit->setGroupSpacingWidth( GSW );
}

void KByteArrayEdit::setBinaryGapWidth( int BGW )
{
  d->HexEdit->setBinaryGapWidth( BGW );
}

void KByteArrayEdit::setEncoding( KEncoding C )
{
  d->HexEdit->setEncoding( (KHEUI::KHexEdit::KEncoding)C );
}

void KByteArrayEdit::setShowUnprintable( bool SU )
{
  d->HexEdit->setShowUnprintable( SU );
}

void KByteArrayEdit::setSubstituteChar( QChar SC )
{
  d->HexEdit->setSubstituteChar( SC );
}


char *KByteArrayEdit::data() const
{
  return d->data();
}

int KByteArrayEdit::dataSize() const
{
  return d->dataSize();
}
int KByteArrayEdit::maxDataSize () const
{
  return d->maxDataSize();
}
bool KByteArrayEdit::isAutoDelete() const
{
  return d->isAutoDelete();
}

bool KByteArrayEdit::keepsMemory() const
{
  return d->keepsMemory();
}

bool KByteArrayEdit::isOverwriteMode() const
{
  return d->HexEdit->isOverwriteMode();
}

bool KByteArrayEdit::isOverwriteOnly() const
{
  return d->HexEdit->isOverwriteOnly();
}

bool KByteArrayEdit::isModified() const
{
  return d->HexEdit->isModified();
}

bool KByteArrayEdit::isReadOnly() const
{
  return d->HexEdit->isReadOnly();
}


KByteArrayEdit::KCoding KByteArrayEdit::coding() const
{
  return (KCoding)d->HexEdit->coding();
}

int KByteArrayEdit::byteSpacingWidth() const
{
  return d->HexEdit->byteSpacingWidth();
}

int KByteArrayEdit::noOfGroupedBytes() const
{
  return d->HexEdit->noOfGroupedBytes();
}

int KByteArrayEdit::groupSpacingWidth() const
{
  return d->HexEdit->groupSpacingWidth();
}

int KByteArrayEdit::binaryGapWidth() const
{
  return d->HexEdit->binaryGapWidth();
}

bool KByteArrayEdit::showUnprintable() const
{
  return d->HexEdit->showUnprintable();
}

QChar KByteArrayEdit::substituteChar() const
{
  return d->HexEdit->substituteChar();
}

KByteArrayEdit::KEncoding KByteArrayEdit::encoding() const
{
  return (KEncoding)d->HexEdit->encoding();
}


bool KByteArrayEdit::hasSelectedData() const
{
  return d->HexEdit->hasSelectedData();
}


void KByteArrayEdit::repaintRange( int i1, int i2 )
{
  d->repaintRange( i1, i2 );
}


void KByteArrayEdit::insert( const QByteArray &D )
{
  d->HexEdit->insert( D );
}


void KByteArrayEdit::selectAll( bool Select )
{
  d->HexEdit->selectAll( Select );
}

  // clipboard interface
void KByteArrayEdit::copy()
{
  d->HexEdit->copy();
}

void KByteArrayEdit::cut()
{
  d->HexEdit->cut();
}

void KByteArrayEdit::paste()
{
  d->HexEdit->paste();
}

// zooming interface
void KByteArrayEdit::zoomIn( int PointInc )
{
  d->HexEdit->zoomIn( PointInc );
}

void KByteArrayEdit::zoomIn()
{
  d->HexEdit->zoomIn();
}

void KByteArrayEdit::zoomOut( int PointDec )
{
  d->HexEdit->zoomOut( PointDec );
}

void KByteArrayEdit::zoomOut()
{
  d->HexEdit->zoomOut();
}

void KByteArrayEdit::zoomTo( int PointSize )
{
  d->HexEdit->zoomTo( PointSize );
}

void KByteArrayEdit::unZoom()
{
  d->HexEdit->unZoom();
}

#include "kbytearrayedit.moc"
