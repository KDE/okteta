/***************************************************************************
                          ksectiontest.h  -  description
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


#ifndef KSECTIONTEST_H
#define KSECTIONTEST_H

// qt specific
#include <QObject>

class KSectionTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testConstructorByEnd();
    void testConstructorByWidth();
    void testSimpleConstructor();
    void testSetGetStart();
    void testSetGetEnd();
    void testSetGetWidth();
    void testIsEmpty();
    void testCompare();
    void testIncludes();
    void testAdaptToChange();
    void testStartForInclude();
    void testIsJoinable();
};

#endif
