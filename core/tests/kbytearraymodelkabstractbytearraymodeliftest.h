/***************************************************************************
                          kbytearraymodelkabstractbytearraymodeliftest.h  -  description
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



#ifndef KBYTEARRAYMODELKABSTRACTBYTEARRAYMODELIFTEST_H
#define KBYTEARRAYMODELKABSTRACTBYTEARRAYMODELIFTEST_H


// test
#include "kabstractbytearraymodeliftest.h"


class KByteArrayModelKAbstractByteArrayModelIfTest : public KAbstractByteArrayModelIfTest
{
  protected: // KAbstractByteArrayModelIfTest API
    KHECore::KAbstractByteArrayModel *createByteArrayModel();
    void deleteByteArrayModel( KHECore::KAbstractByteArrayModel *ByteArrayModel );
};

#endif
