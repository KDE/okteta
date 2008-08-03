/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "testdocumentfilesynchronizertest.h"

// test object
#include <testdocumentfilesynchronizer.h>
// test utils
#include <util/filesystem.h>
#include <testdocument.h>
// lib
#include <abstractloadjob.h>
#include <abstractconnectjob.h>
#include <abstractsynctoremotejob.h>
#include <abstractsyncfromremotejob.h>
#include <abstractsyncwithremotejob.h>
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


void TestDocumentFileSynchronizerTest::writeToFile( const QString &filePath, const QByteArray &data, const QByteArray &header )
{
    QFile file;
    file.setFileName( filePath );
    file.open( QIODevice::WriteOnly );

    QDataStream outStream( &file );
    outStream.writeRawData( header.data(), header.size() );
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
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName1) );

    writeToFile( filePath, testData );
}

void TestDocumentFileSynchronizerTest::cleanupTestCase()
{
    delete mFileSystem;
}

void TestDocumentFileSynchronizerTest::checkFileContent( const KUrl &fileUrl, const QByteArray &data,
                                                         const QByteArray &header )
{

    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer( header );
    synchronizer->startLoad( fileUrl )->exec();
    KAbstractDocument *document = synchronizer->document();

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );

    QVERIFY( testDocument != 0 );
    QCOMPARE( *testDocument->data(), data );

    delete document;
}

// ------------------------------------------------------------------ tests ----

void TestDocumentFileSynchronizerTest::testLoadFromFile()
{
    const QByteArray testData( TestData1 );
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName1) ).prepend( FileProtocolName );

    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer();
    synchronizer->startLoad( fileUrl )->exec();
    KAbstractDocument *document = synchronizer->document();

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
    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer();
    AbstractLoadJob *loadJob = synchronizer->startLoad( fileUrl );
    loadJob->exec();
    KAbstractDocument *document = synchronizer->document();

    QVERIFY( document == 0 );
    delete synchronizer;
}

void TestDocumentFileSynchronizerTest::testLoadFromNotExistingFile()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(NotExistingFileName) ).prepend( FileProtocolName );
    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer();
    synchronizer->startLoad( fileUrl )->exec();
    KAbstractDocument *document = synchronizer->document();

    QVERIFY( document == 0 );
    delete synchronizer;
}

void TestDocumentFileSynchronizerTest::testLoadSaveFile()
{
    const QByteArray otherData( TestData2 );
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName1) ).prepend( FileProtocolName );
    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer();
    synchronizer->startLoad( fileUrl )->exec();
    KAbstractDocument *document = synchronizer->document();

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );
    // change and save
    testDocument->setData( otherData );
    document->synchronizer()->startSyncToRemote()->exec();

    // now delete document and load new
    delete document;

    checkFileContent( fileUrl, otherData );
}

void TestDocumentFileSynchronizerTest::testLoadReloadFile()
{
    const QByteArray otherData( TestData2 );
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName1) );
    const KUrl fileUrl = QString( filePath ).prepend( FileProtocolName );

    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer();
    synchronizer->startLoad( fileUrl )->exec();
    KAbstractDocument *document = synchronizer->document();

    TestDocument *testDocument = qobject_cast<TestDocument *>( document );
    QVERIFY( testDocument != 0 );

    // change on disc and reload
    writeToFile( filePath, otherData );
   // ? use QTest::kWaitForSignal (QObject *obj, const char *signal, int timeout=0)
//     QCOMPARE( document->synchronizationStates(), TestDocument::RemoteHasChanges );

    document->synchronizer()->startSyncFromRemote()->exec();

    QCOMPARE( *testDocument->data(), otherData );

    delete document;
}

void TestDocumentFileSynchronizerTest::testChangeFile()
{
    const QByteArray data( TestData1 );
    const QByteArray otherData( TestData2 );
    const KUrl fileUrl1 = mFileSystem->createFilePath( QLatin1String(TestFileName1) ).prepend( FileProtocolName );
    const QString filePath2 = mFileSystem->createFilePath( QLatin1String(TestFileName2) );
    const KUrl fileUrl2 = QString( filePath2 ).prepend( FileProtocolName );

    // load from 1
    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer();
    synchronizer->startLoad( fileUrl1 )->exec();
    KAbstractDocument *document = synchronizer->document();

    // prepare 2 and overwrite
    writeToFile( filePath2, otherData );
    synchronizer->startSyncWithRemote( fileUrl2, AbstractModelSynchronizer::ReplaceRemote )->exec();

    // now delete document and load new
    delete document;

    checkFileContent( fileUrl2, data );
}

void TestDocumentFileSynchronizerTest::testConnectToFile()
{
    const QByteArray otherData( TestData2 );
    const KUrl fileUrl1 = mFileSystem->createFilePath( QLatin1String(TestFileName1) ).prepend( FileProtocolName );
    const QString filePath2 = mFileSystem->createFilePath( QLatin1String(TestFileName2) );
    const KUrl fileUrl2 = QString( filePath2 ).prepend( FileProtocolName );

    TestDocument *testDocument = new TestDocument();
    KAbstractDocument *document = testDocument;
    testDocument->setData( otherData );

    // file 1
    TestDocumentFileSynchronizer *synchronizer =
        new TestDocumentFileSynchronizer();
    synchronizer->startConnect( document, fileUrl1, TestDocumentFileSynchronizer::ReplaceRemote )->exec();
    QCOMPARE( synchronizer->document(), document );

    // file 2
    synchronizer =
        new TestDocumentFileSynchronizer();
    synchronizer->startConnect( document, fileUrl2, TestDocumentFileSynchronizer::ReplaceRemote )->exec();
    QCOMPARE( synchronizer->document(), document );

    // now delete document and load new
    delete document;

    checkFileContent( fileUrl1, otherData );
    checkFileContent( fileUrl2, otherData );
}

void TestDocumentFileSynchronizerTest::testHeader()
{
    const QByteArray header( Header1);
    const QByteArray otherData( TestData2 );
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName1) );
    const KUrl fileUrl = QString( filePath ).prepend( FileProtocolName );

// TODO: failing calls in KAbstractDocumentFileSystemSynchronizer trigger GUI here, so far it worked ;)
#if 0
    // try to load false header
    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer( fileUrl, header );
    KAbstractDocument *document = synchronizer->document();
    QVERIFY( document == 0 );
    delete synchronizer;

    // try to reload false header
    synchronizer = new TestDocumentFileSynchronizer( fileUrl );
    document = synchronizer->document();
    QVERIFY( document != 0 );

    writeToFile( filePath, otherData, header );
    bool success = document->synchronizer()->syncFromRemote();
    QCOMPARE( success, false );

    delete document;
#endif
    // try to connect to false header
    // TODO: we overwrite anyway
//     TestDocument *testDocument = new TestDocument();
//     document = testDocument;
//     testDocument->setData( otherData );

//     TestDocumentFileSynchronizer *synchronizer =
//         new TestDocumentFileSynchronizer( document, fileUrl, TestDocumentFileSynchronizer::ReplaceRemote );
//     QVERIFY( synchronizer->document() == 0 );

//     delete document;
}


QTEST_KDEMAIN_CORE( TestDocumentFileSynchronizerTest )
