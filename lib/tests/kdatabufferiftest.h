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
#include <QObject>

namespace KHE {
class KDataBuffer;
}


class KDataBufferIfTest : public QObject
{
  Q_OBJECT

  protected:
    KDataBufferIfTest();

  protected: // our API
    virtual KHE::KDataBuffer *createBuffer() = 0;
    virtual void deleteBuffer( KHE::KDataBuffer *Buffer ) = 0;

  private Q_SLOTS: // test functions
    void init();
    void cleanup();

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

  private: // used in all tests
    /** pointer to the buffer to test */
    KHE::KDataBuffer *DataBuffer;
};

inline KDataBufferIfTest::KDataBufferIfTest()  : DataBuffer( 0 ) {}

#endif
