/***************************************************************************
                          testsection.h  -  description
                            -------------------
    begin                : Sam Apr 17 2004
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


#ifndef TEST_SECTION_H
#define TEST_SECTION_H

// kde specific
#include <kde/test/slottest.h>
// test specific
#include <ksection.h>

class SectionTest : public KDE::Test::SlotTest 
{
  Q_OBJECT
  
  protected:
    KHE::KSection Section;
    
  public:
    SectionTest();
  
  public: // SlotTest API
    void setUp();
    void tearDown();
  
  public slots: // test functions
    void testSectionCopyCompare();
};

#endif
