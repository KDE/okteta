/***************************************************************************
                          kabstractbytearraymodeliftest.cpp  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#include "kabstractbytearraymodeliftest.h"

// test object
#include <kfixedsizebytearraymodel.h>
// test util
#include <util/fill.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

using namespace KHE;
using namespace KHECore;


#include <KDebug>
// ---------------------------------------------------------------- Tests -----


void KAbstractByteArrayModelIfTest::init()
{
  ByteArrayModel = createByteArrayModel();

  ContentsChangedSpy =  new QSignalSpy( ByteArrayModel, SIGNAL(contentsChanged(int,int)) );
  ContentsReplacedSpy = new QSignalSpy( ByteArrayModel, SIGNAL(contentsReplaced(int,int,int)) );
  ContentsSwappedSpy =    new QSignalSpy( ByteArrayModel, SIGNAL(contentsSwapped(int,int,int)) );
}

void KAbstractByteArrayModelIfTest::cleanup()
{
  deleteByteArrayModel( ByteArrayModel );

  delete ContentsChangedSpy;
  delete ContentsReplacedSpy;
  delete ContentsSwappedSpy;
}

void KAbstractByteArrayModelIfTest::clearSignalSpys()
{
  ContentsChangedSpy->clear();
  ContentsReplacedSpy->clear();
  ContentsSwappedSpy->clear();
}

void KAbstractByteArrayModelIfTest::checkContentsReplaced( int Position, int RemovedLength, int InsertedLength )
{
   QVERIFY( ContentsReplacedSpy->isValid() );
   QCOMPARE( ContentsReplacedSpy->count(), 1 );
   QList<QVariant> Arguments = ContentsReplacedSpy->takeFirst();
   QCOMPARE( Arguments.at(0).toInt(), Position );
   QCOMPARE( Arguments.at(1).toInt(), RemovedLength );
   QCOMPARE( Arguments.at(2).toInt(), InsertedLength );
}
void KAbstractByteArrayModelIfTest::checkContentsReplaced( const KHE::KSection &RemoveSection, int InsertedLength )
{ checkContentsReplaced( RemoveSection.start(), RemoveSection.width(), InsertedLength ); }

void KAbstractByteArrayModelIfTest::checkContentsSwapped( int firstStart, int secondStart, int secondLength )
{
   QVERIFY( ContentsSwappedSpy->isValid() );
   QCOMPARE( ContentsSwappedSpy->count(), 1 );
   QList<QVariant> Arguments = ContentsSwappedSpy->takeFirst();
   QCOMPARE( Arguments.at(0).toInt(), firstStart );
   QCOMPARE( Arguments.at(1).toInt(), secondStart );
   QCOMPARE( Arguments.at(2).toInt(), secondLength );
}
void KAbstractByteArrayModelIfTest::checkContentsSwapped( int firstStart, const KHE::KSection &secondSection )
{ checkContentsSwapped( firstStart, secondSection.start(), secondSection.width() ); }

void KAbstractByteArrayModelIfTest::checkContentsChanged( int Start, int End )
{
   QVERIFY( ContentsChangedSpy->isValid() );
   QCOMPARE( ContentsChangedSpy->count(), 1 );
   QList<QVariant> Arguments = ContentsChangedSpy->takeFirst();
   QCOMPARE( Arguments.at(0).toInt(), Start );
   QCOMPARE( Arguments.at(1).toInt(), End );
}
void KAbstractByteArrayModelIfTest::checkContentsChanged( const KHE::KSection &Section )
{ checkContentsChanged( Section.start(), Section.end() ); }


// ---------------------------------------------------------------- Tests -----

void KAbstractByteArrayModelIfTest::testModified()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  ByteArrayModel->setModified( false );
  QVERIFY( !ByteArrayModel->isModified() );
  ByteArrayModel->setModified( true );
  QVERIFY( ByteArrayModel->isModified() );
}

// TODO: test all edit operations to be blocked, but in their tests
void KAbstractByteArrayModelIfTest::testSetReadOnly()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  ByteArrayModel->setReadOnly( true );
  QVERIFY( ByteArrayModel->isReadOnly() );

  ByteArrayModel->setReadOnly( false );
  QVERIFY( !ByteArrayModel->isReadOnly() );
}

void KAbstractByteArrayModelIfTest::testCopyTo()
{
  if( !ByteArrayModel->isReadOnly() )
  {
    // prepare ByteArrayModel
    textureByteArrayModel( ByteArrayModel );
    ByteArrayModel->setModified( false );
  }

  //
  static const unsigned int CopySize = 10;
  unsigned int Size = ByteArrayModel->size();
  KSection CopySection( 0, CopySize-1 );

  KFixedSizeByteArrayModel Copy( Size, BlankChar );
  // copyTo() all
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  QCOMPARE( Copy.compare(*ByteArrayModel), 0 );

  // copyTo() at begin
  Copy.fill( BlankChar );

  ByteArrayModel->copyTo( Copy.rawData(), CopySection );

  QCOMPARE( Copy.compare(*ByteArrayModel, CopySection), 0 );
  QCOMPARE( Copy.datum(CopySection.behindEnd()), BlankChar );
  QVERIFY( !ByteArrayModel->isModified() );

  // copyTo() at end
  Copy.fill( BlankChar );
  CopySection.moveToEnd( Size - 1 );

  ByteArrayModel->copyTo( &Copy.rawData()[CopySection.start()], CopySection );

  QCOMPARE( Copy.datum(CopySection.beforeStart()), BlankChar );
  QCOMPARE( Copy.compare(*ByteArrayModel, CopySection, CopySection.start()), 0 );
  QVERIFY( !ByteArrayModel->isModified() );

  // copyTo() at mid
  Copy.fill( BlankChar );
  CopySection.moveToStart( Size/2 );

  ByteArrayModel->copyTo( &Copy.rawData()[CopySection.start()], CopySection );

  QCOMPARE( Copy.datum(CopySection.beforeStart()), BlankChar );
  QCOMPARE( Copy.compare(*ByteArrayModel, CopySection, CopySection.start()), 0 );
  QCOMPARE( Copy.datum(CopySection.behindEnd()), BlankChar );
  QVERIFY( !ByteArrayModel->isModified() );
}

void KAbstractByteArrayModelIfTest::testFill()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  static const unsigned int FillSize = 10;
  unsigned int Size = ByteArrayModel->size();
  KSection FillSection = KSection::fromWidth( 0, Size );

  KFixedSizeByteArrayModel Copy( Size, PaintChar );

  // fill() all
  ByteArrayModel->setModified( false );
  ByteArrayModel->fill( BlankChar );
  clearSignalSpys();

  ByteArrayModel->fill( PaintChar );
  QCOMPARE( Copy.compare(*ByteArrayModel), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( FillSection, FillSection.width() );
  checkContentsChanged( 0, ByteArrayModel->size()-1 );

  // fill() at begin
  FillSection.set( 0, FillSize );
  ByteArrayModel->setModified( false );
  ByteArrayModel->fill( BlankChar );
  clearSignalSpys();

  ByteArrayModel->fill( PaintChar, FillSection );
  QCOMPARE( Copy.compare(*ByteArrayModel, FillSection), 0 );
  QCOMPARE( ByteArrayModel->datum(FillSection.behindEnd()), BlankChar );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( FillSection, FillSection.width() );
  checkContentsChanged( FillSection );

  // fill() at end
  ByteArrayModel->setModified( false );
  ByteArrayModel->fill( BlankChar );
  FillSection.moveToEnd( Size - 1 );
  clearSignalSpys();

  ByteArrayModel->fill( PaintChar, FillSection );
  QCOMPARE( ByteArrayModel->datum(FillSection.beforeStart()), BlankChar );
  QCOMPARE( Copy.compare(*ByteArrayModel, FillSection, FillSection.start()), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( FillSection, FillSection.width() );
  checkContentsChanged( FillSection );

  // fill() at mid
  ByteArrayModel->setModified( false );
  ByteArrayModel->fill( BlankChar );
  FillSection.moveToStart( Size/2 );
  clearSignalSpys();

  ByteArrayModel->fill( PaintChar, FillSection );
  QCOMPARE( ByteArrayModel->datum(FillSection.beforeStart()), BlankChar );
  QCOMPARE( Copy.compare(*ByteArrayModel, FillSection, FillSection.start()), 0 );
  QCOMPARE( ByteArrayModel->datum(FillSection.behindEnd()), BlankChar );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( FillSection, FillSection.width() );
  checkContentsChanged( FillSection );
}

void KAbstractByteArrayModelIfTest::testSetGet()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare buffer
  unsigned int Size = ByteArrayModel->size();
  ByteArrayModel->fill( BlankChar );

  // test
  for( unsigned int i=0; i<Size; ++i )
  {
    clearSignalSpys();

    ByteArrayModel->setModified( false );

    ByteArrayModel->setDatum( i, PaintChar );

    if( i>0 )
      QCOMPARE( ByteArrayModel->datum(i-1), BlankChar  );
    QCOMPARE( ByteArrayModel->datum(i), PaintChar );
    if( i<Size-1 )
      QCOMPARE( ByteArrayModel->datum(i+1), BlankChar );
    checkContentsReplaced( i, 1, 1 );
    checkContentsChanged( i, i );

    // clean up
    ByteArrayModel->setDatum( i, BlankChar );
    QCOMPARE( ByteArrayModel->datum(i), BlankChar );
    QVERIFY( ByteArrayModel->isModified() );
  }
}

// as some buffers might be restricted in growing
// we test for the success of some operations
void KAbstractByteArrayModelIfTest::testRemove()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  textureByteArrayModel( ByteArrayModel );
  static const unsigned int RemoveSize = 10;
  // create Copy
  unsigned int Size = ByteArrayModel->size();
  KFixedSizeByteArrayModel Copy( Size );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );

  // remove() at end
  ByteArrayModel->setModified( false );
  KSection RemoveSection( Size-RemoveSize, Size-1 );
  clearSignalSpys();

  unsigned int Removed = ByteArrayModel->remove( RemoveSection );
  RemoveSection.setEndByWidth( Removed );

  QCOMPARE( Copy.compare(*ByteArrayModel,0,RemoveSection.beforeStart(),0), 0 );
  QCOMPARE( ByteArrayModel->isModified(), Removed > 0 );
  checkContentsReplaced( RemoveSection, 0 );
  checkContentsChanged( RemoveSection );

  // remove() at mid
  ByteArrayModel->setModified( false );
  Size = ByteArrayModel->size();
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  RemoveSection.setByWidth( Size/2, RemoveSize );
  clearSignalSpys();

  Removed = ByteArrayModel->remove( RemoveSection );
  RemoveSection.setEndByWidth( Removed );

  QCOMPARE( Copy.compare(*ByteArrayModel,0,RemoveSection.beforeStart(),0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(RemoveSection.start(),Size-Removed-1),RemoveSection.behindEnd()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), Removed > 0 );
  checkContentsReplaced( RemoveSection, 0 );
  checkContentsChanged( RemoveSection.start(), Size-1 );

  // remove() at start
  ByteArrayModel->setModified( false );
  Size = ByteArrayModel->size();
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  RemoveSection.setByWidth( 0, RemoveSize );
  clearSignalSpys();

  Removed = ByteArrayModel->remove( RemoveSection );
  RemoveSection.setEndByWidth( Removed );

  QCOMPARE( Copy.compare( *ByteArrayModel,KSection(RemoveSection.start(),Size-Removed-1),RemoveSection.behindEnd()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), Removed > 0 );
  checkContentsReplaced( RemoveSection, 0 );
  checkContentsChanged( RemoveSection.start(), Size-1 );
}


static const int InsertSize = 10;

struct KTestData {
  KFixedSizeByteArrayModel Copy;
  KFixedSizeByteArrayModel InsertData;

  KTestData( int CopySize, int InsertSize ) : Copy( CopySize ), InsertData( InsertSize ) {}
  const char *insertionData() { return InsertData.rawData(); }
};

KTestData *KAbstractByteArrayModelIfTest::prepareTestInsert()
{
  int Size = ByteArrayModel->size();

  KTestData *TestData = new KTestData( Size+3*InsertSize, InsertSize );

  // prepare InsertData
  textureByteArrayModel( &TestData->InsertData, 10, 99 );

  textureByteArrayModel( ByteArrayModel,100,255 );
  ByteArrayModel->copyTo( TestData->Copy.rawData(), 0, Size );
  ByteArrayModel->setModified( false );
  return TestData;
}

void KAbstractByteArrayModelIfTest::testInsertAtBegin()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare
  KTestData *Data = prepareTestInsert();
  int Size = ByteArrayModel->size();
  KSection InsertSection( 0, -1 );
  clearSignalSpys();

  int Inserted = ByteArrayModel->insert( InsertSection.start(), Data->InsertData.rawData(), InsertSize );
  InsertSection.setEndByWidth( Inserted );

  QCOMPARE( Data->InsertData.compare(*ByteArrayModel,InsertSection,0), 0 );
  QCOMPARE( Data->Copy.compare(*ByteArrayModel,InsertSection.behindEnd(),Size-InsertSection.end()-1,InsertSection.start()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), Inserted > 0 );
  checkContentsReplaced( InsertSection.start(), 0, Inserted );
  checkContentsChanged( InsertSection.start(), ByteArrayModel->size()-1 );

  delete Data;
}

void KAbstractByteArrayModelIfTest::testInsertAtMid()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare
  KTestData *Data = prepareTestInsert();
  int Size = ByteArrayModel->size();
  KSection InsertSection( Size/2, -1 );
  clearSignalSpys();

  int Inserted = ByteArrayModel->insert( InsertSection.start(), Data->InsertData.rawData(), InsertSize );
  InsertSection.setEndByWidth( Inserted );

  QCOMPARE( Data->Copy.compare(*ByteArrayModel, 0,InsertSection.start(),0), 0 );
  QCOMPARE( Data->InsertData.compare(*ByteArrayModel,InsertSection,0), 0 );
  QCOMPARE( Data->Copy.compare(*ByteArrayModel,InsertSection.behindEnd(),Size-InsertSection.end()-1,InsertSection.start()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), Inserted > 0 );
  if( Inserted > 0 )
  {
    checkContentsReplaced( InsertSection.start(), 0, Inserted );
    checkContentsChanged( InsertSection.start(), ByteArrayModel->size()-1 );
  }

  delete Data;
}

void KAbstractByteArrayModelIfTest::testInsertAtEnd()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare
  KTestData *Data = prepareTestInsert();
  int Size = ByteArrayModel->size();
  KSection InsertSection( Size, -1 );
  clearSignalSpys();

  int Inserted = ByteArrayModel->insert( InsertSection.start(), Data->InsertData.rawData(), InsertSize );
  InsertSection.setEndByWidth( Inserted );

//   list( ByteArrayModel, "ByteArrayModel" );
//   list( &Copy, "Copy" );
//   kdDebug() << QString("(%1-%2").arg(InsertSection.start()).arg(InsertSection.end()) << endl;
  QCOMPARE( Data->Copy.compare(*ByteArrayModel, 0,InsertSection.start(),0), 0 );
  if( InsertSection.isValid() )
    QCOMPARE( Data->InsertData.compare(*ByteArrayModel,InsertSection,0), 0 );
  QCOMPARE( ByteArrayModel->isModified(), Inserted > 0 );
  if( Inserted > 0 )
  {
    checkContentsReplaced( InsertSection.start(), 0, Inserted );
    checkContentsChanged( InsertSection.start(), ByteArrayModel->size()-1 );
  }

  delete Data;
}


// how the test works:
// fills the buffer with random data, puts special data at the begin
// copies Data
// moves the Data to the mid, then the start.
// tests for correct data, modified flag and size
void KAbstractByteArrayModelIfTest::testSwap()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare Copy
  static const int MoveSize = 10;
  int Size = ByteArrayModel->size();
  const KSection Origin = KSection::fromWidth( Size-MoveSize, MoveSize );
  KFixedSizeByteArrayModel Copy( Size );

  // prepare ByteArrayModel
  textureByteArrayModel( ByteArrayModel, 100, 255, 0, Origin.beforeStart() );
  textureByteArrayModel( ByteArrayModel, 10, 99, Origin );
  KSection Source = Origin;
  ByteArrayModel->setModified( false );

  // create Copy
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );

  // Action: move to middle (to left)
  int DestPos = Size/2;
  KSection Target = KSection::fromWidth( DestPos, Source.width() );
  clearSignalSpys();

  bool success = ByteArrayModel->swap( DestPos, Source );

  QVERIFY( success );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(0,Target.beforeStart()),0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,Target,Origin.start()), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(Target.behindEnd(),Size-1),Target.start()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), success );
  QCOMPARE( ByteArrayModel->size(), Size );
  checkContentsSwapped( DestPos, Source );
  checkContentsChanged( DestPos, Source.end() );

  // clean
  ByteArrayModel->setModified( false );
  Source = Target;

  // Action: move one in middle (to left)
  --DestPos;
  Target.moveToStart( DestPos );
  clearSignalSpys();

  success = ByteArrayModel->swap( DestPos, Source );

  QVERIFY( success );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(0,Target.beforeStart()),0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,Target,Origin.start()), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(Target.behindEnd(),Size-1),Target.start()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), success );
  QCOMPARE( ByteArrayModel->size(), Size );
  checkContentsSwapped( DestPos, Source );
  checkContentsChanged( DestPos, Source.end() );

  // clean
  ByteArrayModel->setModified( false );
  Source = Target;

  // Action: move to start (to left)
  DestPos = 0;
  Target.moveToStart( DestPos );
  clearSignalSpys();

  success = ByteArrayModel->swap( DestPos, Source );

  QVERIFY( success );
  QCOMPARE( Copy.compare(*ByteArrayModel,Target,Origin.start()), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(Target.behindEnd(),Size-1),Target.start()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), success );
  QCOMPARE( ByteArrayModel->size(), Size );
  checkContentsSwapped( DestPos, Source );
  checkContentsChanged( DestPos, Source.end() );

#if 0
  // clean
  ByteArrayModel->setModified( false );
  Source = Target;

  // Action: move to begin again (to left)
  DestPos = 0;
  Target = Origin;
  clearSignalSpys();

  NewPos = ByteArrayModel->move( DestPos, Source );

  QCOMPARE( NewPos, Target.start() );
  QCOMPARE( Copy.compare(*ByteArrayModel,Target,Origin.start()), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(Target.behindEnd(),Size-1),Origin.behindEnd()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), NewPos != Source.start() );
  QCOMPARE( ByteArrayModel->size(), Size );
  checkContentsSwapped( DestPos, Source );
  checkContentsChanged( DestPos, Source.end() );
#endif
}


void KAbstractByteArrayModelIfTest::testReplaceEqual()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  // create InsertData
  static const unsigned int RemoveSize = 10;
  static const unsigned int InsertSize = RemoveSize;
  KFixedSizeByteArrayModel InsertData( InsertSize );
  textureByteArrayModel( &InsertData, 10, 99 );

  // prepare ByteArrayModel
  unsigned int Size = ByteArrayModel->size();
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->setModified( false );

  // create Copy
  KFixedSizeByteArrayModel Copy( Size );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );

  // Action: move to begin again (to left)
  KSection Target = KSection::fromWidth( 0, RemoveSize );
  clearSignalSpys();

  unsigned int Inserted = ByteArrayModel->replace( Target, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( InsertData.compare(*ByteArrayModel,Target,0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(Target.behindEnd(),Size-1),Target.behindEnd()), 0 );
  QCOMPARE( ByteArrayModel->isModified(), true );
  QCOMPARE( ByteArrayModel->size(), (int)Size );
  checkContentsReplaced( Target, Inserted );
  checkContentsChanged( Target );

  // clean
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  ByteArrayModel->setModified( false );

  // Action: replace at middle
  Target.moveToStart( Size/2 );
  clearSignalSpys();

  Inserted = ByteArrayModel->replace( Target, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(0,Target.beforeStart()),0), 0 );
  QCOMPARE( InsertData.compare(*ByteArrayModel,Target,0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(Target.behindEnd(),Size-1),Target.behindEnd()), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  QCOMPARE( ByteArrayModel->size(), (int)Size );
  checkContentsReplaced( Target, Inserted );
  checkContentsChanged( Target );

  // clean
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  ByteArrayModel->setModified( false );

  // Action: replace at end
  Target.moveToEnd( Size-1 );
  clearSignalSpys();

  Inserted = ByteArrayModel->replace( Target, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(0,Target.beforeStart()),0), 0 );
  QCOMPARE( InsertData.compare(*ByteArrayModel,Target,0), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  QCOMPARE( ByteArrayModel->size(), (int)Size );
  checkContentsReplaced( Target, Inserted );
  checkContentsChanged( Target );
}


void KAbstractByteArrayModelIfTest::testReplaceLess()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  // create InsertData
  static const unsigned int RemoveSize = 10;
  static const unsigned int Diff = 4;
  static const unsigned int InsertSize = RemoveSize-Diff;
  KFixedSizeByteArrayModel InsertData( InsertSize );
  textureByteArrayModel( &InsertData, 10, 99 );

  // prepare ByteArrayModel
  int Size = ByteArrayModel->size();
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->setModified( false );

  // create Copy
  KFixedSizeByteArrayModel Copy( Size );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );

  // Action: replace at begin
  KSection RemoveSection = KSection::fromWidth( 0, RemoveSize );
  KSection InsertSection = KSection::fromWidth( 0, InsertSize );
  clearSignalSpys();

  unsigned int Inserted = ByteArrayModel->replace( RemoveSection, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( InsertData.compare(*ByteArrayModel,InsertSection,0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(InsertSection.behindEnd(),Size-1-Diff),RemoveSection.behindEnd()), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( RemoveSection, Inserted );
  checkContentsChanged( RemoveSection.start(), Size-1 );

  // clean
  Size = ByteArrayModel->size();
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  ByteArrayModel->setModified( false );

  // Action: replace at middle
  RemoveSection.moveToStart( Size/2 );
  InsertSection.moveToStart( RemoveSection.start() );
  clearSignalSpys();

  Inserted = ByteArrayModel->replace( RemoveSection, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(0,InsertSection.beforeStart()),0), 0 );
  QCOMPARE( InsertData.compare(*ByteArrayModel,InsertSection,0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(InsertSection.behindEnd(),Size-1-Diff),RemoveSection.behindEnd()), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( RemoveSection, Inserted );
  checkContentsChanged( RemoveSection.start(), Size-1 );

  // clean
  Size = ByteArrayModel->size();
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  ByteArrayModel->setModified( false );

  // Action: replace at end
  RemoveSection.moveToEnd( Size-1 );
  InsertSection.moveToStart( RemoveSection.start() );
  clearSignalSpys();

  Inserted = ByteArrayModel->replace( RemoveSection, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(0,InsertSection.beforeStart()),0), 0 );
  QCOMPARE( InsertData.compare(*ByteArrayModel,InsertSection,0), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( RemoveSection, Inserted );
  checkContentsChanged( RemoveSection );
}


void KAbstractByteArrayModelIfTest::testReplaceMore()
{
  // can we alter the buffer at all?
  if( ByteArrayModel->isReadOnly() )
    // skip
    return;

  // create InsertData
  static const unsigned int RemoveSize = 10;
  static const unsigned int Diff = 4;
  static const unsigned int InsertSize = RemoveSize+Diff;
  KFixedSizeByteArrayModel InsertData( InsertSize );
  textureByteArrayModel( &InsertData, 10, 99 );

  // prepare ByteArrayModel
  int Size = ByteArrayModel->size();
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->setModified( false );

  // create Copy
  KFixedSizeByteArrayModel Copy( Size + 2*InsertSize);
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );

  // Action: replace at begin
  KSection RemoveSection = KSection::fromWidth( 0, RemoveSize );
  KSection InsertSection = KSection::fromWidth( 0, InsertSize );
  clearSignalSpys();

  unsigned int Inserted = ByteArrayModel->replace( RemoveSection, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( InsertData.compare(*ByteArrayModel,InsertSection,0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(InsertSection.behindEnd(),Size-1),RemoveSection.behindEnd()), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( RemoveSection, Inserted );
  checkContentsChanged( RemoveSection.start(), ByteArrayModel->size()-1 );

  // clean
  Size = ByteArrayModel->size();
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  ByteArrayModel->setModified( false );
  // Action: replace at middle
  RemoveSection.moveToStart( Size/2 );
  InsertSection.moveToStart( RemoveSection.start() );
  clearSignalSpys();

  Inserted = ByteArrayModel->replace( RemoveSection, InsertData.rawData(), InsertSize );

  QCOMPARE( Inserted, InsertSize );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(0,InsertSection.beforeStart()),0), 0 );
  QCOMPARE( InsertData.compare(*ByteArrayModel,InsertSection,0), 0 );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(InsertSection.behindEnd(),Size-1),RemoveSection.behindEnd()), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( RemoveSection, Inserted );
  checkContentsChanged( RemoveSection.start(), ByteArrayModel->size()-1 );

  // clean
  Size = ByteArrayModel->size();
  textureByteArrayModel( ByteArrayModel, 100, 255 );
  ByteArrayModel->copyTo( Copy.rawData(), 0, Size );
  ByteArrayModel->setModified( false );

  // Action: replace at end
  RemoveSection.moveToEnd( Size-1 );
  InsertSection.moveToStart( RemoveSection.start() );
  clearSignalSpys();

  Inserted = ByteArrayModel->replace( RemoveSection, InsertData.rawData(), InsertSize );

  QCOMPARE( RemoveSize<=Inserted && Inserted<=InsertSize, true );
  QCOMPARE( Copy.compare(*ByteArrayModel,KSection(0,InsertSection.beforeStart()),0), 0 );

  QCOMPARE( InsertData.compare(*ByteArrayModel,KSection(InsertSection.start(),ByteArrayModel->size()-1),0), 0 );
  QVERIFY( ByteArrayModel->isModified() );
  checkContentsReplaced( RemoveSection, Inserted );
  checkContentsChanged( RemoveSection.start(), ByteArrayModel->size()-1 );
}

#include "kabstractbytearraymodeliftest.moc"
