/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractbytearraymodeliftest.h"

// test object
#include <fixedsizebytearraymodel.h>
// test util
#include <util/fill.h>
// lib
#include <arraychangemetricslist.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>


Q_DECLARE_METATYPE(Okteta::ArrayChangeMetricsList)

namespace Okteta
{

// ---------------------------------------------------------------- Tests -----


void AbstractByteArrayModelIfTest::init()
{
    mByteArrayModel = createByteArrayModel();

    qRegisterMetaType<ArrayChangeMetricsList>( "Okteta::ArrayChangeMetricsList" );
    ContentsChangeListSpy = new QSignalSpy( mByteArrayModel, SIGNAL(contentsChanged(Okteta::ArrayChangeMetricsList)) );
}

void AbstractByteArrayModelIfTest::cleanup()
{
    deleteByteArrayModel( mByteArrayModel );

    delete ContentsChangeListSpy;
}

void AbstractByteArrayModelIfTest::clearSignalSpys()
{
    ContentsChangeListSpy->clear();
}

void AbstractByteArrayModelIfTest::checkContentsReplaced( Address offset, Size removeLength, int insertLength )
{
   QVERIFY( ContentsChangeListSpy->isValid() );
   QCOMPARE( ContentsChangeListSpy->count(), 1 );
   const QList<QVariant> arguments = ContentsChangeListSpy->takeFirst();
   const ArrayChangeMetrics changeMetrics = (qvariant_cast<ArrayChangeMetricsList>(arguments.at(0))).at(0);
   QCOMPARE( changeMetrics.offset(), offset );
   QCOMPARE( changeMetrics.removeLength(), removeLength );
   QCOMPARE( changeMetrics.insertLength(), insertLength );
}
void AbstractByteArrayModelIfTest::checkContentsReplaced( const AddressRange& removeRange, int insertLength )
{ checkContentsReplaced( removeRange.start(), removeRange.width(), insertLength ); }

void AbstractByteArrayModelIfTest::checkContentsSwapped( Address firstStart, Address secondStart, Size secondLength )
{
   QVERIFY( ContentsChangeListSpy->isValid() );
   QCOMPARE( ContentsChangeListSpy->count(), 1 );
   const QList<QVariant> arguments = ContentsChangeListSpy->takeFirst();
   const ArrayChangeMetrics changeMetrics = (qvariant_cast<ArrayChangeMetricsList>(arguments.at(0))).at(0);
   QCOMPARE( changeMetrics.offset(), firstStart );
   QCOMPARE( changeMetrics.secondStart(), secondStart );
   QCOMPARE( changeMetrics.secondLength(), secondLength );
}
void AbstractByteArrayModelIfTest::checkContentsSwapped( Address firstStart, const AddressRange& secondSection )
{ checkContentsSwapped( firstStart, secondSection.start(), secondSection.width() ); }


// ---------------------------------------------------------------- Tests -----

void AbstractByteArrayModelIfTest::testModified()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  mByteArrayModel->setModified( false );
  QVERIFY( !mByteArrayModel->isModified() );
  mByteArrayModel->setModified( true );
  QVERIFY( mByteArrayModel->isModified() );
}

// TODO: test all edit operations to be blocked, but in their tests
void AbstractByteArrayModelIfTest::testSetReadOnly()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  mByteArrayModel->setReadOnly( true );
  QVERIFY( mByteArrayModel->isReadOnly() );

  mByteArrayModel->setReadOnly( false );
  QVERIFY( !mByteArrayModel->isReadOnly() );
}

void AbstractByteArrayModelIfTest::testCopyTo()
{
  if( !mByteArrayModel->isReadOnly() )
  {
    // prepare mByteArrayModel
    textureByteArrayModel( mByteArrayModel );
    mByteArrayModel->setModified( false );
  }

  //
  static const Size copySize = 10;
  Size size = mByteArrayModel->size();
  AddressRange copyRange( 0, copySize-1 );

  FixedSizeByteArrayModel copy( size, BlankChar );
  // copyTo() all
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  QCOMPARE( copy.compare(*mByteArrayModel), 0 );

  // copyTo() at begin
  copy.fill( BlankChar );

  mByteArrayModel->copyTo( copy.rawData(), copyRange );

  QCOMPARE( copy.compare(*mByteArrayModel, copyRange), 0 );
  QCOMPARE( copy.byte(copyRange.nextBehindEnd()), BlankChar );
  QVERIFY( !mByteArrayModel->isModified() );

  // copyTo() at end
  copy.fill( BlankChar );
  copyRange.moveToEnd( size - 1 );

  mByteArrayModel->copyTo( &copy.rawData()[copyRange.start()], copyRange );

  QCOMPARE( copy.byte(copyRange.nextBeforeStart()), BlankChar );
  QCOMPARE( copy.compare(*mByteArrayModel, copyRange, copyRange.start()), 0 );
  QVERIFY( !mByteArrayModel->isModified() );

  // copyTo() at mid
  copy.fill( BlankChar );
  copyRange.moveToStart( size/2 );

  mByteArrayModel->copyTo( &copy.rawData()[copyRange.start()], copyRange );

  QCOMPARE( copy.byte(copyRange.nextBeforeStart()), BlankChar );
  QCOMPARE( copy.compare(*mByteArrayModel, copyRange, copyRange.start()), 0 );
  QCOMPARE( copy.byte(copyRange.nextBehindEnd()), BlankChar );
  QVERIFY( !mByteArrayModel->isModified() );
}

void AbstractByteArrayModelIfTest::testFill()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  static const Size fillSize = 10;
  Size size = mByteArrayModel->size();
  AddressRange fillRange = AddressRange::fromWidth( 0, size );

  FixedSizeByteArrayModel copy( size, PaintChar );

  // fill() all
  mByteArrayModel->setModified( false );
  mByteArrayModel->fill( BlankChar );
  clearSignalSpys();

  mByteArrayModel->fill( PaintChar );
  QCOMPARE( copy.compare(*mByteArrayModel), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( fillRange, fillRange.width() );

  // fill() at begin
  fillRange.set( 0, fillSize );
  mByteArrayModel->setModified( false );
  mByteArrayModel->fill( BlankChar );
  clearSignalSpys();

  mByteArrayModel->fill( PaintChar, fillRange );
  QCOMPARE( copy.compare(*mByteArrayModel, fillRange), 0 );
  QCOMPARE( mByteArrayModel->byte(fillRange.nextBehindEnd()), BlankChar );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( fillRange, fillRange.width() );

  // fill() at end
  mByteArrayModel->setModified( false );
  mByteArrayModel->fill( BlankChar );
  fillRange.moveToEnd( size - 1 );
  clearSignalSpys();

  mByteArrayModel->fill( PaintChar, fillRange );
  QCOMPARE( mByteArrayModel->byte(fillRange.nextBeforeStart()), BlankChar );
  QCOMPARE( copy.compare(*mByteArrayModel, fillRange, fillRange.start()), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( fillRange, fillRange.width() );

  // fill() at mid
  mByteArrayModel->setModified( false );
  mByteArrayModel->fill( BlankChar );
  fillRange.moveToStart( size/2 );
  clearSignalSpys();

  mByteArrayModel->fill( PaintChar, fillRange );
  QCOMPARE( mByteArrayModel->byte(fillRange.nextBeforeStart()), BlankChar );
  QCOMPARE( copy.compare(*mByteArrayModel, fillRange, fillRange.start()), 0 );
  QCOMPARE( mByteArrayModel->byte(fillRange.nextBehindEnd()), BlankChar );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( fillRange, fillRange.width() );
}

void AbstractByteArrayModelIfTest::testSetGet()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare buffer
  Size size = mByteArrayModel->size();
  mByteArrayModel->fill( BlankChar );

  // test
  for( Address i=0; i<size; ++i )
  {
    clearSignalSpys();

    mByteArrayModel->setModified( false );

    mByteArrayModel->setByte( i, PaintChar );

    if( i>0 )
      QCOMPARE( mByteArrayModel->byte(i-1), BlankChar  );
    QCOMPARE( mByteArrayModel->byte(i), PaintChar );
    if( i<size-1 )
      QCOMPARE( mByteArrayModel->byte(i+1), BlankChar );
    checkContentsReplaced( i, 1, 1 );

    // clean up
    mByteArrayModel->setByte( i, BlankChar );
    QCOMPARE( mByteArrayModel->byte(i), BlankChar );
    QVERIFY( mByteArrayModel->isModified() );
  }
}

// as some buffers might be restricted in growing
// we test for the success of some operations
void AbstractByteArrayModelIfTest::testRemove()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  textureByteArrayModel( mByteArrayModel );
  static const Size removeSize = 10;
  // create copy
  Size size = mByteArrayModel->size();
  FixedSizeByteArrayModel copy( size );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );

  // remove() at end
  mByteArrayModel->setModified( false );
  AddressRange removeRange( size-removeSize, size-1 );
  clearSignalSpys();

  Size removed = mByteArrayModel->remove( removeRange );
  removeRange.setEndByWidth( removed );

  QCOMPARE( copy.compare(*mByteArrayModel,0,removeRange.nextBeforeStart(),0), 0 );
  QCOMPARE( mByteArrayModel->isModified(), removed > 0 );
  checkContentsReplaced( removeRange, 0 );

  // remove() at mid
  mByteArrayModel->setModified( false );
  size = mByteArrayModel->size();
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  removeRange.setByWidth( size/2, removeSize );
  clearSignalSpys();

  removed = mByteArrayModel->remove( removeRange );
  removeRange.setEndByWidth( removed );

  QCOMPARE( copy.compare(*mByteArrayModel,0,removeRange.nextBeforeStart(),0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(removeRange.start(),size-removed-1),removeRange.nextBehindEnd()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), removed > 0 );
  checkContentsReplaced( removeRange, 0 );

  // remove() at start
  mByteArrayModel->setModified( false );
  size = mByteArrayModel->size();
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  removeRange.setByWidth( 0, removeSize );
  clearSignalSpys();

  removed = mByteArrayModel->remove( removeRange );
  removeRange.setEndByWidth( removed );

  QCOMPARE( copy.compare( *mByteArrayModel,AddressRange(removeRange.start(),size-removed-1),removeRange.nextBehindEnd()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), removed > 0 );
  checkContentsReplaced( removeRange, 0 );
}


static const Size insertSize = 10;

struct KTestData {
  FixedSizeByteArrayModel copy;
  FixedSizeByteArrayModel insertData;

  KTestData( Size copySize, Size insertSize ) : copy( copySize ), insertData( insertSize ) {}
  const Byte* insertionData() { return insertData.rawData(); }
};

KTestData* AbstractByteArrayModelIfTest::prepareTestInsert()
{
  Size size = mByteArrayModel->size();

  KTestData* testData = new KTestData( size+3*insertSize, insertSize );

  // prepare insertData
  textureByteArrayModel( &testData->insertData, 10, 99 );

  textureByteArrayModel( mByteArrayModel,100,255 );
  mByteArrayModel->copyTo( testData->copy.rawData(), 0, size );
  mByteArrayModel->setModified( false );
  return testData;
}

void AbstractByteArrayModelIfTest::testInsertAtBegin()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare
  KTestData* data = prepareTestInsert();
  Size size = mByteArrayModel->size();
  AddressRange insertRange( 0, -1 );
  clearSignalSpys();

  Size inserted = mByteArrayModel->insert( insertRange.start(), data->insertData.rawData(), insertSize );
  insertRange.setEndByWidth( inserted );

  QCOMPARE( data->insertData.compare(*mByteArrayModel,insertRange,0), 0 );
  QCOMPARE( data->copy.compare(*mByteArrayModel,insertRange.nextBehindEnd(),size-insertRange.end()-1,insertRange.start()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), inserted > 0 );
  checkContentsReplaced( insertRange.start(), 0, inserted );

  delete data;
}

void AbstractByteArrayModelIfTest::testInsertAtMid()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare
  KTestData* data = prepareTestInsert();
  Size size = mByteArrayModel->size();
  AddressRange insertRange( size/2, -1 );
  clearSignalSpys();

  Size inserted = mByteArrayModel->insert( insertRange.start(), data->insertData.rawData(), insertSize );
  insertRange.setEndByWidth( inserted );

  QCOMPARE( data->copy.compare(*mByteArrayModel, 0,insertRange.start(),0), 0 );
  QCOMPARE( data->insertData.compare(*mByteArrayModel,insertRange,0), 0 );
  QCOMPARE( data->copy.compare(*mByteArrayModel,insertRange.nextBehindEnd(),size-insertRange.end()-1,insertRange.start()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), inserted > 0 );
  if( inserted > 0 )
  {
    checkContentsReplaced( insertRange.start(), 0, inserted );
  }

  delete data;
}

void AbstractByteArrayModelIfTest::testInsertAtEnd()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare
  KTestData* data = prepareTestInsert();
  Size size = mByteArrayModel->size();
  AddressRange insertRange( size, -1 );
  clearSignalSpys();

  Size inserted = mByteArrayModel->insert( insertRange.start(), data->insertData.rawData(), insertSize );
  insertRange.setEndByWidth( inserted );

//   list( mByteArrayModel, "mByteArrayModel" );
//   list( &copy, "copy" );
//   kdDebug() << QString("(%1-%2").arg(insertRange.start()).arg(insertRange.end()) << endl;
  QCOMPARE( data->copy.compare(*mByteArrayModel, 0,insertRange.start(),0), 0 );
  if( insertRange.isValid() )
    QCOMPARE( data->insertData.compare(*mByteArrayModel,insertRange,0), 0 );
  QCOMPARE( mByteArrayModel->isModified(), inserted > 0 );
  if( inserted > 0 )
  {
    checkContentsReplaced( insertRange.start(), 0, inserted );
  }

  delete data;
}


// how the test works:
// fills the buffer with random data, puts special data at the begin
// copies data
// moves the data to the mid, then the start.
// tests for correct data, modified flag and size
void AbstractByteArrayModelIfTest::testSwap()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  // prepare copy
  static const Size moveSize = 10;
  Size size = mByteArrayModel->size();
  const AddressRange origin = AddressRange::fromWidth( size-moveSize, moveSize );
  FixedSizeByteArrayModel copy( size );

  // prepare mByteArrayModel
  textureByteArrayModel( mByteArrayModel, 100, 255, 0, origin.nextBeforeStart() );
  textureByteArrayModel( mByteArrayModel, 10, 99, origin );
  AddressRange Source = origin;
  mByteArrayModel->setModified( false );

  // create copy
  mByteArrayModel->copyTo( copy.rawData(), 0, size );

  // Action: move to middle (to left)
  Address destOffset = size/2;
  AddressRange targetRange = AddressRange::fromWidth( destOffset, Source.width() );
  clearSignalSpys();

  bool success = mByteArrayModel->swap( destOffset, Source );

  QVERIFY( success );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(0,targetRange.nextBeforeStart()),0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,targetRange,origin.start()), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(targetRange.nextBehindEnd(),size-1),targetRange.start()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), success );
  QCOMPARE( mByteArrayModel->size(), size );
  checkContentsSwapped( destOffset, Source );

  // clean
  mByteArrayModel->setModified( false );
  Source = targetRange;

  // Action: move one in middle (to left)
  --destOffset;
  targetRange.moveToStart( destOffset );
  clearSignalSpys();

  success = mByteArrayModel->swap( destOffset, Source );

  QVERIFY( success );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(0,targetRange.nextBeforeStart()),0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,targetRange,origin.start()), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(targetRange.nextBehindEnd(),size-1),targetRange.start()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), success );
  QCOMPARE( mByteArrayModel->size(), size );
  checkContentsSwapped( destOffset, Source );

  // clean
  mByteArrayModel->setModified( false );
  Source = targetRange;

  // Action: move to start (to left)
  destOffset = 0;
  targetRange.moveToStart( destOffset );
  clearSignalSpys();

  success = mByteArrayModel->swap( destOffset, Source );

  QVERIFY( success );
  QCOMPARE( copy.compare(*mByteArrayModel,targetRange,origin.start()), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(targetRange.nextBehindEnd(),size-1),targetRange.start()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), success );
  QCOMPARE( mByteArrayModel->size(), size );
  checkContentsSwapped( destOffset, Source );

#if 0
  // clean
  mByteArrayModel->setModified( false );
  Source = targetRange;

  // Action: move to begin again (to left)
  destOffset = 0;
  targetRange = origin;
  clearSignalSpys();

  NewPos = mByteArrayModel->move( destOffset, Source );

  QCOMPARE( NewPos, targetRange.start() );
  QCOMPARE( copy.compare(*mByteArrayModel,targetRange,origin.start()), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(targetRange.nextBehindEnd(),size-1),origin.nextBehindEnd()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), NewPos != Source.start() );
  QCOMPARE( mByteArrayModel->size(), size );
  checkContentsSwapped( destOffset, Source );
#endif
}


void AbstractByteArrayModelIfTest::testReplaceEqual()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  // create insertData
  static const Size removeSize = 10;
  static const Size insertSize = removeSize;
  FixedSizeByteArrayModel insertData( insertSize );
  textureByteArrayModel( &insertData, 10, 99 );

  // prepare mByteArrayModel
  Size size = mByteArrayModel->size();
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->setModified( false );

  // create copy
  FixedSizeByteArrayModel copy( size );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );

  // Action: move to begin again (to left)
  AddressRange targetRange = AddressRange::fromWidth( 0, removeSize );
  clearSignalSpys();

  Size inserted = mByteArrayModel->replace( targetRange, insertData.rawData(), insertSize );

  QCOMPARE( inserted, insertSize );
  QCOMPARE( insertData.compare(*mByteArrayModel,targetRange,0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(targetRange.nextBehindEnd(),size-1),targetRange.nextBehindEnd()), 0 );
  QCOMPARE( mByteArrayModel->isModified(), true );
  QCOMPARE( mByteArrayModel->size(), size );
  checkContentsReplaced( targetRange, inserted );

  // clean
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  mByteArrayModel->setModified( false );

  // Action: replace at middle
  targetRange.moveToStart( size/2 );
  clearSignalSpys();

  inserted = mByteArrayModel->replace( targetRange, insertData.rawData(), insertSize );

  QCOMPARE( inserted, insertSize );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(0,targetRange.nextBeforeStart()),0), 0 );
  QCOMPARE( insertData.compare(*mByteArrayModel,targetRange,0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(targetRange.nextBehindEnd(),size-1),targetRange.nextBehindEnd()), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  QCOMPARE( mByteArrayModel->size(), size );
  checkContentsReplaced( targetRange, inserted );

  // clean
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  mByteArrayModel->setModified( false );

  // Action: replace at end
  targetRange.moveToEnd( size-1 );
  clearSignalSpys();

  inserted = mByteArrayModel->replace( targetRange, insertData.rawData(), insertSize );

  QCOMPARE( inserted, insertSize );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(0,targetRange.nextBeforeStart()),0), 0 );
  QCOMPARE( insertData.compare(*mByteArrayModel,targetRange,0), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  QCOMPARE( mByteArrayModel->size(), size );
  checkContentsReplaced( targetRange, inserted );
}


void AbstractByteArrayModelIfTest::testReplaceLess()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  // create insertData
  static const Size removeSize = 10;
  static const Size diff = 4;
  static const Size insertSize = removeSize-diff;
  FixedSizeByteArrayModel insertData( insertSize );
  textureByteArrayModel( &insertData, 10, 99 );

  // prepare mByteArrayModel
  Size size = mByteArrayModel->size();
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->setModified( false );

  // create copy
  FixedSizeByteArrayModel copy( size );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );

  // Action: replace at begin
  AddressRange removeRange = AddressRange::fromWidth( 0, removeSize );
  AddressRange insertRange = AddressRange::fromWidth( 0, insertSize );
  clearSignalSpys();

  Size inserted = mByteArrayModel->replace( removeRange, insertData.rawData(), insertSize );

  QCOMPARE( inserted, insertSize );
  QCOMPARE( insertData.compare(*mByteArrayModel,insertRange,0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(insertRange.nextBehindEnd(),size-1-diff),removeRange.nextBehindEnd()), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( removeRange, inserted );

  // clean
  size = mByteArrayModel->size();
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  mByteArrayModel->setModified( false );

  // Action: replace at middle
  removeRange.moveToStart( size/2 );
  insertRange.moveToStart( removeRange.start() );
  clearSignalSpys();

  inserted = mByteArrayModel->replace( removeRange, insertData.rawData(), insertSize );

  QCOMPARE( inserted, insertSize );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(0,insertRange.nextBeforeStart()),0), 0 );
  QCOMPARE( insertData.compare(*mByteArrayModel,insertRange,0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(insertRange.nextBehindEnd(),size-1-diff),removeRange.nextBehindEnd()), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( removeRange, inserted );

  // clean
  size = mByteArrayModel->size();
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  mByteArrayModel->setModified( false );

  // Action: replace at end
  removeRange.moveToEnd( size-1 );
  insertRange.moveToStart( removeRange.start() );
  clearSignalSpys();

  inserted = mByteArrayModel->replace( removeRange, insertData.rawData(), insertSize );

  QCOMPARE( inserted, insertSize );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(0,insertRange.nextBeforeStart()),0), 0 );
  QCOMPARE( insertData.compare(*mByteArrayModel,insertRange,0), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( removeRange, inserted );
}


void AbstractByteArrayModelIfTest::testReplaceMore()
{
  // can we alter the buffer at all?
  if( mByteArrayModel->isReadOnly() )
    // skip
    return;

  // create insertData
  static const Size removeSize = 10;
  static const Size diff = 4;
  static const Size insertSize = removeSize+diff;
  FixedSizeByteArrayModel insertData( insertSize );
  textureByteArrayModel( &insertData, 10, 99 );

  // prepare mByteArrayModel
  Size size = mByteArrayModel->size();
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->setModified( false );

  // create copy
  FixedSizeByteArrayModel copy( size + 2*insertSize);
  mByteArrayModel->copyTo( copy.rawData(), 0, size );

  // Action: replace at begin
  AddressRange removeRange = AddressRange::fromWidth( 0, removeSize );
  AddressRange insertRange = AddressRange::fromWidth( 0, insertSize );
  clearSignalSpys();

  Size inserted = mByteArrayModel->replace( removeRange, insertData.rawData(), insertSize );

  QCOMPARE( inserted, insertSize );
  QCOMPARE( insertData.compare(*mByteArrayModel,insertRange,0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(insertRange.nextBehindEnd(),size-1),removeRange.nextBehindEnd()), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( removeRange, inserted );

  // clean
  size = mByteArrayModel->size();
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  mByteArrayModel->setModified( false );
  // Action: replace at middle
  removeRange.moveToStart( size/2 );
  insertRange.moveToStart( removeRange.start() );
  clearSignalSpys();

  inserted = mByteArrayModel->replace( removeRange, insertData.rawData(), insertSize );

  QCOMPARE( inserted, insertSize );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(0,insertRange.nextBeforeStart()),0), 0 );
  QCOMPARE( insertData.compare(*mByteArrayModel,insertRange,0), 0 );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(insertRange.nextBehindEnd(),size-1),removeRange.nextBehindEnd()), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( removeRange, inserted );

  // clean
  size = mByteArrayModel->size();
  textureByteArrayModel( mByteArrayModel, 100, 255 );
  mByteArrayModel->copyTo( copy.rawData(), 0, size );
  mByteArrayModel->setModified( false );

  // Action: replace at end
  removeRange.moveToEnd( size-1 );
  insertRange.moveToStart( removeRange.start() );
  clearSignalSpys();

  inserted = mByteArrayModel->replace( removeRange, insertData.rawData(), insertSize );

  QCOMPARE( removeSize<=inserted && inserted<=insertSize, true );
  QCOMPARE( copy.compare(*mByteArrayModel,AddressRange(0,insertRange.nextBeforeStart()),0), 0 );

  QCOMPARE( insertData.compare(*mByteArrayModel,AddressRange(insertRange.start(),mByteArrayModel->size()-1),0), 0 );
  QVERIFY( mByteArrayModel->isModified() );
  checkContentsReplaced( removeRange, inserted );
}

}
