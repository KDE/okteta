/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilesynchronizerfactorytest.hpp"

// test object
#include <testdocumentfilesynchronizerfactory.hpp>
// lib
#include <testdocument.hpp>
// test utils
#include <util/filesystem.hpp>
// Qt
#include <QTest>
#include <QSignalSpy>
#include <QByteArray>
#include <QFile>
#include <QDataStream>
#include <QUrl>

static constexpr char TestDirectory[] = "testdocumentfile1synchronizertest";
static constexpr char TestFileName1[] = "test1.data";
#if 0
static constexpr char TestFileName2[] = "test2.data";
static constexpr char NotExistingFileName[] = "not.existing";
static constexpr char NotExistingUrlName[] = "not://existing";
#endif
static constexpr char TestData1[] = "TestData1";
#if 0
static constexpr char TestData2[] = "TestData2";
static constexpr char Header1[] = "Header1";
static constexpr char Header2[] = "Header2";
#endif

void TestDocumentFileSynchronizerFactoryTest::writeToFile(const QString& filePath, const QByteArray& data, const QByteArray& header)
{
    QFile file;
    file.setFileName(filePath);
    file.open(QIODevice::WriteOnly);

    QDataStream outStream(&file);
    outStream.writeRawData(header.data(), header.size());
    outStream.writeRawData(data.data(), data.size());

    file.close();
}

void TestDocumentFileSynchronizerFactoryTest::initTestCase()
{
    mFileSystem = new TestFileSystem(QLatin1String(TestDirectory));
}

void TestDocumentFileSynchronizerFactoryTest::init()
{
    const QByteArray testData(TestData1);
    const QString filePath = mFileSystem->createFilePath(QLatin1String(TestFileName1));

    writeToFile(filePath, testData);
}

void TestDocumentFileSynchronizerFactoryTest::cleanupTestCase()
{
    delete mFileSystem;
}

void TestDocumentFileSynchronizerFactoryTest::checkFileContent(const QUrl& fileUrl, const QByteArray& data,
                                                               const QByteArray& header)
{
    Q_UNUSED(data)
    Q_UNUSED(fileUrl)
    auto* factory = new Kasten::TestDocumentFileSynchronizerFactory(header);
#if 0
    AbstractDocument* document = factory->loadNewDocument(fileUrl);
    Kasten::TestDocument* testDocument = qobject_cast<Kasten::TestDocument*>(document);
    QVERIFY(testDocument != 0);
    QCOMPARE(*testDocument->data(), data);

    delete document;
#endif
    delete factory;
}

// ------------------------------------------------------------------ tests ----
void TestDocumentFileSynchronizerFactoryTest::testCreate()
{
    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();

    QVERIFY(factory != nullptr);

    delete factory;
}
#if 0
void TestDocumentFileSynchronizerFactoryTest::testLoadFromFile()
{
    const QByteArray testData(TestData1);
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QStringLiteral(TestFileName1)));

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument(fileUrl);

    Kasten::TestDocument* testDocument = qobject_cast<Kasten::TestDocument*>(document);

    QVERIFY(document != 0);
    QVERIFY(testDocument != 0);
    QVERIFY(document->synchronizer() != 0);
    QCOMPARE(document->synchronizer()->document(), document);
    QCOMPARE(document->hasLocalChanges(), false);
    QCOMPARE(*testDocument->data(), testData);
    QCOMPARE(document->synchronizer()->url(), fileUrl);

    delete document;
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testLoadFromNotExistingUrl()
{
    const QUrl fileUrl(QString::fromLatin1(NotExistingUrlName));
    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument(fileUrl);

    QVERIFY(document == 0);
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testLoadFromNotExistingFile()
{
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QStringLiteral(NotExistingFileName)));
    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument(fileUrl);

    QVERIFY(document == 0);
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testLoadSaveFile()
{
    const QByteArray otherData(TestData2);
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QStringLiteral(TestFileName1)));

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument(fileUrl);

    Kasten::TestDocument* testDocument = qobject_cast<Kasten::TestDocument*>(document);
    QVERIFY(testDocument != 0);
    // change and save
    testDocument->setData(otherData);
    document->synchronizer()->syncToRemote();

    // now delete document and load new
    delete document;
    delete factory;

    checkFileContent(fileUrl, otherData);
}

void TestDocumentFileSynchronizerFactoryTest::testLoadReloadFile()
{
    const QByteArray otherData(TestData2);
    const QString filePath = mFileSystem->createFilePath(QStringLiteral(TestFileName1));
    const QUrl fileUrl = QUrl::fromLocalFile(filePath);

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument(fileUrl);

    Kasten::TestDocument* testDocument = qobject_cast<Kasten::TestDocument*>(document);
    QVERIFY(testDocument != 0);

    // change on disc and reload
    writeToFile(filePath, otherData);
    // ? use QTest::kWaitForSignal (QObject* obj, const char* signal, int timeout=0)
//     QCOMPARE( document->syncStates(), Kasten::TestDocument::RemoteHasChanges );

    document->synchronizer()->syncFromRemote();

    QCOMPARE(*testDocument->data(), otherData);

    delete document;
    delete factory;
}

void TestDocumentFileSynchronizerFactoryTest::testConnectToFile()
{
    const QByteArray otherData(TestData2);
    const QUrl fileUrl1 = QUrl::::fromLocalFile(mFileSystem->createFilePath(QStringLiteral(TestFileName1)));
    const QString filePath2 = mFileSystem->createFilePath(QStringLiteral(TestFileName2));
    const QUrl fileUrl2 = QUrl::fromLocalFile(filePath2);

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    Kasten::TestDocument* testDocument = new Kasten::TestDocument();
    AbstractDocument* document = testDocument;
    testDocument->setData(otherData);

    // file 1
    bool result = factory->connectDocument(document, fileUrl1, AbstractModelSynchronizer::ReplaceRemote);
    QVERIFY(result);

    // file 2
    result = factory->connectDocument(document, fileUrl2, AbstractModelSynchronizer::ReplaceRemote);
    QVERIFY(result);

    // now delete document and load new
    delete document;
    delete factory;

    checkFileContent(fileUrl1, otherData);
    checkFileContent(fileUrl2, otherData);
}
#endif

QTEST_GUILESS_MAIN(TestDocumentFileSynchronizerFactoryTest)
