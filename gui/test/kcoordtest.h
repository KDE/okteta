/***************************************************************************
                          kcoordtest.h  -  description
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


#ifndef KBUFFERCOORDTEST_H
#define KBUFFERCOORDTEST_H

// qt specific
#include <QObject>

class KCoordTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testConstructor();
    void testSetConstructor();
    void testFromIndexConstructor();
    void testCopyConstructor();
    void testEquals();
    void testAssign();
    void testSetPosLine();
    void testSet();
    void testIsAtStart();
    void testGotoStart();
    void testGo();
    void testGoN();
    void testGotoControlled();
    void testGoLineStart();
    void testGoLineEnd();
    void testCompareOperator();
    void testIsBelowIsAbove();
    void testIsLaterPriorInLineThan();
    void testIsBeforeLineStart();
    void testIsBeforeLineEnd();
    void testIndexNWidth();
};

#endif
