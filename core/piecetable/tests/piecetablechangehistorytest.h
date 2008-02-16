/***************************************************************************
                          piecetablechangehistorytest.h  -  description
                             -------------------
    begin                : Fri Jan 18 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef PIECETABLECHANGEHISTORYTEST_H
#define PIECETABLECHANGEHISTORYTEST_H

// Qt
#include <QtCore/QObject>

namespace KPieceTable
{

class PieceTableChangeHistoryTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testAppendChange();
    void testRevertBeforeChange();
};

}

#endif
