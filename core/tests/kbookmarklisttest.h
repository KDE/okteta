/***************************************************************************
                          kbookmarklisttest.h  -  description
                             -------------------
    begin                : Mon Dec 3 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KBOOKMARKTEST_H
#define KBOOKMARKTEST_H

// Qt
#include <QtCore/QObject>

class KBookmarkListTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testAddRemoveBookmark();
    void testContains();
    void testPreviousFrom();
    void testNextFrom();
    void testAdjustToMove();
    void testAdjustToReplaced();
};

#endif
