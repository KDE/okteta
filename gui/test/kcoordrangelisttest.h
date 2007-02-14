/***************************************************************************
                          kcoordrangelisttest.h  -  description
                             -------------------
    begin                : Thu Mai 25 2006
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


#ifndef KSECTIONLISTTEST_H
#define KSECTIONLISTTEST_H

// qt specific
#include <QObject>

class KCoordRangeListTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testAddSingleSection();
    void testAddJoinableSections();
    void testAddNonJoinableSections();
};

#endif
