/***************************************************************************
                          kcharcodectest.h  -  description
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



#ifndef KCHARCODECTEST_H
#define KCHARCODECTEST_H

class KInstance;

// qt specific
#include <QObject>

class KCharCodecTest : public QObject
{
  Q_OBJECT

    KInstance *Instance;

  private Q_SLOTS: // test functions
    void initTestCase();
    void cleanupTestCase();
    void testCodecNames();
};

#endif
