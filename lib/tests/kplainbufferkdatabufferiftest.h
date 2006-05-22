/***************************************************************************
                          kplainbufferkdatabufferiftest.h  -  description
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



#ifndef KPLAINBUFFERKDATABUFFERIFTEST_H
#define KPLAINBUFFERKDATABUFFERIFTEST_H


// test specific
#include "kdatabufferiftest.h"


class KPlainBufferKDataBufferIfTest : public KDataBufferIfTest
{
  protected: // KDataBufferIfTest API
    KHE::KDataBuffer *createBuffer();
    void deleteBuffer( KHE::KDataBuffer *Buffer );
};

#endif
