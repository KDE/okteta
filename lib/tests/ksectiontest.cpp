/***************************************************************************
                          ksectiontest.cpp -  description
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
// test specific
#include <ksection.h>
#include "ksectiontest.h"

using namespace KHE;

// local variables
static const int Start = 15;
static const int End = 27;

static const int Width = End - Start + 1;


void KSectionTest::testConstructorByEnd()
{
  KSection Section( Start, End );
  QCOMPARE( Section.start(), Start );
  QCOMPARE( Section.end(),   End );
  QCOMPARE( Section.width(), Width );
}

void KSectionTest::testConstructorByWidth()
{
  KSection Section = KSection::fromWidth( Start, Width );
  QCOMPARE( Section.start(), Start );
  QCOMPARE( Section.end(), End );
  QCOMPARE( Section.width(), Width );
}

void KSectionTest::testSimpleConstructor()
{
  KSection Section;
  QVERIFY( Section.isEmpty() );
}


void KSectionTest::testSetGetStart()
{
  KSection Section;
  Section.setStart( Start );
  QCOMPARE( Section.start(), Start );
}

void KSectionTest::testSetGetEnd()
{
  KSection Section;
  Section.setEnd( End );
  QCOMPARE( Section.end(), End );
}

void KSectionTest::testSetGetWidth()
{
  KSection Section;
  // end, width by width
  Section.setStart( Start );
  Section.setEndByWidth( Width );
  QCOMPARE( Section.end(), End );
  QCOMPARE( Section.width(), Width );
  // start, width by width
  Section.setEnd( End );
  Section.setStartByWidth( Width );
  QCOMPARE( Section.start(), Start );
  QCOMPARE( Section.width(), Width );
}

void KSectionTest::testIsEmpty()
{
  KSection Section( Start, End );
  QVERIFY( !Section.isEmpty() );
  Section.unset();
  QVERIFY( Section.isEmpty() );
}

void KSectionTest::testCompare()
{
  KSection Section( Start, End );
  // same
  KSection OtherSection( Start, End );
  QVERIFY( Section == OtherSection );
  // different start
  OtherSection.set(Start+1, End);
  QVERIFY( !(Section == OtherSection) );
  // different end
  OtherSection.set(Start, End+1);
  QVERIFY( !(Section == OtherSection) );
}

void KSectionTest::testIncludes()
{
  KSection Section( Start, End );
  // at start
  int i = Section.start();
  QVERIFY( Section.includes(i) );
  // before start
  --i;
  QVERIFY( !Section.includes(i) );
  // at end
  i = Section.end();
  QVERIFY( Section.includes(i) );
  // behind end
  ++i;
  QVERIFY( !Section.includes(i) );
}

void KSectionTest::testAdaptToChange()
{
  // adaptToChange, same length, behind
  KSection Section( Start, End );
  KSection OtherSection( Section );
  Section.adaptToChange( End+1, Width, Width );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, same length, before
  Section.set( Start, End );
  OtherSection.set( Section );
  Section.adaptToChange( Start-2, 1, 1 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, same length, -1,-1
  Section.set( Start-1, End-1 );
  OtherSection.set( Start-1, Start-1 );
  Section.adaptToChange( Start, Width, Width );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, same length, -1,0
  Section.set( Start-1, End );
  OtherSection.set( Section );
  Section.adaptToChange( Start, Width, Width );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, same length, -1,+1
  Section.set( Start-1, End+1 );
  OtherSection.set( Section );
  Section.adaptToChange( Start, Width, Width );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, same length, 0,-1
  Section.set( Start, End-1 );
  OtherSection.set( Section );
  Section.adaptToChange( Start, Width, Width );
  QVERIFY( !Section.isValid() );

  // adaptToChange, same length, 0,0
  Section.set( Start, End );
  OtherSection.set( Section );
  Section.adaptToChange( Start, Width, Width );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, same length, 0,+1
  Section.set( Start, End+1 );
  OtherSection.set( Section );
  Section.adaptToChange( Start, Width, Width );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, same length, +1,-1
  Section.set( Start+1, End-1 );
  Section.adaptToChange( Start, Width, Width );
  QVERIFY( !Section.isValid() );

  // adaptToChange, same length, +1,0
  Section.set( Start+1, End );
  Section.adaptToChange( Start, Width, Width );
  QVERIFY( !Section.isValid() );

  // adaptToChange, same length, +1,+1
  Section.set( Start+1, End+1 );
  OtherSection.set( End+1, End+1 );
  Section.adaptToChange( Start, Width, Width );
  QCOMPARE( Section, OtherSection );


  // adaptToChange, less length, behind
  Section.set( Start, End );
  OtherSection.set( Section );
  Section.adaptToChange( End+1, Width, Width-2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, less length, before
  Section.set( Start, End );
  OtherSection.set( Start-2, End-2 );
  Section.adaptToChange( Start-Width, Width, Width-2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, less length, -1,-1
  Section.set( Start-1, End-1 );
  OtherSection.set( Start-1, Start-1 );
  Section.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, less length, -1,0
  Section.set( Start-1, End );
  OtherSection.set( Start-1, End-2 );
  Section.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, less length, -1,+1
  Section.set( Start-1, End+1 );
  OtherSection.set( Start-1, End-1 );
  Section.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, less length, 0,-1
  Section.set( Start, End-1 );
  Section.adaptToChange( Start, Width, Width-2 );
  QVERIFY( !Section.isValid() );

  // adaptToChange, less length, 0,0
  Section.set( Start, End );
  OtherSection.set( Start, End-2 );
  Section.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, less length, 0,+1
  Section.set( Start, End+1 );
  OtherSection.set( Start, End-1 );
  Section.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, less length, +1,-1
  Section.set( Start+1, End-1 );
  Section.adaptToChange( Start, Width, Width-2 );
  QVERIFY( !Section.isValid() );

  // adaptToChange, less length, +1,0
  Section.set( Start+1, End );
  Section.adaptToChange( Start, Width, Width-2 );
  QVERIFY( !Section.isValid() );

  // adaptToChange, less length, +1,+1
  Section.set( Start+1, End+1 );
  OtherSection.set( End-1, End-1 );
  Section.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( Section, OtherSection );


  // adaptToChange, greater length, behind
  Section.set( Start, End );
  OtherSection.set( Section );
  Section.adaptToChange( End+1, Width, Width+2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, greater length, before
  Section.set( Start, End );
  OtherSection.set( Start+2, End+2 );
  Section.adaptToChange( Start-Width, Width, Width+2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, greater length, -1,-1
  Section.set( Start-1, End-1 );
  OtherSection.set( Start-1, Start-1 );
  Section.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, greater length, -1,0
  Section.set( Start-1, End );
  OtherSection.set( Start-1, End+2 );
  Section.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, greater length, -1,+1
  Section.set( Start-1, End+1 );
  OtherSection.set( Start-1, End+3 );
  Section.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, greater length, 0,-1
  Section.set( Start, End-1 );
  Section.adaptToChange( Start, Width, Width+2 );
  QVERIFY( !Section.isValid() );

  // adaptToChange, greater length, 0,0
  Section.set( Start, End );
  OtherSection.set( Start, End+2 );
  Section.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, greater length, 0,+1
  Section.set( Start, End+1 );
  OtherSection.set( Start, End+3 );
  Section.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, greater length, +1,-1
  Section.set( Start+1, End-1 );
  Section.adaptToChange( Start, Width, Width+2 );
  QVERIFY( !Section.isValid() );

  // adaptToChange, greater length, +1,0
  Section.set( Start+1, End );
  Section.adaptToChange( Start, Width, Width+2 );
  QVERIFY( !Section.isValid() );

  // adaptToChange, greater length, +1,+1
  Section.set( Start+1, End+1 );
  OtherSection.set( End+3, End+3 );
  Section.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( Section, OtherSection );


  // adaptToChange, insert, behind
  Section.set( Start-1, Start-1 );
  OtherSection.set( Start-1, End );
  Section.adaptToChange( Start, 0, Width );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, insert, middle
  Section.set( Start-1, Start );
  OtherSection.set( Start-1, End+1 );
  Section.adaptToChange( Start, 0, Width );
  QCOMPARE( Section, OtherSection );

  // adaptToChange, insert, before
  Section.set( Start, Start );
  OtherSection.set( End+1, End+1 );
  Section.adaptToChange( Start, 0, Width );
  QCOMPARE( Section, OtherSection );
}

void KSectionTest::testStartForInclude()
{
  // testStartForInclude, same length, start at start
  const KSection Section( Start, End );
  KSection OtherSection( Section );
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, same length, start before start
  OtherSection.moveBy( -1 );
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, same length, end behind end
  OtherSection.moveBy( 2 );
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, start at start
  OtherSection.set( Start, End-1 );
  int Diff = Section.width() - OtherSection.width();
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, start before start
  OtherSection.moveBy( -1 );
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, end behind end
  OtherSection.moveBy( 2 );
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start()-Diff );

/* // undefined in startForInclude
  // testStartForInclude, greater length, start at start
  OtherSection.set( Start, End+1 );
  Diff = Section.width() - OtherSection.width();
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, start before start
  OtherSection.moveBy( -1 );
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, end behind end
  OtherSection.moveBy( 2 );
  QCOMPARE( Section.startForInclude(OtherSection), OtherSection.start()-Diff );
*/
}


void KSectionTest::testIsJoinable()
{
  // testStartForInclude, same length, start at start
  const KSection Section( Start, End );
  KSection OtherSection( Section );
  QVERIFY( Section.isJoinable(OtherSection) );

  // adaptToChange, insert, before
  OtherSection.set( End+1, End+Width );
  QVERIFY( Section.isJoinable(OtherSection) );

  // Overlapping, right shifted
  OtherSection.set( Start+1, End+1 );
  QVERIFY( Section.isJoinable(OtherSection) );

  // Overlapping, left shifted
  OtherSection.set( Start-1, End-1 );
  QVERIFY( Section.isJoinable(OtherSection) );

  // Overlapping, 1 includes 2
  OtherSection.set( Start+1, End-1 );
  QVERIFY( Section.isJoinable(OtherSection) );

  // Overlapping, 2 includes 1
  OtherSection.set( Start-1, End+1 );
  QVERIFY( Section.isJoinable(OtherSection) );

  // Overlapping, identic
  OtherSection.set( Section );
  QVERIFY( Section.isJoinable(OtherSection) );

  // Coupled, first, then second
  OtherSection.set( End+1, End+Width );
  QVERIFY( Section.isJoinable(OtherSection) );

  // Coupled, second, then first
  QVERIFY( OtherSection.isJoinable(Section) );

  // NonOverlapping, first, then second
  OtherSection.set( End+2, End+Width+1 );
  QVERIFY( !Section.isJoinable(OtherSection) );

  // NonOverlapping, second, then first
  QVERIFY( !OtherSection.isJoinable(Section) );
}


QTEST_MAIN( KSectionTest )

#include "ksectiontest.moc"
