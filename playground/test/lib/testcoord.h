/***************************************************************************
                          testcoord.h  -  description
                            -------------------
    begin                : Mit Apr 21 2004
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


#ifndef TEST_COORD_H
#define TEST_COORD_H

// kde specific
#include <kde/test/slottest.h>

class CoordTest : public KDE::Test::SlotTest 
{
  Q_OBJECT

  public:
    CoordTest();

  public slots: // test functions
    void testConstructor();
    void testSetConstructor();
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
