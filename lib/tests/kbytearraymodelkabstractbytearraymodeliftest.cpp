/***************************************************************************
                          kbytearraymodelkabstractbytearraymodeliftest.cpp  -  description
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


// qt specific
#include <QtTest>
// lib specific
#include <kbytearraymodel.h>
// app specific
#include "kbytearraymodelkabstractbytearraymodeliftest.h"


using namespace KHE;


static const int ByteArrayModelSize = 60;


KHE::KAbstractByteArrayModel *KByteArrayModelKAbstractByteArrayModelIfTest::createByteArrayModel()
{
  KByteArrayModel *ByteArrayModel = new KByteArrayModel( ByteArrayModelSize );
  ByteArrayModel->setReadOnly( false );

  return ByteArrayModel;
}

void KByteArrayModelKAbstractByteArrayModelIfTest::deleteByteArrayModel(
       KHE::KAbstractByteArrayModel *ByteArrayModel )
{
  delete ByteArrayModel;
}


QTEST_MAIN( KByteArrayModelKAbstractByteArrayModelIfTest )
