/***************************************************************************
                          abstractpiecetablechangeiftest.h  -  description
                            -------------------
    begin                : Tue Feb 12 2008
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


#ifndef ABSTRACTPIECETABLECHANGEIFTEST_H
#define ABSTRACTPIECETABLECHANGEIFTEST_H


// Qt
#include <QtCore/QObject>

namespace KHE {
class KSection;
}

namespace KPieceTable
{

class AbstractPieceTableChange;
class PieceTable;

class AbstractPieceTableChangeIfTest : public QObject
{
  Q_OBJECT

  protected:
    AbstractPieceTableChangeIfTest();

  protected: // our API
    virtual AbstractPieceTableChange *createPieceTableChange() = 0;
    virtual void deletePieceTableChange( AbstractPieceTableChange *pieceTableChange ) = 0;

    virtual void changePieceTable( PieceTable *pieceTable ) = 0;

  protected:
    struct KTestData *prepareTestInsert();

  private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testMerge();
    void testRevertApply();

  private: // used in all tests
    /** pointer to the change to test */
    AbstractPieceTableChange *mPieceTableChange;
};

inline AbstractPieceTableChangeIfTest::AbstractPieceTableChangeIfTest() : mPieceTableChange( 0 ) {}

}

#endif
