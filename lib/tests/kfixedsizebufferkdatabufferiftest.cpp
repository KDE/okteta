/***************************************************************************
                          kfixedsizebufferkdatabufferiftest.cpp  -  description
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
// lib specific
#include <kfixedsizebuffer.h>
// test specific
#include "kfixedsizebufferkdatabufferiftest.h"


using namespace KHE;

static const int FixedSizeBufferSize = 60;



void KFixedSizeBufferDataBufferIfTest::init()
{
  DataBuffer = new KFixedSizeBuffer( FixedSizeBufferSize );
}

void KFixedSizeBufferDataBufferIfTest::cleanup()
{
  delete DataBuffer;
}

QTEST_MAIN( KFixedSizeBufferDataBufferIfTest )

#include "kfixedsizebufferkdatabufferiftest.moc"
