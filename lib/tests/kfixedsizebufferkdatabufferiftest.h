/***************************************************************************
                          kfixedsizebufferkabstractbytearraymodeliftest.h  -  description
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


// test specific
#include "kabstractbytearraymodeliftest.h"


#ifndef KFIXEDSIZEBYTEARRAYMODELKABSTRACTBYTEARRAYMODELIFTEST_H
#define KFIXEDSIZEBYTEARRAYMODELKABSTRACTBYTEARRAYMODELIFTEST_H


class KFixedSizeByteArrayModelAbstractByteArrayModelIfTest : public KAbstractByteArrayModelIfTest
{
  protected: // KAbstractByteArrayModelIfTest API
    KHE::KAbstractByteArrayModel *createByteArrayModel();
    void deleteByteArrayModel( KHE::KAbstractByteArrayModel *ByteArrayModel );
};

#endif
