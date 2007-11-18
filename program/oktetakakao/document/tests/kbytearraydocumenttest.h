/***************************************************************************
                          kbytearraydocumenttest.h  -  description
                            -------------------
    begin                : Wed Mai 23 2007
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



#ifndef KBYTEARRAYDOCUMENTTEST_H
#define KBYTEARRAYDOCUMENTTEST_H

// Qt
#include <QtCore/QObject>

class KByteArrayDocumentTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS: // test functions
    void testCreateNew();
    void testSetTitle();
};

#endif
