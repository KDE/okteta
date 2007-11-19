/***************************************************************************
                          testdocumentfilesynchronizerfactorytest.cpp  -  description
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


#include "testdocumentfilesynchronizerfactorytest.h"

// test object
#include <testdocumentfilesynchronizerfactory.h>
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
static const char TestFileName1[] = "test1.data";
static const char TestFileName2[] = "test2.data";
static const char NotExistingFileName[] = "not.existing";
static const char FileProtocolName[] = "file://";
static const char NotExistingUrlName[] = "not://existing";
static const char TestData1[] = "TestData1";
static const char TestData2[] = "TestData2";
static const char Header1[] = "Header1";
static const char Header2[] = "Header2";


void TestDocumentFileSynchronizerFactoryTest::writeToFile( const QString &filePath, const QByteArray &data, const QByteArray &header  )
{
    QFile file;
    file.setFileName( filePath );
    file.open( QIODevice::WriteOnly );

    QDataStream outStream( &file );
    outStream.writeRawData( header.data(), header.size() );
    outStream.writeRawData( data.data(), data.size() );

    file.close();
}

void TestDocumentFileSynchronizerFactoryTest::initTestCase()
{
    mFileSystem = new TestFileSystem( QLatin1String(TestDirectory) );
}

void TestDocumentFileSynchronizerFactoryTest::init()
{
    const QByteArray testData( TestData1 );
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName1) );

    writeToFile( filePath, testData );
}

void TestDocumentFileSynchronizerFactoryTest::cleanupTestCase()
{
    delete mFileSystem;
}

void TestDocumentFileSynchronizerFactoryTest::checkFileContent( const KUrl &fileUrl, const QByteArray &data,
                                                                const QByteArray &header )
{
    TestDocumentFileSynchronizerFactory *factory = new TestDocumentFileSynchronizerFactory( header );

    KAbstractDocument *document = factory->loadNewDocument( fileUrl );
    TestDocument *testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );
    QCOMPARE( *testDocument->data(), data );

    delete document;
    delete factory;
}

// ------------------------------------------------------------------ tests ----
void TestDocumentFileSynchronizerFactoryTest::testCreate()
{
    TestDocumentFileSynchronizerFactory *factory = new TestDocumentFileSynchronizerFactory();

    QVERIFY( factory != 0 );

    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testLoadFromFile()
{
    const QByteArray testData( TestData1 );
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName1) ).prepend( FileProtocolName );

    TestDocumentFileSynchronizerFactory *factory = new TestDocumentFileSynchronizerFactory();
    KAbstractDocument *document = factory->loadNewDocument( fileUrl );

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );

    QVERIFY( document != 0 );
    QVERIFY( testDocument != 0 );
    QVERIFY( document->synchronizer() != 0 );
    QCOMPARE( document->synchronizer()->document(), document );
    QCOMPARE( document->hasLocalChanges(), false );
    QCOMPARE( *testDocument->data(), testData );
    QCOMPARE( document->synchronizer()->url(), fileUrl );

    delete document;
    delete factory;
}


void TestDocumentFileSynchronizerFactoryTest::testLoadFromNotExistingUrl()
{
    const KUrl fileUrl( QString::fromLatin1(NotExistingUrlName) );
    TestDocumentFileSynchronizerFactory *factory = new TestDocumentFileSynchronizerFactory();
    KAbstractDocument *document = factory->loadNewDocument( fileUrl );

    QVERIFY( document == 0 );
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testLoadFromNotExistingFile()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(NotExistingFileName) ).prepend( FileProtocolName );
    TestDocumentFileSynchronizerFactory *factory = new TestDocumentFileSynchronizerFactory();
    KAbstractDocument *document = factory->loadNewDocument( fileUrl );

    QVERIFY( document == 0 );
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testLoadSaveFile()
{
    const QByteArray otherData( TestData2 );
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName1) ).prepend( FileProtocolName );

    TestDocumentFileSynchronizerFactory *factory = new TestDocumentFileSynchronizerFactory();
    KAbstractDocument *document = factory->loadNewDocument( fileUrl );

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );
    // change and save
    testDocument->setData( otherData );
    document->synchronizer()->syncToRemote();

    // now delete document and load new
    delete document;
    delete factory;

    checkFileContent( fileUrl, otherData );
}

void TestDocumentFileSynchronizerFactoryTest::testLoadReloadFile()
{
    const QByteArray otherData( TestData2 );
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName1) );
    const KUrl fileUrl = QString( filePath ).prepend( FileProtocolName );

    TestDocumentFileSynchronizerFactory *factory = new TestDocumentFileSynchronizerFactory();
    KAbstractDocument *document = factory->loadNewDocument( fileUrl );

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );

    // change on disc and reload
    writeToFile( filePath, otherData );
   // ? use QTest::kWaitForSignal (QObject *obj, const char *signal, int timeout=0)
//     QCOMPARE( document->synchronizationStates(), TestDocument::RemoteHasChanges );

    document->synchronizer()->syncFromRemote();

    QCOMPARE( *testDocument->data(), otherData );

    delete document;
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testConnectToFile()
{
    const QByteArray otherData( TestData2 );
    const KUrl fileUrl1 = mFileSystem->createFilePath( QLatin1String(TestFileName1) ).prepend( FileProtocolName );
    const QString filePath2 = mFileSystem->createFilePath( QLatin1String(TestFileName2) );
    const KUrl fileUrl2 = QString( filePath2 ).prepend( FileProtocolName );

    TestDocumentFileSynchronizerFactory *factory = new TestDocumentFileSynchronizerFactory();
    TestDocument *testDocument = new TestDocument();
    KAbstractDocument *document = testDocument;
    testDocument->setData( otherData );

    // file 1
    bool result = factory->connectDocument( document, fileUrl1, KAbstractDocumentSynchronizer::ReplaceRemote );
    QVERIFY( result );

    // file 2
    result = factory->connectDocument( document, fileUrl2, KAbstractDocumentSynchronizer::ReplaceRemote );
    QVERIFY( result );

    // now delete document and load new
    delete document;
    delete factory;

    checkFileContent( fileUrl1, otherData );
    checkFileContent( fileUrl2, otherData );
}


QTEST_KDEMAIN_CORE( TestDocumentFileSynchronizerFactoryTest )
