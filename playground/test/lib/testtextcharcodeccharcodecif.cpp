/***************************************************************************
                          testtextcharcodeccharcodecif.cpp  -  description
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


// lib specific
#include <codecs/ktextcharcodec.h>
// app specific
#include "testtextcharcodeccharcodecif.h"


using namespace KHE;



// all test imply that the buffer ca
// readonly 
KTextCharCodecCharCodecIfTest::KTextCharCodecCharCodecIfTest(const QString &Name)
  : KCharCodecIfTest( "KTextCharCodecCharCodecIfTest" )
{
  CharCodec = KTextCharCodec::createCodec(Name);
}


KTextCharCodecCharCodecIfTest::~KTextCharCodecCharCodecIfTest()
{
  delete CharCodec;
}

void KTextCharCodecCharCodecIfTest::setup()
{
}

void KTextCharCodecCharCodecIfTest::tearDown()
{
}


#include "testtextcharcodeccharcodecif.moc"
