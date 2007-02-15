/***************************************************************************
                          kselectiontest.h  -  description
                             -------------------
    begin                : Mon Mai 22 2006
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


#ifndef KSELECTIONTEST_H
#define KSELECTIONTEST_H

// qt specific
#include <QObject>

class KSelectionTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testPlainConstructor();
    void testAnchorConstructor();
    void testSetStart();
    void testSetStartEnd();
    void testCancel();
    void testSetForward();
    void testReverse();
};

#endif
