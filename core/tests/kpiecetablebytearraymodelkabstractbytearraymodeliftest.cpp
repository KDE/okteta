/***************************************************************************
                          kpiecetablebytearraymodelkabstractbytearraymodeliftest.cpp  -  description
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


#include "kpiecetablebytearraymodelkabstractbytearraymodeliftest.h"

// test object
#include <kpiecetablebytearraymodel.h>
// Qt
#include <QtTest/QtTest>


static const int ByteArrayModelSize = 60;


KHECore::KAbstractByteArrayModel *KPieceTableByteArrayModelKAbstractByteArrayModelIfTest::createByteArrayModel()
{
    KHECore::KPieceTableByteArrayModel *byteArrayModel = new KHECore::KPieceTableByteArrayModel( ByteArrayModelSize );
    byteArrayModel->setReadOnly( false );

    return byteArrayModel;
}

void KPieceTableByteArrayModelKAbstractByteArrayModelIfTest::deleteByteArrayModel(
       KHECore::KAbstractByteArrayModel *byteArrayModel )
{
    delete byteArrayModel;
}


QTEST_MAIN( KPieceTableByteArrayModelKAbstractByteArrayModelIfTest )
