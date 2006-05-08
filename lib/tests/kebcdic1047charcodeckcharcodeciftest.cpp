/***************************************************************************
                          kebcdic1047charcodeckcharcodeciftest.cpp  -  description
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
#include <codecs/kebcdic1047charcodec.h>
// test specific
#include "kebcdic1047charcodeckcharcodeciftest.h"


using namespace KHE;



void KEBCDIC1047CharCodecKCharCodecIfTest::init()
{
  CharCodec = KEBCDIC1047CharCodec::create();
}


void KEBCDIC1047CharCodecKCharCodecIfTest::cleanup()
{
  delete CharCodec;
}

QTEST_MAIN( KEBCDIC1047CharCodecKCharCodecIfTest )

#include "kebcdic1047charcodeckcharcodeciftest.moc"
