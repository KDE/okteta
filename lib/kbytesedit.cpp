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


// app specific
#include "kplainbuffer.h"
#include "kbytesedit.h"
#include "kbufferranges.h"

using namespace KHE;


KBytesEdit::KBytesEdit( char *D, int DS, int RS, bool KM, QWidget *Parent, const char *Name, WFlags F )
 : KHexEdit( 0L, Parent, Name, F ),
   AutoDelete( false )
{
  setData( D, DS, RS, KM );
}

KBytesEdit::KBytesEdit( char *D, int DS, QWidget *Parent, const char *Name, WFlags F )
 : KHexEdit( 0L, Parent, Name, F ),
   AutoDelete( false )
{
  setData( D, DS );
}


KBytesEdit::KBytesEdit( QWidget *Parent, const char *Name, WFlags F )
 : KHexEdit( 0L, Parent, Name, F ),
   AutoDelete( false )
{
  setDataBuffer( new KPlainBuffer() );
}

KBytesEdit::~KBytesEdit()
{
  clean();
}

void KBytesEdit::setReadOnly( bool RO )
{
  KPlainBuffer *Buffer = dynamic_cast<KPlainBuffer *>(DataBuffer);
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
  KPlainBuffer *Buffer = dynamic_cast<KPlainBuffer *>(DataBuffer);
  return Buffer ? Buffer->data() : 0L;
}


void KBytesEdit::resetData( char *D, int S, bool Repaint )
{
  setData( D, S );

// TODO: think about a way to not reset everything


  if( Repaint )
    repaint();
}


void KBytesEdit::setData( char *D, int S, int RS, bool KM )
{
  KPlainBuffer *NewBuffer = new KPlainBuffer( D, S, RS, KM );
  if( DataBuffer )
  {
    // take the settings
    NewBuffer->setReadOnly( DataBuffer->isReadOnly() );
    clean();
  }
  else
    NewBuffer->setReadOnly( isReadOnly() );

  setDataBuffer( NewBuffer );
}


int KBytesEdit::dataSize() const
{
  KPlainBuffer *Buffer = dynamic_cast<KPlainBuffer *>(DataBuffer);
  return Buffer ? Buffer->size() : -1;
}


int KBytesEdit::maxDataSize() const
{
  KPlainBuffer *Buffer = dynamic_cast<KPlainBuffer *>(DataBuffer);
  return Buffer ? Buffer->maxSize() : -1;
}


void KBytesEdit::setMaxDataSize( int MS )
{
  KPlainBuffer *Buffer = dynamic_cast<KPlainBuffer *>(DataBuffer);
  if( Buffer )
    Buffer->setMaxSize( MS );
}


bool KBytesEdit::keepsMemory() const
{
  KPlainBuffer *Buffer = dynamic_cast<KPlainBuffer *>(DataBuffer);
  return Buffer ? Buffer->keepsMemory() : false;
}


void KBytesEdit::setKeepsMemory( bool KM )
{
  KPlainBuffer *Buffer = dynamic_cast<KPlainBuffer *>(DataBuffer);
  if( Buffer )
    Buffer->setKeepsMemory( KM );
}


bool KBytesEdit::isAutoDelete() const { return AutoDelete; }


void KBytesEdit::repaintRange( int i1, int i2 )
{
  BufferRanges->addChangedRange( i1, i2 );
  repaintChanged();
}


void KBytesEdit::clean()
{
  if( DataBuffer )
  {
    if( AutoDelete )
    {
      char *D = data();
      if( D )
        delete [] D;
    }
    delete DataBuffer;
  }
}


#include "kbytesedit.moc"
