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


#ifndef TEST_KCHARCODECIF_H
#define TEST_KCHARCODECIF_H


// kde specific
#include <kde/test/slottest.h>
#include <kcharcodec.h>


/**
 *
 * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
 * @testfor KCharCodec
 */
class KCharCodecIfTest : public KDE::Test::SlotTest
{
  Q_OBJECT

  protected: // fixtures used in all tests
    /** pointer to the buffer to test */
    KHE::KCharCodec *CharCodec;

  protected:
    KCharCodecIfTest( const char* Name );
  public:
    virtual ~KCharCodecIfTest();

  public slots:
    void test();
};


inline KCharCodecIfTest::KCharCodecIfTest( const char* Name )  : SlotTest( Name ), CharCodec( 0 ) {}
inline KCharCodecIfTest::~KCharCodecIfTest() {}

#endif
