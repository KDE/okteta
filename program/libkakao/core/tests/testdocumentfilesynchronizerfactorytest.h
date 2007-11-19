/***************************************************************************
                          testdocumentfilesynchronizerfactorytest.h  -  description
                            -------------------
    begin                : Mon Nov 19 2007
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



#ifndef TESTDOCUMENTFILESYNCHRONIZERFACTORYTEST_H
#define TESTDOCUMENTFILESYNCHRONIZERFACTORYTEST_H

// Qt
#include <QtCore/QObject>

class TestFileSystem;
class QString;
class QByteArray;

class TestDocumentFileSynchronizerFactoryTest : public QObject
{
  Q_OBJECT

  private:
    void writeToFile( const QString &filePath, const QByteArray &data );

  private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();

  private Q_SLOTS: // test functions
    void testCreate();
    void testLoadFromFile();
    void testLoadSaveFile();
    void testLoadReloadFile();
    void testConnectToFile();
  private: // not working tests
    // TODO: need a way besides popups to report problems
    void testLoadFromNotExistingUrl();
    void testLoadFromNotExistingFile();

  private:
    TestFileSystem *mFileSystem;
};

#endif
