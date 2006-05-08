/***************************************************************************
                          kdatabufferiftest.h  -  description
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


#ifndef KDATABUFFERIFTEST_H
#define KDATABUFFERIFTEST_H


// qt specific
#include <qobject.h>

namespace KHE {
class KDataBuffer;
}


class KDataBufferIfTest : public QObject
{
  Q_OBJECT

  protected: // used in all tests
    /** pointer to the buffer to test */
    KHE::KDataBuffer *DataBuffer;

  protected:
    KDataBufferIfTest();

  private Q_SLOTS: // test functions
    void testModified();
    void testCopyTo();
    void testFill();
    void testSetGet();

    void testRemove();
    void testInsert();
    //void testReplace();
    void testMove();
    void testReplaceEqual();
    void testReplaceLess();
    void testReplaceMore();
};

inline KDataBufferIfTest::KDataBufferIfTest()  : DataBuffer( 0 ) {}

#endif
