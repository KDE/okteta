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
// app specific
#include "testsection.h"

// namespaces
using namespace KDE::Test;
using namespace KHE;

// local variables
static const int Start = 3;
static const int End = 15;


SectionTest::SectionTest() 
 :  SlotTest( "SectionTest" ) 
{
}

void SectionTest::setUp() 
{
  Section.set( Start, End );
}

void SectionTest::tearDown() 
{
}



//---------------------------------------------------------------------------- Tests -----

void SectionTest::testSectionCopyCompare() 
{
  KT_ASSERT_EQUALS( "start()", Section.start(), Start );
  KT_ASSERT_EQUALS( "end()", Section.end(), End );

  KSection S1 = Section;
  KT_ASSERT( "Test ==", S1 == Section );    
  
  KSection S2;
  KT_ASSERT( "isNull()", S2.isNull() );
  S2 = KSection( S1.start(), S1.end() + S1.width() );
  KT_ASSERT( "!isNull()", !S2.isNull() );
  KT_ASSERT_EQUALS( "width()" , S2.width(), S1.width() * 2 );
  
  int i = S1.start();
  KT_ASSERT( "includes, min", S1.includes(i) );
  --i;
  KT_ASSERT( "!includes, min", !S1.includes(i) );
  i = S1.end();
  KT_ASSERT("includes, max", S1.includes(i) );
  ++i;
  KT_ASSERT("!includes, max", !S1.includes(i) );
  
  KT_ASSERT("hi, this is wrong", false ); 
}


int main( int, char** )
{
  SectionTest t;
  return TestRunner( &t ).run();
}

#include "testsection.moc"
