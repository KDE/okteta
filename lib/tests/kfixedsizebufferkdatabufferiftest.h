/***************************************************************************
                          kfixedsizebufferkdatabufferiftest.h  -  description
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


// test specific
#include "kdatabufferiftest.h"


#ifndef KFIXEDSIZEBUFFERKDATABUFFERIFTEST_H
#define KFIXEDSIZEBUFFERKDATABUFFERIFTEST_H


class KFixedSizeBufferDataBufferIfTest : public KDataBufferIfTest
{
  Q_OBJECT

  public Q_SLOTS: // 
    void init();
    void cleanup();
};

#endif
