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
static void textureBuffer( KDataBuffer *DataBuffer, unsigned char b = 1, unsigned char e = 255, unsigned int From = 0, int To = -1 )
{
  int Size = DataBuffer->size();

  if( To == -1 || To >= Size )
    To = Size-1;
  unsigned char c = b;
  for( int i=From; i<=To; ++i )
  {
    if( c == e )
      c = b;
    else
      c++;
    DataBuffer->setDatum(i, c );
  }
}

static void textureBuffer( KDataBuffer *DataBuffer, unsigned char b, unsigned char e, KSection S )
{
  textureBuffer( DataBuffer, b, e, S.start(), S.end() );
}

static void list( KDataBuffer *DataBuffer, const char* Name )
{
  unsigned int Size = DataBuffer->size();
  for( unsigned int i=0; i<Size; ++i )
  {
    kdDebug() << Name<<":"<<i<<":"<<DataBuffer->datum(i) << endl;
  }
}

void KDataBufferIfTest::testModified()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  DataBuffer->setModified( false );
  KT_ASSERT_EQUALS( "modified",  DataBuffer->isModified(), false );
  DataBuffer->setModified( true );
  KT_ASSERT_EQUALS( "modified",  DataBuffer->isModified(), true );
}

void KDataBufferIfTest::testCopyTo()
{
  if( !DataBuffer->isReadOnly() )
  {
    // prepare Buffer
    textureBuffer( DataBuffer );
    DataBuffer->setModified( false );
  }

  //
  static const unsigned int CopySize = 10;
  unsigned int Size = DataBuffer->size();
  KSection Range( 0, CopySize-1 );

  KFixedSizeBuffer Copy( Size, BlankChar );

  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  KT_ASSERT_EQUALS( "copyTo() all",  Copy.compare(*DataBuffer), 0 );

  Copy.fill( BlankChar );
  DataBuffer->copyTo( Copy.rawData(), Range );
  KT_ASSERT_EQUALS( "copyTo() at begin",  Copy.compare(*DataBuffer, Range), 0 );
  KT_ASSERT_EQUALS( "behind copyTo() at begin",  Copy.datum(Range.end()+1), BlankChar );
  KT_ASSERT_EQUALS( "modified by copyTo() at mid",  DataBuffer->isModified(), false );

  Copy.fill( BlankChar );
  Range.moveToEnd( Size - 1 );
  DataBuffer->copyTo( &Copy.rawData()[Range.start()], Range );
  KT_ASSERT_EQUALS( "before copyTo() at end",  Copy.datum(Range.start()-1), BlankChar );
  KT_ASSERT_EQUALS( "copyTo() at end",  Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  KT_ASSERT_EQUALS( "modified by copyTo() at end",  DataBuffer->isModified(), false );

  Copy.fill( BlankChar );
  Range.moveToStart( Size/2 );
  DataBuffer->copyTo( &Copy.rawData()[Range.start()], Range );
  KT_ASSERT_EQUALS( "before copyTo() at mid",  Copy.datum(Range.start()-1), BlankChar );
  KT_ASSERT_EQUALS( "copyTo() at mid",  Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  KT_ASSERT_EQUALS( "behind copyTo() at mid",  Copy.datum(Range.end()+1), BlankChar );
  KT_ASSERT_EQUALS( "modified by copyTo() at mid",  DataBuffer->isModified(), false );
}

void KDataBufferIfTest::testFill()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  static const unsigned int FillSize = 10;
  unsigned int Size = DataBuffer->size();
  KSection Range( 0, FillSize, true );

  KFixedSizeBuffer Copy( Size, PaintChar );

  DataBuffer->setModified( false );
  DataBuffer->fill( BlankChar );
  DataBuffer->fill( PaintChar );
  KT_ASSERT_EQUALS( "fill() all",  Copy.compare(*DataBuffer), 0 );
  KT_ASSERT_EQUALS( "modified by fill() all",  DataBuffer->isModified(), true );

  DataBuffer->setModified( false );
  DataBuffer->fill( BlankChar );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  KT_ASSERT_EQUALS( "fill() at begin",  Copy.compare(*DataBuffer, Range), 0 );
  KT_ASSERT_EQUALS( "behind fill() at begin", DataBuffer->datum(Range.end()+1), BlankChar );
  KT_ASSERT_EQUALS( "modified by fill() at begin",  DataBuffer->isModified(), true );

  DataBuffer->setModified( false );
  DataBuffer->fill( BlankChar );
  Range.moveToEnd( Size - 1 );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  KT_ASSERT_EQUALS( "before fill() at end", DataBuffer->datum(Range.start()-1), BlankChar );
  KT_ASSERT_EQUALS( "fill() at end",  Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  KT_ASSERT_EQUALS( "modified by fill() at end",  DataBuffer->isModified(), true );

  DataBuffer->setModified( false );
  DataBuffer->fill( BlankChar );
  Range.moveToStart( Size/2 );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  KT_ASSERT_EQUALS( "before fill() at mid", DataBuffer->datum(Range.start()-1), BlankChar );
  KT_ASSERT_EQUALS( "fill() at mid",  Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  KT_ASSERT_EQUALS( "behind fill() at mid", DataBuffer->datum(Range.end()+1), BlankChar );
  KT_ASSERT_EQUALS( "modified by fill() at mid",  DataBuffer->isModified(), true );
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
    DataBuffer->setModified( false );
    DataBuffer->setDatum( i, PaintChar );
    if( i>0 )
      KT_ASSERT_EQUALS( "set(), before",  DataBuffer->datum(i-1), BlankChar  );
    KT_ASSERT_EQUALS( "set()",  DataBuffer->datum(i), PaintChar );
    if( i<Size-1 )
      KT_ASSERT_EQUALS( "fill(), behind",  DataBuffer->datum(i+1), BlankChar );
    // clean up
    DataBuffer->setDatum( i, BlankChar );
    KT_ASSERT_EQUALS( "set(), cleared",  DataBuffer->datum(i), BlankChar );
    KT_ASSERT_EQUALS( "modified by setDatum()",  DataBuffer->isModified(), true );
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
  static const unsigned int RemoveSize = 10;
  // create Copy
  unsigned int Size = DataBuffer->size();
  KFixedSizeBuffer Copy( Size );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );

  //
  DataBuffer->setModified( false );
  KSection Range( Size-RemoveSize, Size-1 );
  unsigned int Removed = DataBuffer->remove( Range );
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

  // prepare Copy
  static const int InsertSize = 10;
  int Size = DataBuffer->size();
  KFixedSizeBuffer Copy( Size+3*InsertSize );

  // create InsertData
  KFixedSizeBuffer InsertData( InsertSize );
  textureBuffer( &InsertData, 10, 99 );

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


// how the test works:
// fills the buffer with random data, puts special data at the begin
// copies Data
// moves the Data to the mid, the end and to the begin again
// tests for correct data, modified flag and size
void KDataBufferIfTest::testMove()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  // prepare Copy
  static const int MoveSize = 10;
  const KSection Origin(0, MoveSize, true );
  int Size = DataBuffer->size();
  KFixedSizeBuffer Copy( Size );

  // prepare Buffer
  textureBuffer( DataBuffer, 100, 255, Origin.end()+1 );
  textureBuffer( DataBuffer, 10, 99, Origin );
  KSection Source = Origin;
  DataBuffer->setModified( false );

  // create Copy
  DataBuffer->copyTo( Copy.rawData(), 0, Size );

  // Action: move to middle (to right)
  int DestPos = Size/2;
  KSection Target( DestPos-Source.width(), Source.width(), true );
  int NewPos = DataBuffer->move( DestPos, Source );

  KT_ASSERT_EQUALS( "move() at mid: correct position",  NewPos, Target.start() );
  KT_ASSERT_EQUALS( "move() at mid: data before", Copy.compare(*DataBuffer,KSection(0,Target.start()-1),Origin.end()+1), 0 );
  KT_ASSERT_EQUALS( "move() at mid: data moved", Copy.compare(*DataBuffer,Target,Origin.start()), 0 );
  KT_ASSERT_EQUALS( "move() at mid: data behind",
                    Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Target.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by move() at mid",  DataBuffer->isModified(), NewPos != Source.start() );
  KT_ASSERT_EQUALS( "Size unchanged by move() at mid",  DataBuffer->size(), Size );
  // clean
  DataBuffer->setModified( false );
  Source = Target;

  // Action: move to end (to right)
  DestPos = Size;
  Target.moveToStart( DestPos-Source.width() );
  NewPos = DataBuffer->move( DestPos, Source );

  KT_ASSERT_EQUALS( "move() at end: correct position",  NewPos, Target.start() );
  KT_ASSERT_EQUALS( "move() at end: data before", Copy.compare(*DataBuffer, KSection(0,Target.start()-1),Origin.end()+1), 0 );
  KT_ASSERT_EQUALS( "move() at end: data moved", Copy.compare(*DataBuffer,Target,Origin.start()), 0 );
  KT_ASSERT_EQUALS( "modified by move() at end",  DataBuffer->isModified(), NewPos != Source.start() );
  KT_ASSERT_EQUALS( "Size unchanged by move() at end",  DataBuffer->size(), Size );
  // clean
  DataBuffer->setModified( false );
  Source = Target;

  // Action: move to begin again (to left)
  DestPos = 0;
  Target = Origin;
  NewPos = DataBuffer->move( DestPos, Source );

  KT_ASSERT_EQUALS( "move() at begin: correct position",  NewPos, Target.start() );
  KT_ASSERT_EQUALS( "move() at begin: data moved", Copy.compare(*DataBuffer,Target,Origin.start()), 0 );
  KT_ASSERT_EQUALS( "move() at begin: data behind",
                    Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Origin.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by move() at begin",  DataBuffer->isModified(), NewPos != Source.start() );
  KT_ASSERT_EQUALS( "Size unchanged by move() at begin",  DataBuffer->size(), Size );
}


void KDataBufferIfTest::testReplaceEqual()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  // create InsertData
  static const unsigned int ReplaceSize = 10;
  static const unsigned int InsertSize = ReplaceSize;
  KFixedSizeBuffer InsertData( InsertSize );
  textureBuffer( &InsertData, 10, 99 );

  // prepare Buffer
  unsigned int Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->setModified( false );

  // create Copy
  KFixedSizeBuffer Copy( Size );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );

  // Action: move to begin again (to left)
  KSection Target( 0, ReplaceSize, true );
  unsigned int Inserted = DataBuffer->replace( Target, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at begin: correct number",  Inserted, InsertSize );
  KT_ASSERT_EQUALS( "replace() at begin: data replaced", InsertData.compare(*DataBuffer,Target,0), 0 );
  KT_ASSERT_EQUALS( "replace() at begin: data behind",
                    Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Target.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at begin",  DataBuffer->isModified(), true );
  KT_ASSERT_EQUALS( "Size unchanged by equal replace() at begin",  DataBuffer->size(), (int)Size );
  // clean
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at middle
  Target.moveToStart( Size/2 );
  Inserted = DataBuffer->replace( Target, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at mid: correct number",  Inserted, InsertSize );
  KT_ASSERT_EQUALS( "replace() at mid: data before",
                    Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  KT_ASSERT_EQUALS( "replace() at mid: data replaced", InsertData.compare(*DataBuffer,Target,0), 0 );
  KT_ASSERT_EQUALS( "replace() at mid: data behind",
                    Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Target.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at mid",  DataBuffer->isModified(), true );
  KT_ASSERT_EQUALS( "Size unchanged by equal replace() at mid",  DataBuffer->size(), (int)Size );
  // clean
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at end
  Target.moveToEnd( Size-1 );
  Inserted = DataBuffer->replace( Target, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at end: correct number",  Inserted, InsertSize );
  KT_ASSERT_EQUALS( "replace() at end: data before",
                    Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  KT_ASSERT_EQUALS( "replace() at end: data replaced", InsertData.compare(*DataBuffer,Target,0), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at end",  DataBuffer->isModified(), true );
  KT_ASSERT_EQUALS( "Size unchanged by equal replace() at end",  DataBuffer->size(), (int)Size );
}


void KDataBufferIfTest::testReplaceLess()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  // create InsertData
  static const unsigned int ReplaceSize = 10;
  static const unsigned int Diff = 4;
  static const unsigned int InsertSize = ReplaceSize-Diff;
  KFixedSizeBuffer InsertData( InsertSize );
  textureBuffer( &InsertData, 10, 99 );

  // prepare Buffer
  int Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->setModified( false );

  // create Copy
  KFixedSizeBuffer Copy( Size );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );

  // Action: replace at begin
  KSection Source( 0, ReplaceSize, true );
  KSection Target( 0, InsertSize, true );
  unsigned int Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at begin: correct number",  Inserted, InsertSize );
  KT_ASSERT_EQUALS( "replace() at begin: data replaced", InsertData.compare(*DataBuffer,Target,0), 0 );
  KT_ASSERT_EQUALS( "replace() at begin: data behind",
                    Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1-Diff),Source.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at begin",  DataBuffer->isModified(), true );
  // clean
  Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at middle
  Source.moveToStart( Size/2 );
  Target.moveToStart( Source.start() );
  Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at mid: correct number",  Inserted, InsertSize );
  KT_ASSERT_EQUALS( "replace() at mid: data before",
                    Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  KT_ASSERT_EQUALS( "replace() at mid: data replaced", InsertData.compare(*DataBuffer,Target,0), 0 );
  KT_ASSERT_EQUALS( "replace() at mid: data behind",
                    Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1-Diff),Source.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at mid",  DataBuffer->isModified(), true );
  // clean
  Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at end
  Source.moveToEnd( Size-1 );
  Target.moveToStart( Source.start() );
  Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at end: correct number",  Inserted, InsertSize );
  KT_ASSERT_EQUALS( "replace() at end: data before",
                    Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  KT_ASSERT_EQUALS( "replace() at end: data replaced", InsertData.compare(*DataBuffer,Target,0), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at end",  DataBuffer->isModified(), true );
}


void KDataBufferIfTest::testReplaceMore()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  // create InsertData
  static const unsigned int ReplaceSize = 10;
  static const unsigned int Diff = 4;
  static const unsigned int InsertSize = ReplaceSize+Diff;
  KFixedSizeBuffer InsertData( InsertSize );
  textureBuffer( &InsertData, 10, 99 );

  // prepare Buffer
  int Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->setModified( false );

  // create Copy
  KFixedSizeBuffer Copy( Size );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );

  // Action: replace at begin
  KSection Source( 0, ReplaceSize, true );
  KSection Target( 0, InsertSize, true );
  unsigned int Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at begin: correct number",  Inserted, InsertSize );
  KT_ASSERT_EQUALS( "replace() at begin: data replaced", InsertData.compare(*DataBuffer,Target,0), 0 );
  KT_ASSERT_EQUALS( "replace() at begin: data behind",
                    Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Source.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at begin",  DataBuffer->isModified(), true );
  // clean
  Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at middle
  Source.moveToStart( Size/2 );
  Target.moveToStart( Source.start() );
  Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at mid: correct number", Inserted, InsertSize );
  KT_ASSERT_EQUALS( "replace() at mid: data before",
                    Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  KT_ASSERT_EQUALS( "replace() at mid: data replaced", InsertData.compare(*DataBuffer,Target,0), 0 );
  KT_ASSERT_EQUALS( "replace() at mid: data behind",
                    Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Source.end()+1), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at mid",  DataBuffer->isModified(), true );
  // clean
  Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at end
  Source.moveToEnd( Size-1 );
  Target.moveToStart( Source.start() );
  Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  KT_ASSERT_EQUALS( "replace() at end: correct number", ReplaceSize<=Inserted && Inserted<=InsertSize, true );
  KT_ASSERT_EQUALS( "replace() at end: data before",
                    Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  KT_ASSERT_EQUALS( "replace() at end: data replaced", InsertData.compare(*DataBuffer,KSection(Target.start(),DataBuffer->size()-1),0), 0 );
  KT_ASSERT_EQUALS( "modified by replace() at end",  DataBuffer->isModified(), true );
}


#include "testdatabufferif.moc"
