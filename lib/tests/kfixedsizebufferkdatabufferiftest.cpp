/***************************************************************************
                          kfixedsizebufferkdatabufferiftest.cpp  -  description
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
#include <kfixedsizebuffer.h>
// test specific
#include "kfixedsizebufferkdatabufferiftest.h"


using namespace KHE;

static const int FixedSizeByteArrayModelSize = 60;



KHE::KAbstractByteArrayModel *KFixedSizeByteArrayModelAbstractByteArrayModelIfTest::createByteArrayModel()
{
  return new KFixedSizeBuffer( FixedSizeByteArrayModelSize );
}

void KFixedSizeByteArrayModelAbstractByteArrayModelIfTest::deleteByteArrayModel( KHE::KAbstractByteArrayModel *ByteArrayModel )
{
  delete ByteArrayModel;
}


QTEST_MAIN( KFixedSizeByteArrayModelAbstractByteArrayModelIfTest )
