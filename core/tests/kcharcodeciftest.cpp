/***************************************************************************
                          kcharcodeciftest.h  -  description
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
// kde specific
#include <khechar.h>
#include <kcharcodec.h>
// test specific
#include "kcharcodeciftest.h"

using namespace KHECore;

static const char QTextCodecWhiteSpace = 63;


void KCharCodecIfTest::init()
{
  CharCodec = createCodec();
}

void KCharCodecIfTest::cleanup()
{
  deleteCodec( CharCodec );
}


void KCharCodecIfTest::testEncodeDecode()
{
  unsigned char c = 0;
  do
  {
    ++c;
    KChar UTF8 = CharCodec->decode( c );
    char r = 0;
    bool Success = CharCodec->encode( &r, UTF8 );
    QCOMPARE( (unsigned char)r, c );
    QVERIFY( Success );
  }
  while( c < 255 );
}

#include "kcharcodeciftest.moc"
