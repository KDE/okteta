/***************************************************************************
                          testebcdic1047charcodeccharcodecif.cpp  -  description
                            -------------------
    begin                : Sa Jan 15 2005
    copyright            : (C) 2005 by Friedrich W. H. Kossebau
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
#include <kde/test/testrunner.h>
// test specific
#include <codecs/kebcdic1047charcodec.h>
#include "testebcdic1047charcodeccharcodecif.h"


// namespaces
using namespace KDE::Test;
using namespace KHE;



// all test imply that the buffer ca
// readonly 
KEBCDIC1047CharCodecCharCodecIfTest::KEBCDIC1047CharCodecCharCodecIfTest()
  : KCharCodecIfTest( "KEBCDIC1047CharCodecCharCodecIfTest" )
{
  CharCodec = KEBCDIC1047CharCodec::create();
}


KEBCDIC1047CharCodecCharCodecIfTest::~KEBCDIC1047CharCodecCharCodecIfTest()
{
  delete CharCodec;
}

void KEBCDIC1047CharCodecCharCodecIfTest::setup()
{
}

void KEBCDIC1047CharCodecCharCodecIfTest::tearDown()
{
}

int main( int, char** )
{
  KEBCDIC1047CharCodecCharCodecIfTest CharCodecIfTest;

  TestRunner Runner;
  Runner.add( &CharCodecIfTest );

  return Runner.run();
}

#include "testebcdic1047charcodeccharcodecif.moc"
