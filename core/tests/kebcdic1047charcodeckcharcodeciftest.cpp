/***************************************************************************
                          kebcdic1047charcodeckcharcodeciftest.cpp  -  description
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


#include "kebcdic1047charcodeckcharcodeciftest.h"

// test object
#include <codecs/kebcdic1047charcodec.h>
// Qt
#include <QtTest/QtTest>


using namespace KHECore;


KHECore::KCharCodec *KEBCDIC1047CharCodecKCharCodecIfTest::createCodec()
{
  return KEBCDIC1047CharCodec::create();
}


void KEBCDIC1047CharCodecKCharCodecIfTest::deleteCodec( KHECore::KCharCodec *Codec )
{
  delete Codec;
}


QTEST_MAIN( KEBCDIC1047CharCodecKCharCodecIfTest )
