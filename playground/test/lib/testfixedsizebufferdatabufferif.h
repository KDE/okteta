/***************************************************************************
                          testfixedsizebufferdatabufferif.h  -  description
                            -------------------
    begin                : Mon Mai 3 2004
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


// app specific
#include "testdatabufferif.h"


#ifndef TEST_KFIXEDSIZEBUFFERDATABUFFERIF_H
#define TEST_KFIXEDSIZEBUFFERDATABUFFERIF_H

/** base class for all Data buffers that are used to display
  * 
  * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
  * @testfor KDataBuffer
  */
class KFixedSizeBufferDataBufferIfTest : public KDataBufferIfTest 
{
  Q_OBJECT

  public:
    KFixedSizeBufferDataBufferIfTest();
    virtual ~KFixedSizeBufferDataBufferIfTest();

  public: //
    virtual void setup();
    virtual void tearDown();
};

#endif
