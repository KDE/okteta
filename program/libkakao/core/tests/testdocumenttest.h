/***************************************************************************
                          testdocumenttest.h  -  description
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



#ifndef TESTDOCUMENTTEST_H
#define TESTDOCUMENTTEST_H

// sut
#include <kabstractdocument.h>
// Qt
#include <QtCore/QObject>

class QSignalSpy;
class QString;


class TestDocumentTest : public QObject
{
  Q_OBJECT

  private:
    void checkTitleChanged( QSignalSpy *titleChangedSpy, const QString &title );
    void checkSynchronizationStatesChanged( QSignalSpy *changedSpy, KAbstractDocument::SynchronizationStates states );

  private Q_SLOTS: // test functions
    void testConstructor();
    void testSetTitle();
    void testSetSynchronizationStates();

};

#endif
