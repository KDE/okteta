/*
    This file is part of the Kasten Framework, made within the KDE community.

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

#include "testdocumentfilesynchronizerfactorytest.h"

// test object
#include <testdocumentfilesynchronizerfactory.h>
// lib
#include <testdocument.h>
// test utils
#include <util/filesystem.h>
// Qt
#include <QTest>
#include <QSignalSpy>
#include <QByteArray>
#include <QFile>
#include <QDataStream>
#include <QUrl>


static const char TestDirectory[] = "testdocumentfile1synchronizertest";
static const char TestFileName1[] = "test1.data";
#if 0
static const char TestFileName2[] = "test2.data";
static const char NotExistingFileName[] = "not.existing";
static const char NotExistingUrlName[] = "not://existing";
#endif
static const char TestData1[] = "TestData1";
#if 0
static const char TestData2[] = "TestData2";
static const char Header1[] = "Header1";
static const char Header2[] = "Header2";
#endif


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

void TestDocumentFileSynchronizerFactoryTest::checkFileContent( const QUrl& fileUrl, const QByteArray& data,
                                                                const QByteArray& header )
{
Q_UNUSED( data )
Q_UNUSED( fileUrl )
    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory( header );
#if 0
    AbstractDocument* document = factory->loadNewDocument( fileUrl );
    Kasten::TestDocument* testDocument = qobject_cast<Kasten::TestDocument* >( document );
    QVERIFY( testDocument != 0 );
    QCOMPARE( *testDocument->data(), data );

    delete document;
#endif
    delete factory;
}

// ------------------------------------------------------------------ tests ----
void TestDocumentFileSynchronizerFactoryTest::testCreate()
{
    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();

    QVERIFY( factory != nullptr );

    delete factory;
}
#if 0
void TestDocumentFileSynchronizerFactoryTest::testLoadFromFile()
{
    const QByteArray testData( TestData1 );
    const QUrl fileUrl = QUrl::fromLocalFile( mFileSystem->createFilePath( QStringLiteral(TestFileName1) ) );

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument( fileUrl );

    Kasten::TestDocument* testDocument = qobject_cast<Kasten::TestDocument* >( document );

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
    const QUrl fileUrl( QString::fromLatin1(NotExistingUrlName) );
    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument( fileUrl );

    QVERIFY( document == 0 );
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testLoadFromNotExistingFile()
{
    const QUrl fileUrl = QUrl::fromLocalFile( mFileSystem->createFilePath( QStringLiteral(NotExistingFileName) ) );
    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument( fileUrl );

    QVERIFY( document == 0 );
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testLoadSaveFile()
{
    const QByteArray otherData( TestData2 );
    const QUrl fileUrl = QUrl::fromLocalFile( mFileSystem->createFilePath( QStringLiteral(TestFileName1) ) );

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument( fileUrl );

    Kasten::TestDocument* testDocument = qobject_cast<Kasten::TestDocument* >( document );
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
    const QString filePath = mFileSystem->createFilePath( QStringLiteral(TestFileName1) );
    const QUrl fileUrl = QUrl::fromLocalFile( filePath );

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument( fileUrl );

    Kasten::TestDocument* testDocument = qobject_cast<Kasten::TestDocument* >( document );
    QVERIFY( testDocument != 0 );

    // change on disc and reload
    writeToFile( filePath, otherData );
   // ? use QTest::kWaitForSignal (QObject* obj, const char* signal, int timeout=0)
//     QCOMPARE( document->syncStates(), Kasten::TestDocument::RemoteHasChanges );

    document->synchronizer()->syncFromRemote();

    QCOMPARE( *testDocument->data(), otherData );

    delete document;
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testConnectToFile()
{
    const QByteArray otherData( TestData2 );
    const QUrl fileUrl1 = QUrl::::fromLocalFile( mFileSystem->createFilePath( QStringLiteral(TestFileName1) ) );
    const QString filePath2 = mFileSystem->createFilePath( QStringLiteral(TestFileName2) );
    const QUrl fileUrl2 = QUrl::fromLocalFile( filePath2 );

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    Kasten::TestDocument* testDocument = new Kasten::TestDocument();
    AbstractDocument* document = testDocument;
    testDocument->setData( otherData );

    // file 1
    bool result = factory->connectDocument( document, fileUrl1, AbstractModelSynchronizer::ReplaceRemote );
    QVERIFY( result );

    // file 2
    result = factory->connectDocument( document, fileUrl2, AbstractModelSynchronizer::ReplaceRemote );
    QVERIFY( result );

    // now delete document and load new
    delete document;
    delete factory;

    checkFileContent( fileUrl1, otherData );
    checkFileContent( fileUrl2, otherData );
}
#endif

QTEST_GUILESS_MAIN( TestDocumentFileSynchronizerFactoryTest )
