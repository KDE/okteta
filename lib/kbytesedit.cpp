/***************************************************************************
                          kbytesedit.cpp  -  description
                             -------------------
    begin                : Die Jul 9 2003
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


// lib specific
#include "kbytearraymodel.h"
#include "kbytesedit.h"
#include "kbufferranges.h"
#include "kbuffercursor.h"

using namespace KHE;


KBytesEdit::KBytesEdit( char *D, int DS, int RS, bool KM, QWidget *Parent, const char */*Name*/ )
 : KHexEdit( 0, Parent ),
   AutoDelete( false )
{
  setData( D, DS, RS, KM );
}

KBytesEdit::KBytesEdit( char *D, int DS, QWidget *Parent, const char */*Name*/ )
 : KHexEdit( 0, Parent ),
   AutoDelete( false )
{
  setData( D, DS );
}


KBytesEdit::KBytesEdit( QWidget *Parent, const char */*Name*/ )
 : KHexEdit( 0, Parent ),
   AutoDelete( false )
{
  setDataBuffer( new KByteArrayModel() );
}

KBytesEdit::~KBytesEdit()
{
  clean();
}

void KBytesEdit::setReadOnly( bool RO )
{
  KByteArrayModel *Buffer = dynamic_cast<KByteArrayModel *>(ByteArrayModel);
  if( Buffer )
    Buffer->setReadOnly( RO );
  KHexEdit::setReadOnly( RO );
}


void KBytesEdit::setAutoDelete( bool AD )
{
  AutoDelete = AD;
}


char *KBytesEdit::data() const
{
  KByteArrayModel *Buffer = dynamic_cast<KByteArrayModel *>(ByteArrayModel);
  return Buffer ? Buffer->data() : 0;
}


void KBytesEdit::setData( char *D, int S, int RS, bool KM )
{
  KByteArrayModel *NewBuffer = new KByteArrayModel( D, S, RS, KM );
  if( ByteArrayModel )
  {
    // take the settings
    NewBuffer->setReadOnly( ByteArrayModel->isReadOnly() );
    clean();
  }
  else
    NewBuffer->setReadOnly( isReadOnly() );

  setDataBuffer( NewBuffer );
}


int KBytesEdit::dataSize() const
{
  KByteArrayModel *Buffer = dynamic_cast<KByteArrayModel *>(ByteArrayModel);
  return Buffer ? Buffer->size() : -1;
}


int KBytesEdit::maxDataSize() const
{
  KByteArrayModel *Buffer = dynamic_cast<KByteArrayModel *>(ByteArrayModel);
  return Buffer ? Buffer->maxSize() : -1;
}


void KBytesEdit::setMaxDataSize( int MS )
{
  KByteArrayModel *Buffer = dynamic_cast<KByteArrayModel *>(ByteArrayModel);
  if( Buffer )
    Buffer->setMaxSize( MS );
}


bool KBytesEdit::keepsMemory() const
{
  KByteArrayModel *Buffer = dynamic_cast<KByteArrayModel *>(ByteArrayModel);
  return Buffer ? Buffer->keepsMemory() : false;
}


void KBytesEdit::setKeepsMemory( bool KM )
{
  KByteArrayModel *Buffer = dynamic_cast<KByteArrayModel *>(ByteArrayModel);
  if( Buffer )
    Buffer->setKeepsMemory( KM );
}


bool KBytesEdit::isAutoDelete() const { return AutoDelete; }


void KBytesEdit::repaintRange( int i1, int i2 )
{
  bool ChangeCursor = !(CursorPaused) && KSection(i1,i2).includes( BufferCursor->index() );
  if( ChangeCursor )
    pauseCursor();

  BufferRanges->addChangedRange( i1, i2 );

  updateChanged();

  if( ChangeCursor )
    unpauseCursor();
}


void KBytesEdit::clean()
{
  if( ByteArrayModel )
  {
    if( AutoDelete )
    {
      char *D = data();
      if( D )
        delete [] D;
    }
    delete ByteArrayModel;
  }
}


#include "kbytesedit.moc"
