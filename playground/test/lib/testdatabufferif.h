/***************************************************************************
                          testdatabufferif.h  -  description
                            -------------------
    begin                : Sam Apr 17 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#ifndef TEST_KDATABUFFERIF_H
#define TEST_KDATABUFFERIF_H


// kde specific
#include <kde/test/slottest.h>
#include <kdatabuffer.h>


/** base class for all Data buffers that are used to display
  * 
  * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
  * @testfor KDataBuffer
  */
class KDataBufferIfTest : public KDE::Test::SlotTest
{
  Q_OBJECT

  protected: // fixtures usd in all tests
    /** pointer to the buffer to test */
    KHE::KDataBuffer *DataBuffer;

    /** */
    KHE::KSection TestSection;

  protected:
    KDataBufferIfTest( const char* Name );
  public:
    virtual ~KDataBufferIfTest();

  public slots:
    void testCopyTo();
    void testFill();
    void testSetGet();

    void testRemove();
    //void testInsert();
    //void testReplace();
};


inline KDataBufferIfTest::KDataBufferIfTest( const char* Name )  : SlotTest( Name ), DataBuffer( 0 ) {}
inline KDataBufferIfTest::~KDataBufferIfTest() {}

#endif
