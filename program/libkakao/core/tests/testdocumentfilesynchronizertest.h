/***************************************************************************
                          testdocumentfilesynchronizertest.h  -  description
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



#ifndef TESTDOCUMENTFILESYNCHRONIZERTEST_H
#define TESTDOCUMENTFILESYNCHRONIZERTEST_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QByteArray>

class TestFileSystem;
class QString;
class KUrl;

class TestDocumentFileSynchronizerTest : public QObject
{
  Q_OBJECT

  private:
    void writeToFile( const QString &filePath, const QByteArray &data, const QByteArray &header = QByteArray() );
    void checkFileContent( const KUrl &fileUrl, const QByteArray &data, const QByteArray &header = QByteArray() );

  private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();

  private Q_SLOTS: // test functions
    void testLoadFromFile();
    void testLoadSaveFile();
    void testLoadReloadFile();
    void testChangeFile();
    void testConnectToFile();
  private: // not working tests
    // TODO: need a way besides popups to report problems
    void testHeader();
    void testLoadFromNotExistingUrl();
    void testLoadFromNotExistingFile();

  private:
    TestFileSystem *mFileSystem;
};

#endif
