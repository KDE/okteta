/***************************************************************************
                          testdocumentfilesynchronizertest.cpp  -  description
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


#include "testdocumentfilesynchronizertest.h"

// test object
#include <testdocumentfilesynchronizer.h>
// lib
#include <testdocument.h>
// test utils
#include <util/filesystem.h>
// KDE
#include <qtest_kde.h>
#include <KUrl>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QDataStream>


static const char TestDirectory[] = "testdocumentfile1synchronizertest";
static const char TestFileName[] = "test.data";
static const char NotExistingFileName[] = "not.existing";
static const char FileProtocolName[] = "file://";
static const char NotExistingUrlName[] = "not://existing";
static const char TestData1[] = "TestData1";
static const char TestData2[] = "TestData2";


void TestDocumentFileSynchronizerTest::writeToFile( const QString &filePath, const QByteArray &data )
{
    QFile file;
    file.setFileName( filePath );
    file.open( QIODevice::WriteOnly );

    QDataStream outStream( &file );
    outStream.writeRawData( data.data(), data.size() );

    file.close();
}

void TestDocumentFileSynchronizerTest::initTestCase()
{
    mFileSystem = new TestFileSystem( QLatin1String(TestDirectory) );
}

void TestDocumentFileSynchronizerTest::init()
{
    const QByteArray testData( TestData1 );
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName) );

    writeToFile( filePath, testData );
}

void TestDocumentFileSynchronizerTest::cleanupTestCase()
{
    delete mFileSystem;
}


// ------------------------------------------------------------------ tests ----

void TestDocumentFileSynchronizerTest::testLoadFromFile()
{
    const QByteArray testData( TestData1 );
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName) ).prepend( FileProtocolName );
    KAbstractDocument *document = loadTestDocumentFromFile( fileUrl );

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );

    QVERIFY( document != 0 );
    QVERIFY( testDocument != 0 );
    QVERIFY( document->synchronizer() != 0 );
    QCOMPARE( document->synchronizer()->document(), document );
    QCOMPARE( document->hasLocalChanges(), false );
    QCOMPARE( *testDocument->data(), testData );
    QCOMPARE( document->synchronizer()->url(), fileUrl );

    delete document;
}


void TestDocumentFileSynchronizerTest::testLoadFromNotExistingUrl()
{
    const KUrl fileUrl( QString::fromLatin1(NotExistingUrlName) );
    KAbstractDocument *document = loadTestDocumentFromFile( fileUrl );

    QVERIFY( document == 0 );
}

void TestDocumentFileSynchronizerTest::testLoadFromNotExistingFile()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(NotExistingFileName) ).prepend( FileProtocolName );
    KAbstractDocument *document = loadTestDocumentFromFile( fileUrl );

    QVERIFY( document == 0 );
}

void TestDocumentFileSynchronizerTest::testLoadSaveFile()
{
    const QByteArray otherData( TestData2 );
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName) ).prepend( FileProtocolName );
    KAbstractDocument *document = loadTestDocumentFromFile( fileUrl );

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );
    // change and save
    testDocument->setData( otherData );
    document->synchronizer()->synchToRemote();

    // now delete document and load new
    delete document;

    document = loadTestDocumentFromFile( fileUrl );
    testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );
    QCOMPARE( *testDocument->data(), otherData );

    delete document;
}

void TestDocumentFileSynchronizerTest::testLoadReloadFile()
{
    const QByteArray otherData( TestData2 );
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName) );
    const KUrl fileUrl = QString( filePath ).prepend( FileProtocolName );
    KAbstractDocument *document = loadTestDocumentFromFile( fileUrl );

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );

    // change on disc and reload
    writeToFile( filePath, otherData );
   // ? use QTest::kWaitForSignal (QObject *obj, const char *signal, int timeout=0)
//     QCOMPARE( document->synchronizationStates(), TestDocument::RemoteHasChanges );

    document->synchronizer()->synchFromRemote();

    QCOMPARE( *testDocument->data(), otherData );

    delete document;
}

void TestDocumentFileSynchronizerTest::testConnectToFile()
{
    const QByteArray otherData( TestData2 );
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName) ).prepend( FileProtocolName );

    TestDocument *testDocument = new TestDocument();
    KAbstractDocument *document = testDocument;
    testDocument->setData( otherData );

    bool result = synchDocumentWithFile( document, fileUrl, TestDocumentFileSynchronizer::ReplaceRemote );

    // now delete document and load new
    delete document;

    document = loadTestDocumentFromFile( fileUrl );
    testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );
    QCOMPARE( *testDocument->data(), otherData );

    delete document;
}


QTEST_KDEMAIN_CORE( TestDocumentFileSynchronizerTest )
