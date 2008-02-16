/***************************************************************************
                          insertpiecetablechangeabstractpiecetablechangeiftest.h  -  description
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



#ifndef INSERTPIECETABLECHANGEABSTRACTPIECETABLECHANGEIFTEST_H
#define INSERTPIECETABLECHANGEABSTRACTPIECETABLECHANGEIFTEST_H


// test
#include "abstractpiecetablechangeiftest.h"

namespace KPieceTable
{

class InsertPieceTableChangeAbstractPieceTableChangeIfTest : public AbstractPieceTableChangeIfTest
{
  protected: // AbstractPieceTableChangeIfTest API
    virtual AbstractPieceTableChange *createPieceTableChange();
    virtual void deletePieceTableChange( AbstractPieceTableChange *pieceTableChange );
    virtual void changePieceTable ( PieceTable *pieceTable );
};

}

#endif
