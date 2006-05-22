/***************************************************************************
                          ktextcharcodeckcharcodeciftest.cpp  -  description
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
#include <codecs/ktextcharcodec.h>
// test specific
#include "ktextcharcodeckcharcodeciftest.h"


using namespace KHE;



KHE::KCharCodec *KTextCharCodecKCharCodecIfTest::createCodec()
{
  return KTextCharCodec::createCodec("ISO8859-1"); // TODO: test for some more, e.g. KOI8-R
}


void KTextCharCodecKCharCodecIfTest::deleteCodec( KHE::KCharCodec *Codec )
{
  delete Codec;
}


QTEST_MAIN( KTextCharCodecKCharCodecIfTest )
