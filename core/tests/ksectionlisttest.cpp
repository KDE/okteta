/***************************************************************************
                          ksectionlisttest.cpp -  description
                             -------------------
    begin                : Son Mai 14 2006
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
// lib specific
#include <ksectionlist.h>
// test specific
#include "ksectionlisttest.h"

using namespace KHE;

// local variables
static const int Start1 = 15;
static const int End1 = 27;
static const int Width = End1 - Start1 + 1;
static const int Distance = 10;
static const int Start2 = End1 + Distance;
static const int End2 = Start2+Width-1;

void KSectionListTest::testSimpleConstructor()
{
  KSectionList SectionList;
  QVERIFY( SectionList.isEmpty() );
}


void KSectionListTest::testAddSingleSection()
{
  KSectionList SectionList;

  KSection Section( Start1, End1 );
  SectionList.addSection( Section );
  QCOMPARE( SectionList.size(), 1 );
  QCOMPARE( SectionList.first(), Section );
}


void KSectionListTest::testAddJoinableSections()
{
  KSectionList SectionList;

  KSection Section1( Start1, End1 );
  KSection Section2( Start2, End2 );
  // in front
  KSection OtherSection( 0, Start1-1 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 2 );
  QCOMPARE( SectionList.first(), KSection(OtherSection.start(),Section1.end()) );
  QCOMPARE( SectionList.last(), Section2 );

  // in the middle, with first
  SectionList.clear();
  OtherSection.set( End1+1, Start2-2 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 2 );
  QCOMPARE( SectionList.first(), KSection(Section1.start(),OtherSection.end()) );
  QCOMPARE( SectionList.last(), Section2 );

  // in the middle, with both
  SectionList.clear();
  OtherSection.set( End1+1, Start2-1 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 1 );
  QCOMPARE( SectionList.first(), KSection(Section1.start(),Section2.end()) );

  // in the middle, with last
  SectionList.clear();
  OtherSection.set( End1+2, Start2-1 );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 2 );
  QCOMPARE( SectionList.first(), Section1 );
  QCOMPARE( SectionList.last(), KSection(OtherSection.start(),Section2.end()) );

  // behind
  SectionList.clear();
  OtherSection.setByWidth( End2+1, Width );
  SectionList.addSection( Section1 );
  SectionList.addSection( Section2 );
  SectionList.addSection( OtherSection );

  QCOMPARE( SectionList.size(), 2 );
  QCOMPARE( SectionList.first(), Section1 );
  QCOMPARE( SectionList.last(), KSection(Section2.start(),OtherSection.end()) );
}


void KSectionListTest::testAddNonJoinableSections()
{
  KSectionList SectionList;

  KSection Section1( Start1, End1 );
  KSection Section2( Start2, End2 );
  // in front
  KSection OtherSection( 0, Start1-2 );
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

QTEST_MAIN( KSectionListTest )

#include "ksectionlisttest.moc"
