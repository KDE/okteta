/***************************************************************************
                          testplainbuffer.cpp  -  description
                            -------------------
    begin                : Don Apr 29 2004
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
#include <kde/test/testrunner.h>
// test specific
#include <kplainbuffer.h>
#include "testplainbufferdatabufferif.h"
#include "testplainbuffer.h"

using namespace KDE::Test;
using namespace KHE;


// all test imply that the buffer ca
// readonly 
KPlainBufferTest::KPlainBufferTest()
 : SlotTest( "KPlainBufferTest" )
{
}

KPlainBufferTest::~KPlainBufferTest()
{
}

void KPlainBufferTest::testNothing()
{
}




int main( int, char** )
{
  KPlainBufferTest PlainBufferTest;
  KPlainBufferDataBufferIfTest DataBufferIfTest;

  TestRunner Runner;
  Runner.add( &PlainBufferTest );

  Runner.add( &DataBufferIfTest );

  return Runner.run();
}

#include "testplainbuffer.moc"
