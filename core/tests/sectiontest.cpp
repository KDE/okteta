/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2006,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "sectiontest.h"

// test object
#include <section.h>
// Qt
#include <QtTest/QtTest>

using namespace KDE;

// local variables
static const int Start = 15;
static const int End = 27;

static const int Width = End - Start + 1;


void SectionTest::testConstructorByEnd()
{
  Section section( Start, End );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   End );
  QCOMPARE( section.width(), Width );
}

void SectionTest::testConstructorByWidth()
{
  Section section = Section::fromWidth( Start, Width );

  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), End );
  QCOMPARE( section.width(), Width );

  section = Section::fromWidth( Width );

  QCOMPARE( section.start(), 0 );
  QCOMPARE( section.end(), Width-1 );
  QCOMPARE( section.width(), Width );
}

void SectionTest::testSimpleConstructor()
{
  Section section;
  QVERIFY( section.isEmpty() );
}

void SectionTest::testCopyConstructor()
{
  const Section OtherSection( Start, End );

  Section section( OtherSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   End );
  QCOMPARE( section.width(), Width );
  QVERIFY( !section.isEmpty() );
}


void SectionTest::testSetGetStart()
{
  Section section;
  section.setStart( Start );
  QCOMPARE( section.start(), Start );
}

void SectionTest::testSetGetEnd()
{
  Section section;
  section.setEnd( End );
  QCOMPARE( section.end(), End );
}

void SectionTest::testSetGetWidth()
{
  Section section;
  // end, width by width
  section.setStart( Start );
  section.setEndByWidth( Width );
  QCOMPARE( section.end(), End );
  QCOMPARE( section.width(), Width );
  // start, width by width
  section.setEnd( End );
  section.setStartByWidth( Width );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.width(), Width );
}

void SectionTest::testIsEmpty()
{
  Section section( Start, End );
  QVERIFY( !section.isEmpty() );
  section.unset();
  QVERIFY( section.isEmpty() );
}

void SectionTest::testCompare()
{
  Section section( Start, End );
  // same
  Section OtherSection( Start, End );
  QVERIFY( section == OtherSection );
  // different start
  OtherSection.set(Start+1, End);
  QVERIFY( !(section == OtherSection) );
  // different end
  OtherSection.set(Start, End+1);
  QVERIFY( !(section == OtherSection) );
}

void SectionTest::testIncludes()
{
  Section section( Start, End );
  // at start
  int i = section.start();
  QVERIFY( section.includes(i) );
  // before start
  --i;
  QVERIFY( !section.includes(i) );
  // at end
  i = section.end();
  QVERIFY( section.includes(i) );
  // behind end
  ++i;
  QVERIFY( !section.includes(i) );
}


void SectionTest::testLocalIndex()
{
    Section section( Start, End );

    int localIndex = section.localIndex( Start );
    QCOMPARE( localIndex, 0 );

    localIndex = section.localIndex( End );
    QCOMPARE( localIndex, Width-1 );
}


void SectionTest::testLocalRange()
{
    Section section( Start, End );

    Section localRange = section.localRange( Section(Start,End) );
    QCOMPARE( localRange.start(), 0 );
    QCOMPARE( localRange.end(), Width-1 );

    localRange = section.localRange( Section(Start+1,End) );
    QCOMPARE( localRange.start(), 1 );
    QCOMPARE( localRange.end(), Width-1 );

    localRange = section.localRange( Section(Start,End-1) );
    QCOMPARE( localRange.start(), 0 );
    QCOMPARE( localRange.end(), Width-2 );

    localRange = section.localRange( Section(Start+1,End-1) );
    QCOMPARE( localRange.start(), 1 );
    QCOMPARE( localRange.end(), Width-2 );
}


void SectionTest::testSubRange()
{
    Section section( Start, End );

    Section subRange = section.subRange( Section::fromWidth(Width) );
    QCOMPARE( subRange.start(), Start );
    QCOMPARE( subRange.end(), End );

    subRange = section.subRange( Section(1,Width-1) );
    QCOMPARE( subRange.start(), Start+1 );
    QCOMPARE( subRange.end(), End );

    subRange = section.subRange( Section(0,Width-2) );
    QCOMPARE( subRange.start(), Start );
    QCOMPARE( subRange.end(), End-1 );

    subRange = section.subRange( Section(1,Width-2) );
    QCOMPARE( subRange.start(), Start+1 );
    QCOMPARE( subRange.end(), End-1 );
}


void SectionTest::testAdaptToReplacement()
{
  // adaptToReplacement, same length, behind
  Section section( Start, End );
  Section OtherSection( section );
  section.adaptToReplacement( End+1, Width, Width );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, same length, before
  section.set( Start, End );
  OtherSection.set( section );
  section.adaptToReplacement( Start-2, 1, 1 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, same length, -1,-1
  section.set( Start-1, End-1 );
  OtherSection.set( Start-1, Start-1 );
  section.adaptToReplacement( Start, Width, Width );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, same length, -1,0
  section.set( Start-1, End );
  OtherSection.set( section );
  section.adaptToReplacement( Start, Width, Width );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, same length, -1,+1
  section.set( Start-1, End+1 );
  OtherSection.set( section );
  section.adaptToReplacement( Start, Width, Width );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, same length, 0,-1
  section.set( Start, End-1 );
  OtherSection.set( section );
  section.adaptToReplacement( Start, Width, Width );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, same length, 0,0
  section.set( Start, End );
  OtherSection.set( section );
  section.adaptToReplacement( Start, Width, Width );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, same length, 0,+1
  section.set( Start, End+1 );
  OtherSection.set( section );
  section.adaptToReplacement( Start, Width, Width );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, same length, +1,-1
  section.set( Start+1, End-1 );
  section.adaptToReplacement( Start, Width, Width );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, same length, +1,0
  section.set( Start+1, End );
  section.adaptToReplacement( Start, Width, Width );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, same length, +1,+1
  section.set( Start+1, End+1 );
  OtherSection.set( End+1, End+1 );
  section.adaptToReplacement( Start, Width, Width );
  QCOMPARE( section, OtherSection );


  // adaptToReplacement, less length, behind
  section.set( Start, End );
  OtherSection.set( section );
  section.adaptToReplacement( End+1, Width, Width-2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, less length, before
  section.set( Start, End );
  OtherSection.set( Start-2, End-2 );
  section.adaptToReplacement( Start-Width, Width, Width-2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, less length, -1,-1
  section.set( Start-1, End-1 );
  OtherSection.set( Start-1, Start-1 );
  section.adaptToReplacement( Start, Width, Width-2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, less length, -1,0
  section.set( Start-1, End );
  OtherSection.set( Start-1, End-2 );
  section.adaptToReplacement( Start, Width, Width-2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, less length, -1,+1
  section.set( Start-1, End+1 );
  OtherSection.set( Start-1, End-1 );
  section.adaptToReplacement( Start, Width, Width-2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, less length, 0,-1
  section.set( Start, End-1 );
  section.adaptToReplacement( Start, Width, Width-2 );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, less length, 0,0
  section.set( Start, End );
  OtherSection.set( Start, End-2 );
  section.adaptToReplacement( Start, Width, Width-2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, less length, 0,+1
  section.set( Start, End+1 );
  OtherSection.set( Start, End-1 );
  section.adaptToReplacement( Start, Width, Width-2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, less length, +1,-1
  section.set( Start+1, End-1 );
  section.adaptToReplacement( Start, Width, Width-2 );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, less length, +1,0
  section.set( Start+1, End );
  section.adaptToReplacement( Start, Width, Width-2 );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, less length, +1,+1
  section.set( Start+1, End+1 );
  OtherSection.set( End-1, End-1 );
  section.adaptToReplacement( Start, Width, Width-2 );
  QCOMPARE( section, OtherSection );


  // adaptToReplacement, greater length, behind
  section.set( Start, End );
  OtherSection.set( section );
  section.adaptToReplacement( End+1, Width, Width+2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, greater length, before
  section.set( Start, End );
  OtherSection.set( Start+2, End+2 );
  section.adaptToReplacement( Start-Width, Width, Width+2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, greater length, -1,-1
  section.set( Start-1, End-1 );
  OtherSection.set( Start-1, Start-1 );
  section.adaptToReplacement( Start, Width, Width+2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, greater length, -1,0
  section.set( Start-1, End );
  OtherSection.set( Start-1, End+2 );
  section.adaptToReplacement( Start, Width, Width+2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, greater length, -1,+1
  section.set( Start-1, End+1 );
  OtherSection.set( Start-1, End+3 );
  section.adaptToReplacement( Start, Width, Width+2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, greater length, 0,-1
  section.set( Start, End-1 );
  section.adaptToReplacement( Start, Width, Width+2 );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, greater length, 0,0
  section.set( Start, End );
  OtherSection.set( Start, End+2 );
  section.adaptToReplacement( Start, Width, Width+2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, greater length, 0,+1
  section.set( Start, End+1 );
  OtherSection.set( Start, End+3 );
  section.adaptToReplacement( Start, Width, Width+2 );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, greater length, +1,-1
  section.set( Start+1, End-1 );
  section.adaptToReplacement( Start, Width, Width+2 );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, greater length, +1,0
  section.set( Start+1, End );
  section.adaptToReplacement( Start, Width, Width+2 );
  QVERIFY( !section.isValid() );

  // adaptToReplacement, greater length, +1,+1
  section.set( Start+1, End+1 );
  OtherSection.set( End+3, End+3 );
  section.adaptToReplacement( Start, Width, Width+2 );
  QCOMPARE( section, OtherSection );


  // adaptToReplacement, insert, behind
  section.set( Start-1, Start-1 );
  OtherSection.set( Start-1, End );
  section.adaptToReplacement( Start, 0, Width );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, insert, middle
  section.set( Start-1, Start );
  OtherSection.set( Start-1, End+1 );
  section.adaptToReplacement( Start, 0, Width );
  QCOMPARE( section, OtherSection );

  // adaptToReplacement, insert, before
  section.set( Start, Start );
  OtherSection.set( End+1, End+1 );
  section.adaptToReplacement( Start, 0, Width );
  QCOMPARE( section, OtherSection );
}


void SectionTest::testSplitAt()
{
  Section section( Start, End );

  // split at start
  Section SplitSection = section.splitAt( Start );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), Start );
  QCOMPARE( SplitSection.end(),   End );

  // split at one after start
  section.set( Start, End );
  SplitSection = section.splitAt( Start+1 );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), Start );
  QCOMPARE( SplitSection.start(), Start+1 );
  QCOMPARE( SplitSection.end(),   End );

  // split at mid
  const int Mid = (Start+End)/2;
  section.set( Start, End );
  SplitSection = section.splitAt( Mid );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), Mid-1 );
  QCOMPARE( SplitSection.start(), Mid );
  QCOMPARE( SplitSection.end(),   End );

  // split at one before end
  section.set( Start, End );
  SplitSection = section.splitAt( End );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), End-1 );
  QCOMPARE( SplitSection.start(), End );
  QCOMPARE( SplitSection.end(),   End );

  // split at start so the split is the full
  section.set( Start, End );
  SplitSection = section.splitAt( End+1 );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), End );
  QVERIFY( !SplitSection.isValid() );
}

void SectionTest::testSplitAtLocal()
{
  Section section( Start, End );

  // split at start
  Section SplitSection = section.splitAtLocal( 0 );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), Start );
  QCOMPARE( SplitSection.end(),   End );

  // split at one after start
  section.set( Start, End );
  SplitSection = section.splitAtLocal( 1 );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), Start );
  QCOMPARE( SplitSection.start(), Start+1 );
  QCOMPARE( SplitSection.end(),   End );

  // split at mid
  const int Mid = Width/2;
  section.set( Start, End );
  SplitSection = section.splitAtLocal( Mid );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), Start+Mid-1 );
  QCOMPARE( SplitSection.start(), Start+Mid );
  QCOMPARE( SplitSection.end(),   End );

  // split at one before end
  section.set( Start, End );
  SplitSection = section.splitAtLocal( Width-1 );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), End-1 );
  QCOMPARE( SplitSection.start(), End );
  QCOMPARE( SplitSection.end(),   End );

  // split at start so the split is the full
  section.set( Start, End );
  SplitSection = section.splitAtLocal( Width );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), End );
  QVERIFY( !SplitSection.isValid() );
}

void SectionTest::testRemove()
{
  const int Mid = (Start+End)/2;
  Section section( Start, End );

  // remove none at start
  Section RemoveSection( Start, Start-1  );
  Section SplitSection = section.remove( RemoveSection );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), Start );
  QCOMPARE( SplitSection.end(),   End );

  // remove one at start
  section.set( Start, End );
  RemoveSection.set( Start, Start );
  SplitSection = section.remove( RemoveSection );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), Start+1 );
  QCOMPARE( SplitSection.end(),   End );

  // remove many at start
  section.set( Start, End );
  RemoveSection.set( Start, Mid );
  SplitSection = section.remove( RemoveSection );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), Mid+1 );
  QCOMPARE( SplitSection.end(),   End );

  // remove all except last
  section.set( Start, End );
  RemoveSection.set( Start, End-1 );
  SplitSection = section.remove( RemoveSection );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), End );
  QCOMPARE( SplitSection.end(),   End );

  // remove at mid
  section.set( Start, End );
  RemoveSection.set( Mid-1, Mid+1 );
  SplitSection = section.remove( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), Mid-2 );
  QCOMPARE( SplitSection.start(), Mid+2 );
  QCOMPARE( SplitSection.end(),   End );

  // remove none at end
  section.set( Start, End );
  RemoveSection.set( End+1, End );
  SplitSection = section.remove( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   End );
  QVERIFY( !SplitSection.isValid() );

  // remove one at end
  section.set( Start, End );
  RemoveSection.set( End, End );
  SplitSection = section.remove( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   End-1 );
  QVERIFY( !SplitSection.isValid() );

  // remove many at end
  section.set( Start, End );
  RemoveSection.set( Mid, End );
  SplitSection = section.remove( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   Mid-1 );
  QVERIFY( !SplitSection.isValid() );

  // remove all except first
  section.set( Start, End );
  RemoveSection.set( Start+1, End );
  SplitSection = section.remove( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   Start );
  QVERIFY( !SplitSection.isValid() );

  // remove all
  section.set( Start, End );
  RemoveSection.set( Start, End );
  SplitSection = section.remove( RemoveSection );
  QVERIFY( !section.isValid() );
  QVERIFY( !SplitSection.isValid() );
}

void SectionTest::testRemoveLocal()
{
  const int Mid = Width/2;
  Section section( Start, End );

  // remove none at start
  Section RemoveSection( 0, 0-1  );
  Section SplitSection = section.removeLocal( RemoveSection );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), Start );
  QCOMPARE( SplitSection.end(),   End );

  // remove one at start
  section.set( Start, End );
  RemoveSection.set( 0, 0 );
  SplitSection = section.removeLocal( RemoveSection );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), Start+1 );
  QCOMPARE( SplitSection.end(),   End );

  // remove many at start
  section.set( Start, End );
  RemoveSection.set( 0, Mid );
  SplitSection = section.removeLocal( RemoveSection );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), Start+Mid+1 );
  QCOMPARE( SplitSection.end(),   End );

  // remove all except last
  section.set( Start, End );
  RemoveSection.set( 0, Width-2 );
  SplitSection = section.removeLocal( RemoveSection );
  QVERIFY( !section.isValid() );
  QCOMPARE( SplitSection.start(), End );
  QCOMPARE( SplitSection.end(),   End );

  // remove at mid
  section.set( Start, End );
  RemoveSection.set( Mid-1, Mid+1 );
  SplitSection = section.removeLocal( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(), Start+Mid-2 );
  QCOMPARE( SplitSection.start(), Start+Mid+2 );
  QCOMPARE( SplitSection.end(),   End );

  // remove none at end
  section.set( Start, End );
  RemoveSection.set( Width, Width-1 );
  SplitSection = section.removeLocal( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   End );
  QVERIFY( !SplitSection.isValid() );

  // remove one at end
  section.set( Start, End );
  RemoveSection.set( Width-1, Width-1 );
  SplitSection = section.removeLocal( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   End-1 );
  QVERIFY( !SplitSection.isValid() );

  // remove many at end
  section.set( Start, End );
  RemoveSection.set( Mid, Width-1 );
  SplitSection = section.removeLocal( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   Start+Mid-1 );
  QVERIFY( !SplitSection.isValid() );

  // remove all except first
  section.set( Start, End );
  RemoveSection.set( 1, Width-1 );
  SplitSection = section.removeLocal( RemoveSection );
  QCOMPARE( section.start(), Start );
  QCOMPARE( section.end(),   Start );
  QVERIFY( !SplitSection.isValid() );

  // remove all
  section.set( Start, End );
  RemoveSection.set( 0, Width-1 );
  SplitSection = section.removeLocal( RemoveSection );
  QVERIFY( !section.isValid() );
  QVERIFY( !SplitSection.isValid() );
}

void SectionTest::testStartForInclude()
{
  // testStartForInclude, same length, start at start
  const Section section( Start, End );
  Section OtherSection( section );
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, same length, start before start
  OtherSection.moveBy( -1 );
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, same length, end behind end
  OtherSection.moveBy( 2 );
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, start at start
  OtherSection.set( Start, End-1 );
  int Diff = section.width() - OtherSection.width();
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, start before start
  OtherSection.moveBy( -1 );
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, end behind end
  OtherSection.moveBy( 2 );
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start()-Diff );

/* // undefined in startForInclude
  // testStartForInclude, greater length, start at start
  OtherSection.set( Start, End+1 );
  Diff = section.width() - OtherSection.width();
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, start before start
  OtherSection.moveBy( -1 );
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start() );

  // testStartForInclude, smaller length, end behind end
  OtherSection.moveBy( 2 );
  QCOMPARE( section.startForInclude(OtherSection), OtherSection.start()-Diff );
*/
}


void SectionTest::testIsJoinable()
{
  // testStartForInclude, same length, start at start
  const Section section( Start, End );
  Section OtherSection( section );
  QVERIFY( section.isJoinable(OtherSection) );

  // adaptToReplacement, insert, before
  OtherSection.set( End+1, End+Width );
  QVERIFY( section.isJoinable(OtherSection) );

  // Overlapping, right shifted
  OtherSection.set( Start+1, End+1 );
  QVERIFY( section.isJoinable(OtherSection) );

  // Overlapping, left shifted
  OtherSection.set( Start-1, End-1 );
  QVERIFY( section.isJoinable(OtherSection) );

  // Overlapping, 1 includes 2
  OtherSection.set( Start+1, End-1 );
  QVERIFY( section.isJoinable(OtherSection) );

  // Overlapping, 2 includes 1
  OtherSection.set( Start-1, End+1 );
  QVERIFY( section.isJoinable(OtherSection) );

  // Overlapping, identic
  OtherSection.set( section );
  QVERIFY( section.isJoinable(OtherSection) );

  // Coupled, first, then second
  OtherSection.set( End+1, End+Width );
  QVERIFY( section.isJoinable(OtherSection) );

  // Coupled, second, then first
  QVERIFY( OtherSection.isJoinable(section) );

  // NonOverlapping, first, then second
  OtherSection.set( End+2, End+Width+1 );
  QVERIFY( !section.isJoinable(OtherSection) );

  // NonOverlapping, second, then first
  QVERIFY( !OtherSection.isJoinable(section) );
}


QTEST_MAIN( SectionTest )
