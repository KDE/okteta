/***************************************************************************
                          testsection.cpp  -  description
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


// kde specific
#include <kde/test/slottest.h>
#include <kde/test/testrunner.h>
// test specific
#include <ksection.h>
#include "testsection.h"

// namespaces
using namespace KDE::Test;
using namespace KHE;

// local variables
static const int Start = 3;
static const int End = 15;

static const int Width = End - Start + 1;


SectionTest::SectionTest()
  :  SlotTest( "SectionTest" ) // TODO: replace this by using the object->className()
{
}



//---------------------------------------------------------------------------- Tests -----

void SectionTest::testConstructorByEnd()
{
  KSection Section( Start, End );
  KT_ASSERT_EQUALS( "Start by constructor", Section.start(), Start );
  KT_ASSERT_EQUALS( "End by constructor", Section.end(), End );
  KT_ASSERT_EQUALS( "Width by constructor", Section.width(), Width );
}

void SectionTest::testConstructorByWidth()
{
  KSection Section( Start, Width, true );
  KT_ASSERT_EQUALS( "Start by constructor", Section.start(), Start );
  KT_ASSERT_EQUALS( "End by constructor", Section.end(), End );
  KT_ASSERT_EQUALS( "Width by constructor", Section.width(), Width );
}

void SectionTest::testSimpleConstructor()
{
  KSection Section;
  KT_ASSERT( "Simpleconstructor", Section.isEmpty() );
}

void SectionTest::testSetGetStart()
{
  KSection Section;
  Section.setStart( Start );
  KT_ASSERT_EQUALS( "Start by set", Section.start(), Start );
}

void SectionTest::testSetGetEnd()
{
  KSection Section;
  Section.setEnd( End );
  KT_ASSERT_EQUALS( "End by set", Section.end(), End );
}


void SectionTest::testSetGetWidth()
{
  KSection Section;
  Section.setStart( Start );
  Section.setEndByWidth( Width );
  KT_ASSERT_EQUALS( "End by Width", Section.end(), End );
  KT_ASSERT_EQUALS( "Width by Width", Section.width(), Width );
  Section.setEnd( End );
  Section.setStartByWidth( Width );
  KT_ASSERT_EQUALS( "Start by Width", Section.start(), Start );
  KT_ASSERT_EQUALS( "Width by Width", Section.width(), Width );
}

void SectionTest::testIsEmpty()
{
  KSection Section( Start, End );
  KT_ASSERT( "!isEmpty()", !Section.isEmpty() );
  Section.unset();
  KT_ASSERT( "isEmpty()", Section.isEmpty() );
}

void SectionTest::testCompare()
{
  KSection Section( Start, End );
  KSection OtherSection( Start, End );
  KT_ASSERT( "operator==", Section == OtherSection );
  OtherSection.set(Start+1, End);
  KT_ASSERT( "!operator==", ! (Section == OtherSection) );
  OtherSection.set(Start, End+1);
  KT_ASSERT( "!operator==", ! (Section == OtherSection) );
}

//
void SectionTest::testIncludes()
{
  KSection Section( Start, End );
  int i = Section.start();
  KT_ASSERT( "includes, min", Section.includes(i) );
  --i;
  KT_ASSERT( "!includes, min", !Section.includes(i) );
  i = Section.end();
  KT_ASSERT( "includes, max", Section.includes(i) );
  ++i;
  KT_ASSERT( "!includes, max", !Section.includes(i) );
}

int main( int, char** )
{
  SectionTest t;
  return TestRunner( &t ).run();
}


#include "testsection.moc"
