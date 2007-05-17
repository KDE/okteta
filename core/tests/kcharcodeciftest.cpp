/***************************************************************************
                          kcharcodeciftest.h  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#include "kcharcodeciftest.h"

// test object
#include <kcharcodec.h>
// lib
#include <khechar.h>
// Qt
#include <QtTest/QtTest>

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
