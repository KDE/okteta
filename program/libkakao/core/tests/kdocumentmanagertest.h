/***************************************************************************
                          kdocumentmanagertest.h  -  description
                            -------------------
    begin                : Thu Oct 25 2007
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



#ifndef KDOCUMENTMANAGERTEST_H
#define KDOCUMENTMANAGERTEST_H

// Qt
#include <QtCore/QObject>

class KAbstractDocument;
class QSignalSpy;


class KDocumentManagerTest : public QObject
{
  Q_OBJECT

  private:
    void checkAdded( QSignalSpy *changedSpy, KAbstractDocument *document );
    void checkRemoving( QSignalSpy *changedSpy, KAbstractDocument *document );

  private Q_SLOTS: // test functions
    void testConstructor();
    void testAddRemove();
    void testCanClose();

};

#endif
