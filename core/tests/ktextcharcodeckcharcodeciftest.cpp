/***************************************************************************
                          ktextcharcodeckcharcodeciftest.cpp  -  description
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


// qt specific
#include <QtTest>
// lib specific
#include <codecs/ktextcharcodec.h>
// test specific
#include "ktextcharcodeckcharcodeciftest.h"



KHECore::KCharCodec *KTextCharCodecKCharCodecIfTest::createCodec()
{
  return KHECore::KTextCharCodec::createCodec("ISO8859-1"); // TODO: test for some more, e.g. KOI8-R
}


void KTextCharCodecKCharCodecIfTest::deleteCodec( KHECore::KCharCodec *Codec )
{
  delete Codec;
}


QTEST_MAIN( KTextCharCodecKCharCodecIfTest )
