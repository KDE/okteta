/***************************************************************************
                          testdatabufferif.cpp  -  description
                            -------------------
    begin                : Sam Apr 17 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/

#include <kdebug.h>
// kde specific
#include <kde/test/slottest.h>
#include <kfixedsizebuffer.h>
// app specific
#include "testdatabufferif.h"

using namespace KDE::Test;
using namespace KHE;

static const char FirstMarkChar = 2;
static const char SecondMarkChar = 3;
static const char PaintChar = 1;
static const char BlankChar = '\0';

// fills the buffer with char from b to e
static void textureBuffer( KDataBuffer *DataBuffer, unsigned char b = 1, unsigned char e = 255 )
{
  int Size = DataBuffer->size();

  unsigned char c = b;
  for( int i=0; i<Size; ++i )
  {
    if( c == e )
      c = b;
    else
      c++;
    DataBuffer->setDatum(i, c );
  }
}

static void list( KDataBuffer *DataBuffer, const char* Name )
{
  int Size = DataBuffer->size();
  for( int i=0; i<Size; ++i )
  {
    kdDebug() << Name<<":"<<i<<":"<<DataBuffer->datum(i) << endl;
  }
}

void KDataBufferIfTest::testCopyTo()
{
  if( !DataBuffer->isReadOnly() )
    // prepare Buffer
    textureBuffer( DataBuffer );

  //
  int CopySize = 10;
  int Size = DataBuffer->size();
  KSection Range( 0, CopySize-1 );

  KFixedSizeBuffer Copy( Size, BlankChar );

  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  KT_ASSERT_EQUALS( "copyTo() all",  Copy.compare(*DataBuffer), 0 );

  Copy.fill( BlankChar );
  DataBuffer->copyTo( Copy.rawData(), Range );
  KT_ASSERT_EQUALS( "copyTo() at begin",  Copy.compare(*DataBuffer, Range), 0 );
  KT_ASSERT_EQUALS( "behind copyTo() at begin",  Copy.datum(Range.end()+1), BlankChar );

  Copy.fill( BlankChar );
  Range.moveToEnd( Size - 1 );
  DataBuffer->copyTo( &Copy.rawData()[Range.start()], Range );
  KT_ASSERT_EQUALS( "before copyTo() at end",  Copy.datum(Range.start()-1), BlankChar );
  KT_ASSERT_EQUALS( "copyTo() at end",  Copy.compare(*DataBuffer, Range, Range.start()), 0 );

  Copy.fill( BlankChar );
  Range.moveToStart( Size/2 );
  DataBuffer->copyTo( &Copy.rawData()[Range.start()], Range );
  KT_ASSERT_EQUALS( "before copyTo() at mid",  Copy.datum(Range.start()-1), BlankChar );
  KT_ASSERT_EQUALS( "copyTo() at mid",  Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  KT_ASSERT_EQUALS( "behind copyTo() at mid",  Copy.datum(Range.end()+1), BlankChar );
}

void KDataBufferIfTest::testFill()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  int FillSize = 10;
  int Size = DataBuffer->size();
  KSection Range( 0, FillSize, true );

  KFixedSizeBuffer Copy( Size, PaintChar );

  DataBuffer->fill( BlankChar );
  DataBuffer->fill( PaintChar );
  KT_ASSERT_EQUALS( "fill() all",  Copy.compare(*DataBuffer), 0 );

  DataBuffer->fill( BlankChar );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  KT_ASSERT_EQUALS( "fill() at begin",  Copy.compare(*DataBuffer, Range), 0 );
  KT_ASSERT_EQUALS( "behind fill() at begin", DataBuffer->datum(Range.end()+1), BlankChar );

  DataBuffer->fill( BlankChar );
  Range.moveToEnd( Size - 1 );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  KT_ASSERT_EQUALS( "before fill() at end", DataBuffer->datum(Range.start()-1), BlankChar );
  KT_ASSERT_EQUALS( "fill() at end",  Copy.compare(*DataBuffer, Range, Range.start()), 0 );

  DataBuffer->fill( BlankChar );
  Range.moveToStart( Size/2 );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  KT_ASSERT_EQUALS( "before fill() at mid", DataBuffer->datum(Range.start()-1), BlankChar );
  KT_ASSERT_EQUALS( "fill() at mid",  Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  KT_ASSERT_EQUALS( "behind fill() at mid", DataBuffer->datum(Range.end()+1), BlankChar );
}

void KDataBufferIfTest::testSetGet()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  // prepare buffer
  DataBuffer->fill( BlankChar );

  // test
  unsigned int Size = DataBuffer->size();
  for( unsigned int i=0; i<Size; ++i )
  {
    DataBuffer->setDatum( i, PaintChar );
    if( i>0 )
      KT_ASSERT_EQUALS( "set(), before",  DataBuffer->datum(i-1), BlankChar  );
    KT_ASSERT_EQUALS( "set()",  DataBuffer->datum(i), PaintChar );
    if( i<Size-1 )
      KT_ASSERT_EQUALS( "fill(), behind",  DataBuffer->datum(i+1), BlankChar );
    // clean up
    DataBuffer->setDatum( i, BlankChar );
    KT_ASSERT_EQUALS( "set(), cleared",  DataBuffer->datum(i), BlankChar );
  }
}

// as some buffers might be restricted in growing
// we test for the success of some operations
void KDataBufferIfTest::testRemove()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  textureBuffer( DataBuffer );
  int RemoveSize = 10;
  // create Copy
  int Size = DataBuffer->size();
  KFixedSizeBuffer Copy( Size );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );

  //
  DataBuffer->setModified( false );
  KSection Range( Size-RemoveSize, Size-1 );
  int Removed = DataBuffer->remove( Range );
  Range.setEndByWidth( Removed );

  KT_ASSERT_EQUALS( "remove() at end data before", Copy.compare(*DataBuffer,0,Range.start()-1,0), 0 );
  KT_ASSERT_EQUALS( "modified by remove()", DataBuffer->isModified(), Removed > 0 );

  //
  DataBuffer->setModified( false );
  Size = DataBuffer->size();
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  Range.setByWidth( Size/2, RemoveSize );
  Removed = DataBuffer->remove( Range );
  Range.setEndByWidth( Removed );

  KT_ASSERT_EQUALS( "remove() at mid data before", Copy.compare(*DataBuffer,0,Range.start()-1,0), 0 );
  KT_ASSERT_EQUALS( "remove() at mid data behind",
                    Copy.compare( *DataBuffer,KSection(Range.start(),Size-Removed-1),Range.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by remove()", DataBuffer->isModified(), Removed > 0 );

  //
  DataBuffer->setModified( false );
  Size = DataBuffer->size();
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  Range.setByWidth( 0, RemoveSize );
  Removed = DataBuffer->remove( Range );
  Range.setEndByWidth( Removed );

  KT_ASSERT_EQUALS( "remove() at start data behind",
                    Copy.compare( *DataBuffer,KSection(Range.start(),Size-Removed-1),Range.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by remove()", DataBuffer->isModified(), Removed > 0 );
}


void KDataBufferIfTest::testInsert()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  // create Copy
  int InsertSize = 10;
  int Size = DataBuffer->size();
  KFixedSizeBuffer Copy( Size+3*InsertSize );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );

  // create InsertData
  KFixedSizeBuffer InsertData( InsertSize );
  textureBuffer( &InsertData, 10, 100 );

  // insert at begin
  textureBuffer( DataBuffer,100,255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );
  KSection Range( 0, 0 );

  int Inserted = DataBuffer->insert( Range.start(), InsertData.rawData(), InsertSize );
  Range.setEndByWidth( Inserted );

  KT_ASSERT_EQUALS( "insert() at begin: data inserted", InsertData.compare(*DataBuffer,Range,0), 0 );
  KT_ASSERT_EQUALS( "insert() at begin: data behind",
                    Copy.compare(*DataBuffer,Range.end()+1,Size-Range.end()-1,Range.start()), 0 );
  KT_ASSERT_EQUALS( "modified by insert()",  DataBuffer->isModified(), Inserted > 0 );

  // insert at mid
  Size = DataBuffer->size();
  textureBuffer( DataBuffer,100,255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );
  Range.setStart( Size/2 );

  Inserted = DataBuffer->insert( Range.start(), InsertData.rawData(), InsertSize );
  Range.setEndByWidth( Inserted );

  KT_ASSERT_EQUALS( "insert() at mid: data before", Copy.compare(*DataBuffer, 0,Range.start(),0), 0 );
  KT_ASSERT_EQUALS( "insert() at mid: data inserted", InsertData.compare(*DataBuffer,Range,0), 0 );
  KT_ASSERT_EQUALS( "insert() at mid: data behind",
                    Copy.compare(*DataBuffer,Range.end()+1,Size-Range.end()-1,Range.start()), 0 );
  KT_ASSERT_EQUALS( "modified by insert()",  DataBuffer->isModified(), Inserted > 0 );

  // insert at end
  Size = DataBuffer->size();
  textureBuffer( DataBuffer,100,255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );
  Range.setStart( Size );

  Inserted = DataBuffer->insert( Range.start(), InsertData.rawData(), InsertSize );
  Range.setEndByWidth( Inserted );

//   list( DataBuffer, "DataBuffer" );
//   list( &Copy, "Copy" );
//   kdDebug() << QString("(%1-%2").arg(Range.start()).arg(Range.end()) << endl;
  KT_ASSERT_EQUALS( "insert() at end: data before", Copy.compare(*DataBuffer, 0,Range.start(),0), 0 );
  if( Range.isValid() )
  KT_ASSERT_EQUALS( "insert() at end: data inserted", InsertData.compare(*DataBuffer,Range,0), 0 );
  KT_ASSERT_EQUALS( "modified by insert()",  DataBuffer->isModified(), Inserted > 0 );
}

/*


void KDataBufferIfTest::testAddRemove()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  // create Copy
  int Size = DataBuffer->size();
  KFixedSizeBuffer Copy( Size );
  Copy.copy( *DataBuffer, 0, Size );

  DataBuffer->setModified( false );

  // we remove some part
  DataBuffer->remove( TestSection );

  KT_ASSERT( "remove() data before", DataBuffer->compare(Copy,0,TestSection.start()-1,0) == 0 );
  KT_ASSERT( "remove() data behind",
             DataBuffer->compare(Copy,KSection(TestSection.end()+1,Size-1),TestSection.start()) == 0 );
  KT_ASSERT( "modified by remove()",  DataBuffer->isModified() );
  DataBuffer->setModified( false );

  // and add it again
  DataBuffer->insert( TestSection.start(), Copy.rawData()+TestSection.start(), TestSection.width() );

  KT_ASSERT( "insert() data before",  DataBuffer->compare(Copy,0,TestSection.start()-1,0) == 0 );
  KT_ASSERT( "insert() data inserted",
             DataBuffer->compare(Copy,TestSection,TestSection.start()) == 0 );
  KT_ASSERT( "insert() data behind",
             DataBuffer->compare(Copy,KSection(TestSection.end()+1,Size-1),TestSection.end()+1) == 0 );
  KT_ASSERT( "modified by insert()",  DataBuffer->isModified() );
  DataBuffer->setModified( false );
}
// we 
void KDataBufferIfTest::testReplace() 
{
  // can we alter the buffer at all? 
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  // create Copy
  int Size = DataBuffer->size();
  KFixedSizeBuffer Copy( Size );
  Copy.copy( *DataBuffer, 0, Size );
  // create test data
  int TestSize = TestSection.width();
  KFixedSizeBuffer TestData( TestSize, 'r' );
  KSection LessTestSection( TestSection.start(), TestSection.end()-2 );

  DataBuffer->setModified( false );

  // replace equal space
  DataBuffer->replace( TestSection, TestData.rawData(), TestSize );

  KT_ASSERT( "equal replace() data before",  DataBuffer->compare(Copy,0,TestSection.start()-1,0) == 0 );  
  KT_ASSERT( "equal replace() data inserted",  
             DataBuffer->compare(TestData,0,TestSize,TestSection.start()) == 0 );  
  KT_ASSERT( "equal replace() data behind",  
             DataBuffer->compare(Copy,KSection(TestSection.end()+1,Size-1),TestSection.end()+1) == 0 );  
  KT_ASSERT( "modified by replace()",  DataBuffer->isModified() );  

  DataBuffer->setModified( false );

  DataBuffer->set( TestSection, 'o' );

/*
  // replace less
  DataBuffer->replace( TestSection, TestData.rawData(), TestSize-2 );

  KT_ASSERT( "less replace() data before",  DataBuffer->compare(Copy,0,TestSection.start()-1,0) == 0 );
  KT_ASSERT( "less replace() data inserted",
             DataBuffer->compare(TestData,0,TestSize-2,TestSection.start()) == 0 );
  KT_ASSERT( "less replace() data behind",
             DataBuffer->compare(Copy,KSection(TestSection.end()+1,Size-1),TestSection.end()-1) == 0 );
  KT_ASSERT( "modified by replace()",  DataBuffer->isModified() );
  DataBuffer->setModified( false );
  DataBuffer->set( LessTestSection, 'o' );

  // replace more
  DataBuffer->replace( LessTestSection, TestData.rawData(), TestSize );

  KT_ASSERT( "more replace() data before",  DataBuffer->compare(Copy,0,TestSection.start()-1,0) == 0 );
  KT_ASSERT( "more replace() data inserted",
             DataBuffer->compare(TestData,0,TestSection.width(),TestSection.start()) == 0 );
  KT_ASSERT( "more replace() data behind",
             DataBuffer->compare(Copy,KSection(TestSection.end()+1,Size-1),TestSection.end()+1) == 0 );
  KT_ASSERT( "modified by replace()",  DataBuffer->isModified() );
  *
}
*/
#include "testdatabufferif.moc"
