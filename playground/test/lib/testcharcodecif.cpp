/***************************************************************************
                          testcharcodecif.h  -  description
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


#include <kdebug.h>
// kde specific
#include <kde/test/slottest.h>
// app specific
#include "testcharcodecif.h"

using namespace KDE::Test;
using namespace KHE;

static const char QTextCodecWhiteSpace = 63;


void KCharCodecIfTest::test()
{
  unsigned char c = 0;
  do
  {
    ++c;
    KHEChar UTF8 = CharCodec->decode( c );
    char r = 0;
    bool Success = CharCodec->encode( &r, UTF8 );
    KT_ASSERT_EQUALS( "encode(decode)", (unsigned char)r, c );
    KT_ASSERT_EQUALS( "encode(decode)", Success, true );
  }
  while( c < 255 );
}

#include "testcharcodecif.moc"
