/***************************************************************************
                          kdatabufferiftest.cpp  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
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
#include <QtTest>
// kde specific
#include <kfixedsizebuffer.h>
// test specific
#include "kdatabufferiftest.h"

using namespace KHE;

// local variables
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
/*
static void list( KDataBuffer *DataBuffer, const char* Name )
{
  unsigned int Size = DataBuffer->size();
  for( unsigned int i=0; i<Size; ++i )
  {
    kdDebug() << Name<<":"<<i<<":"<<DataBuffer->datum(i) << endl;
  }
}
*/

void KDataBufferIfTest::init()
{
  DataBuffer = createBuffer();
}

void KDataBufferIfTest::cleanup()
{
  deleteBuffer( DataBuffer );
}

void KDataBufferIfTest::testModified()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  DataBuffer->setModified( false );
  QVERIFY( !DataBuffer->isModified() );
  DataBuffer->setModified( true );
  QVERIFY( DataBuffer->isModified() );
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
  // copyTo() all
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  QCOMPARE( Copy.compare(*DataBuffer), 0 );
  // copyTo() at begin
  Copy.fill( BlankChar );
  DataBuffer->copyTo( Copy.rawData(), Range );
  QCOMPARE( Copy.compare(*DataBuffer, Range), 0 );
  QCOMPARE( Copy.datum(Range.end()+1), BlankChar );
  QVERIFY( !DataBuffer->isModified() );
  // copyTo() at end
  Copy.fill( BlankChar );
  Range.moveToEnd( Size - 1 );
  DataBuffer->copyTo( &Copy.rawData()[Range.start()], Range );
  QCOMPARE( Copy.datum(Range.start()-1), BlankChar );
  QCOMPARE( Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  QVERIFY( !DataBuffer->isModified() );
  // copyTo() at mid
  Copy.fill( BlankChar );
  Range.moveToStart( Size/2 );
  DataBuffer->copyTo( &Copy.rawData()[Range.start()], Range );
  QCOMPARE( Copy.datum(Range.start()-1), BlankChar );
  QCOMPARE( Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  QCOMPARE( Copy.datum(Range.end()+1), BlankChar );
  QVERIFY( !DataBuffer->isModified() );
}

void KDataBufferIfTest::testFill()
{
  // can we alter the buffer at all?
  if( DataBuffer->isReadOnly() )
    // skip
    return;

  static const unsigned int FillSize = 10;
  unsigned int Size = DataBuffer->size();
  KSection Range = KSection::fromWidth( 0, FillSize );

  KFixedSizeBuffer Copy( Size, PaintChar );

  // fill() all
  DataBuffer->setModified( false );
  DataBuffer->fill( BlankChar );
  DataBuffer->fill( PaintChar );
  QCOMPARE( Copy.compare(*DataBuffer), 0 );
  QVERIFY( DataBuffer->isModified() );
  // fill() at begin
  DataBuffer->setModified( false );
  DataBuffer->fill( BlankChar );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  QCOMPARE( Copy.compare(*DataBuffer, Range), 0 );
  QCOMPARE( DataBuffer->datum(Range.end()+1), BlankChar );
  QVERIFY( DataBuffer->isModified() );
  // fill() at end
  DataBuffer->setModified( false );
  DataBuffer->fill( BlankChar );
  Range.moveToEnd( Size - 1 );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  QCOMPARE( DataBuffer->datum(Range.start()-1), BlankChar );
  QCOMPARE( Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  QVERIFY( DataBuffer->isModified() );
  // fill() at mid
  DataBuffer->setModified( false );
  DataBuffer->fill( BlankChar );
  Range.moveToStart( Size/2 );
  DataBuffer->fill( PaintChar, Range.width(), Range.start() );
  QCOMPARE( DataBuffer->datum(Range.start()-1), BlankChar );
  QCOMPARE( Copy.compare(*DataBuffer, Range, Range.start()), 0 );
  QCOMPARE( DataBuffer->datum(Range.end()+1), BlankChar );
  QVERIFY( DataBuffer->isModified() );
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
      QCOMPARE( DataBuffer->datum(i-1), BlankChar  );
    QCOMPARE( DataBuffer->datum(i), PaintChar );
    if( i<Size-1 )
      QCOMPARE( DataBuffer->datum(i+1), BlankChar );
    // clean up
    DataBuffer->setDatum( i, BlankChar );
    QCOMPARE( DataBuffer->datum(i), BlankChar );
    QVERIFY( DataBuffer->isModified() );
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

  // remove() at end
  DataBuffer->setModified( false );
  KSection Range( Size-RemoveSize, Size-1 );
  unsigned int Removed = DataBuffer->remove( Range );
  Range.setEndByWidth( Removed );

  QCOMPARE( Copy.compare(*DataBuffer,0,Range.start()-1,0), 0 );
  QCOMPARE( DataBuffer->isModified(), Removed > 0 );

  // remove() at mid
  DataBuffer->setModified( false );
  Size = DataBuffer->size();
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  Range.setByWidth( Size/2, RemoveSize );
  Removed = DataBuffer->remove( Range );
  Range.setEndByWidth( Removed );

  QCOMPARE( Copy.compare(*DataBuffer,0,Range.start()-1,0), 0 );
  QCOMPARE( Copy.compare( *DataBuffer,KSection(Range.start(),Size-Removed-1),Range.end()+1), 0 );
  QCOMPARE( DataBuffer->isModified(), Removed > 0 );

  // remove() at start
  DataBuffer->setModified( false );
  Size = DataBuffer->size();
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  Range.setByWidth( 0, RemoveSize );
  Removed = DataBuffer->remove( Range );
  Range.setEndByWidth( Removed );

  QCOMPARE( Copy.compare( *DataBuffer,KSection(Range.start(),Size-Removed-1),Range.end()+1), 0 );
  QCOMPARE( DataBuffer->isModified(), Removed > 0 );
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

  QCOMPARE( InsertData.compare(*DataBuffer,Range,0), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,Range.end()+1,Size-Range.end()-1,Range.start()), 0 );
  QCOMPARE( DataBuffer->isModified(), Inserted > 0 );

  // insert at mid
  Size = DataBuffer->size();
  textureBuffer( DataBuffer,100,255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );
  Range.setStart( Size/2 );

  Inserted = DataBuffer->insert( Range.start(), InsertData.rawData(), InsertSize );
  Range.setEndByWidth( Inserted );

  QCOMPARE( Copy.compare(*DataBuffer, 0,Range.start(),0), 0 );
  QCOMPARE( InsertData.compare(*DataBuffer,Range,0), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,Range.end()+1,Size-Range.end()-1,Range.start()), 0 );
  QCOMPARE( DataBuffer->isModified(), Inserted > 0 );

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
  QCOMPARE( Copy.compare(*DataBuffer, 0,Range.start(),0), 0 );
  if( Range.isValid() )
    QCOMPARE( InsertData.compare(*DataBuffer,Range,0), 0 );
  QCOMPARE( DataBuffer->isModified(), Inserted > 0 );
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
  const KSection Origin = KSection::fromWidth( 0, MoveSize );
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
  KSection Target = KSection::fromWidth( DestPos-Source.width(), Source.width() );
  int NewPos = DataBuffer->move( DestPos, Source );

  QCOMPARE( NewPos, Target.start() );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(0,Target.start()-1),Origin.end()+1), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,Target,Origin.start()), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Target.end()+1), 0 );
  QCOMPARE( DataBuffer->isModified(), NewPos != Source.start() );
  QCOMPARE( DataBuffer->size(), Size );
  // clean
  DataBuffer->setModified( false );
  Source = Target;

  // Action: move to end (to right)
  DestPos = Size;
  Target.moveToStart( DestPos-Source.width() );
  NewPos = DataBuffer->move( DestPos, Source );

  QCOMPARE( NewPos, Target.start() );
  QCOMPARE( Copy.compare(*DataBuffer, KSection(0,Target.start()-1),Origin.end()+1), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,Target,Origin.start()), 0 );
  QCOMPARE( DataBuffer->isModified(), NewPos != Source.start() );
  QCOMPARE( DataBuffer->size(), Size );
  // clean
  DataBuffer->setModified( false );
  Source = Target;

  // Action: move to begin again (to left)
  DestPos = 0;
  Target = Origin;
  NewPos = DataBuffer->move( DestPos, Source );

  QCOMPARE( NewPos, Target.start() );
  QCOMPARE( Copy.compare(*DataBuffer,Target,Origin.start()), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Origin.end()+1), 0 );
  QCOMPARE( DataBuffer->isModified(), NewPos != Source.start() );
  QCOMPARE( DataBuffer->size(), Size );
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
  KSection Target = KSection::fromWidth( 0, ReplaceSize );
  unsigned int Inserted = DataBuffer->replace( Target, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( InsertData.compare(*DataBuffer,Target,0), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Target.end()+1), 0 );
  QCOMPARE( DataBuffer->isModified(), true );
  QCOMPARE( DataBuffer->size(), (int)Size );
  // clean
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at middle
  Target.moveToStart( Size/2 );
  Inserted = DataBuffer->replace( Target, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  QCOMPARE( InsertData.compare(*DataBuffer,Target,0), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Target.end()+1), 0 );
  QVERIFY( DataBuffer->isModified() );
  QCOMPARE( DataBuffer->size(), (int)Size );
  // clean
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at end
  Target.moveToEnd( Size-1 );
  Inserted = DataBuffer->replace( Target, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  QCOMPARE( InsertData.compare(*DataBuffer,Target,0), 0 );
  QVERIFY( DataBuffer->isModified() );
  QCOMPARE( DataBuffer->size(), (int)Size );
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
  KSection Source = KSection::fromWidth( 0, ReplaceSize );
  KSection Target = KSection::fromWidth( 0, InsertSize );
  unsigned int Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( InsertData.compare(*DataBuffer,Target,0), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1-Diff),Source.end()+1), 0 );
  QVERIFY( DataBuffer->isModified() );
  // clean
  Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at middle
  Source.moveToStart( Size/2 );
  Target.moveToStart( Source.start() );
  Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  QCOMPARE( InsertData.compare(*DataBuffer,Target,0), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1-Diff),Source.end()+1), 0 );
  QVERIFY( DataBuffer->isModified() );
  // clean
  Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at end
  Source.moveToEnd( Size-1 );
  Target.moveToStart( Source.start() );
  Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  QCOMPARE( InsertData.compare(*DataBuffer,Target,0), 0 );
  QVERIFY( DataBuffer->isModified() );
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
  KFixedSizeBuffer Copy( Size + 2*InsertSize);
  DataBuffer->copyTo( Copy.rawData(), 0, Size );

  // Action: replace at begin
  KSection Source = KSection::fromWidth( 0, ReplaceSize );
  KSection Target = KSection::fromWidth( 0, InsertSize );
  unsigned int Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( InsertData.compare(*DataBuffer,Target,0), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Source.end()+1), 0 );
  QVERIFY( DataBuffer->isModified() );
  // clean
  Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );
  // Action: replace at middle
  Source.moveToStart( Size/2 );
  Target.moveToStart( Source.start() );
  Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );
  QCOMPARE( InsertData.compare(*DataBuffer,Target,0), 0 );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(Target.end()+1,Size-1),Source.end()+1), 0 );
  QVERIFY( DataBuffer->isModified() );
  // clean
  Size = DataBuffer->size();
  textureBuffer( DataBuffer, 100, 255 );
  DataBuffer->copyTo( Copy.rawData(), 0, Size );
  DataBuffer->setModified( false );

  // Action: replace at end
  Source.moveToEnd( Size-1 );
  Target.moveToStart( Source.start() );
  Inserted = DataBuffer->replace( Source, InsertData.rawData(), InsertSize );

  QCOMPARE( ReplaceSize<=Inserted && Inserted<=InsertSize, true );
  QCOMPARE( Copy.compare(*DataBuffer,KSection(0,Target.start()-1),0), 0 );

  QCOMPARE( InsertData.compare(*DataBuffer,KSection(Target.start(),DataBuffer->size()-1),0), 0 );
  QVERIFY( DataBuffer->isModified() );
}

#include "kdatabufferiftest.moc"
