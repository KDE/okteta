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
// test specific
#include <kbuffercoord.h>

class CoordTest : public KDE::Test::SlotTest 
{
  Q_OBJECT
  
  protected:
    KHE::KBufferCoord Coord;
    
  public:
    CoordTest();
  
  public: // SlotTest API
    void setUp();
    void tearDown();
  
  public slots: // test functions
    void testCoordCopyEquals();
    void testCoordSet();
    void testCoordGoto();
    void testCoordCompare();
    void testCoordByIndex();
};

#endif
