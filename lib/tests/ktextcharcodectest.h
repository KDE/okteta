/***************************************************************************
                          ktextcharcodectest.h  -  description
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



#ifndef KTEXTCHARCODECTEST_H
#define KTEXTCHARCODECTEST_H

class KInstance;

// qt specific
#include <qobject.h>

class KTextCharCodecTest : public QObject
{
  Q_OBJECT

    KInstance *Instance;

  private Q_SLOTS: // test functions
    void initTestCase();
    void cleanupTestCase();
    void testCreateLocalCodec();
    void testCreateCodec();
};

#endif
