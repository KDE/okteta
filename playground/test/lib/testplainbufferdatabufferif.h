/***************************************************************************
                          testplainbufferdatabufferif.h  -  description
                            -------------------
    begin                : Sam Mai 1 2004
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


#ifndef TEST_KPLAINBUFFERDATABUFFERIF_H
#define TEST_KPLAINBUFFERDATABUFFERIF_H

/** 
  * 
  * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
  * @testfor KDataBuffer
  */
class KPlainBufferDataBufferIfTest : public KDataBufferIfTest 
{
  Q_OBJECT

  public:
    KPlainBufferDataBufferIfTest();
    virtual ~KPlainBufferDataBufferIfTest();

  public: //
    virtual void setup();
    virtual void tearDown();
};

#endif
