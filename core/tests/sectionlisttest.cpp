/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#include "sectionlisttest.h"

// test object
#include <sectionlist.h>
// Qt
#include <QtTest/QtTest>

using namespace KDE;

// local variables
static const int Start1 = 15;
static const int End1 = 27;
static const int Width = End1 - Start1 + 1;
static const int Distance = 10;
static const int Start2 = End1 + Distance;
static const int End2 = Start2+Width-1;

void SectionListTest::testSimpleConstructor()
{
  SectionList SectionList;
  QVERIFY( SectionList.isEmpty() );
}


void SectionListTest::testAddSingleSection()
{
  SectionList SectionList;

  Section Section( Start1, End1 );
  SectionList.addSection( Section );
  QCOMPARE( SectionList.size(), 1 );
  QCOMPARE( SectionList.first(), Section );
}


void SectionListTest::testAddJoinableSections()
{
  SectionList SectionList;

  Section Section1( Start1, End1 );
  Section Section2( Start2, End2 );
  // in front
  Section OtherSection( 0, Start1-1 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 2 );
  QCOMPARE( SectionList.first(), Section(OtherSection.start(),Section1.end()) );
  QCOMPARE( SectionList.last(), Section2 );

  // in the middle, with first
  SectionList.clear();
  OtherSection.set( End1+1, Start2-2 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 2 );
  QCOMPARE( SectionList.first(), Section(Section1.start(),OtherSection.end()) );
  QCOMPARE( SectionList.last(), Section2 );

  // in the middle, with both
  SectionList.clear();
  OtherSection.set( End1+1, Start2-1 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 1 );
  QCOMPARE( SectionList.first(), Section(Section1.start(),Section2.end()) );

  // in the middle, with last
  SectionList.clear();
  OtherSection.set( End1+2, Start2-1 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 2 );
  QCOMPARE( SectionList.first(), Section1 );
  QCOMPARE( SectionList.last(), Section(OtherSection.start(),Section2.end()) );

  // behind
  SectionList.clear();
  OtherSection.setByWidth( End2+1, Width );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 2 );
  QCOMPARE( SectionList.first(), Section1 );
  QCOMPARE( SectionList.last(), Section(Section2.start(),OtherSection.end()) );
}


void SectionListTest::testAddNonJoinableSections()
{
  SectionList SectionList;

  Section Section1( Start1, End1 );
  Section Section2( Start2, End2 );
  // in front
  Section OtherSection( 0, Start1-2 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 3 );
  QCOMPARE( SectionList.first(), OtherSection );
  QCOMPARE( SectionList.last(), Section2 );

  // in the middle
  SectionList.clear();
  OtherSection.set( End1+2, Start2-2 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 3 );
  QCOMPARE( SectionList.first(), Section1 );
  QCOMPARE( SectionList.last(), Section2 );

  // behind
  SectionList.clear();
  OtherSection.setByWidth( End2+2, Width );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 3 );
  QCOMPARE( SectionList.first(), Section1 );
  QCOMPARE( SectionList.last(), OtherSection );
}

QTEST_MAIN( SectionListTest )
