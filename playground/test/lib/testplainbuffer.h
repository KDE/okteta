/***************************************************************************
                          testplainbuffer.h  -  description
                            -------------------
    begin                : Don Apr 29 2004
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


#ifndef TEST_KPLAINBUFFER_H
#define TEST_KPLAINBUFFER_H

/** base class for all Data buffers that are used to display
  * 
  * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
  * @testfor KDataBuffer
  */
class KPlainBufferTest : public KDE::Test::SlotTest
{
  Q_OBJECT

  public:
    KPlainBufferTest();
    virtual ~KPlainBufferTest();


  public slots:
    void testNothing();
};

#endif
