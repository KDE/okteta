/***************************************************************************
                          kcoordrangetest.h  -  description
                             -------------------
    begin                : Thu May 25 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KCOORDRANGETEST_H
#define KCOORDRANGETEST_H

// qt specific
#include <QObject>

class KCoordRangeTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testConstructorByCoords();
    void testConstructorByPosLines();
    void testSimpleConstructor();
    void testSetGetStart();
    void testSetGetEnd();
    void testIsEmpty();
    void testCompare();
    void testIncludes();
    void testIncludesLine();
    void testWidth();
};

#endif
