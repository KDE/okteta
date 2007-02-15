/***************************************************************************
                          kfixedsizebytearraymodelkabstractbytearraymodeliftest.cpp  -  description
                            -------------------
    begin                : Son Mai 7 2006
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


// qt specific
#include <QtTest>
// lib specific
#include <kfixedsizebytearraymodel.h>
// test specific
#include "kfixedsizebytearraymodelkabstractbytearraymodeliftest.h"


static const int FixedSizeByteArrayModelSize = 60;



KHECore::KAbstractByteArrayModel *KFixedSizeByteArrayModelAbstractByteArrayModelIfTest::createByteArrayModel()
{
  return new KHECore::KFixedSizeByteArrayModel( FixedSizeByteArrayModelSize );
}

void KFixedSizeByteArrayModelAbstractByteArrayModelIfTest::deleteByteArrayModel( KHECore::KAbstractByteArrayModel *ByteArrayModel )
{
  delete ByteArrayModel;
}


QTEST_MAIN( KFixedSizeByteArrayModelAbstractByteArrayModelIfTest )
