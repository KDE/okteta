/***************************************************************************
                          kpiecetablebytearraymodelkabstractbytearraymodeliftest.h  -  description
                            -------------------
    begin                : Sat Feb 2 2008
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



#ifndef KPIECETABLEBYTEARRAYMODELKABSTRACTBYTEARRAYMODELIFTEST_H
#define KPIECETABLEBYTEARRAYMODELKABSTRACTBYTEARRAYMODELIFTEST_H


// test
#include "kabstractbytearraymodeliftest.h"


class KPieceTableByteArrayModelKAbstractByteArrayModelIfTest : public KAbstractByteArrayModelIfTest
{
  protected: // KAbstractByteArrayModelIfTest API
    KHECore::KAbstractByteArrayModel *createByteArrayModel();
    void deleteByteArrayModel( KHECore::KAbstractByteArrayModel *byteArrayModel );
};

#endif
