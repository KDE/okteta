/***************************************************************************
                          testfixedsizebuffer.h  -  description
                            -------------------
    begin                : Son Mai 2 2004
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


// kde specific
#include <kde/test/slottest.h>


#ifndef TEST_KFIXEDSIZEBUFFER_H
#define TEST_KFIXEDSIZEBUFFER_H

/** base class for all Data buffers that are used to display
  * 
  * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
  * @testfor KDataBuffer
  */
class KFixedSizeBufferTest : public KDE::Test::SlotTest
{
  Q_OBJECT

  public:
    KFixedSizeBufferTest();
    virtual ~KFixedSizeBufferTest();


  public slots:
    void testCompare();
    //void testCopy();

};

#endif
