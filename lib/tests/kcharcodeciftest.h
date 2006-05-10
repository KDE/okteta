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


#ifndef KCHARCODECIFTEST_H
#define KCHARCODECIFTEST_H


// qt specific
#include <QObject>

namespace KHE {
class KCharCodec;
}


class KCharCodecIfTest : public QObject
{
  Q_OBJECT

  protected:
    /** pointer to the buffer to test */
    KHE::KCharCodec *CharCodec;

  protected:
    KCharCodecIfTest();

  private Q_SLOTS: // test functions
    void testEncodeDecode();
};


inline KCharCodecIfTest::KCharCodecIfTest() : CharCodec( 0 ) {}

#endif
