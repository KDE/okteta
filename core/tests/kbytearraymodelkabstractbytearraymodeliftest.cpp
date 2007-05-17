/***************************************************************************
                          kbytearraymodelkabstractbytearraymodeliftest.cpp  -  description
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


#include "kbytearraymodelkabstractbytearraymodeliftest.h"

// test object
#include <kbytearraymodel.h>
// Qt
#include <QtTest/QtTest>


static const int ByteArrayModelSize = 60;


KHECore::KAbstractByteArrayModel *KByteArrayModelKAbstractByteArrayModelIfTest::createByteArrayModel()
{
  KHECore::KByteArrayModel *ByteArrayModel = new KHECore::KByteArrayModel( ByteArrayModelSize );
  ByteArrayModel->setReadOnly( false );

  return ByteArrayModel;
}

void KByteArrayModelKAbstractByteArrayModelIfTest::deleteByteArrayModel(
       KHECore::KAbstractByteArrayModel *ByteArrayModel )
{
  delete ByteArrayModel;
}


QTEST_MAIN( KByteArrayModelKAbstractByteArrayModelIfTest )
