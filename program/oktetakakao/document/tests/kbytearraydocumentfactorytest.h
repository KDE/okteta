/***************************************************************************
                          kbytearraydocumentfactorytest.h  -  description
                            -------------------
    begin                : Fri Nov 16 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/



#ifndef KBYTEARRAYDOCUMENTFACTORYTEST_H
#define KBYTEARRAYDOCUMENTFACTORYTEST_H

// Qt
#include <QtCore/QObject>

class KByteArrayDocumentFactoryTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testCreate();
};

#endif
