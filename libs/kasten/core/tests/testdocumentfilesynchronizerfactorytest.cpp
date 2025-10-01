/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilesynchronizerfactorytest.hpp"

// test object
#include <testdocumentfilesynchronizerfactory.hpp>
#include <testdocumentfilesynchronizer.hpp>
// test utils
#include <testdocument.hpp>
#include <util/filesystem.hpp>
// lib
#include <abstractloadjob.hpp>
#include <abstractconnectjob.hpp>
// Qt
#include <QTest>
#include <QSignalSpy>
#include <QByteArray>
#include <QFile>
#include <QDataStream>
#include <QUrl>

static constexpr char TestDirectory[] = "testdocumentfilesynchronizerfactorytest";
static constexpr char TestFileName1[] = "test1.data";
static constexpr char TestFileName2[] = "test2.data";
static constexpr char NotExistingFileName[] = "not.existing";
static constexpr char NotExistingUrlName[] = "not://existing";
static constexpr char TestData1[] = "TestData1";
static constexpr char TestData2[] = "TestData2";
#if 0
static constexpr char Header1[] = "Header1";
static constexpr char Header2[] = "Header2";
#endif

void TestDocumentFileSynchronizerFactoryTest::writeToFile(const QString& filePath, const QByteArray& data, const QByteArray& header)
{
    QFile file;
    file.setFileName(filePath);
    const bool openSuccess = file.open(QIODevice::WriteOnly);
    QVERIFY(openSuccess);

    QDataStream outStream(&file);
    outStream.writeRawData(header.data(), header.size());
    outStream.writeRawData(data.data(), data.size());

    file.close();
}

void TestDocumentFileSynchronizerFactoryTest::initTestCase()
{
    mFileSystem = std::make_unique<TestFileSystem>(QLatin1String(TestDirectory));
}

void TestDocumentFileSynchronizerFactoryTest::init()
{
    const QByteArray testData(TestData1);
    const QString filePath = mFileSystem->createFilePath(QLatin1String(TestFileName1));

    writeToFile(filePath, testData);
}

void TestDocumentFileSynchronizerFactoryTest::cleanupTestCase()
{
    mFileSystem.reset();
}

void TestDocumentFileSynchronizerFactoryTest::checkFileContent(const QUrl& fileUrl, const QByteArray& data,
                                                               const QByteArray& header)
{
    auto factory = std::make_unique<Kasten::TestDocumentFileSynchronizerFactory>(header);
    auto* const loadJob = factory->startLoad(fileUrl);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(loadSuccess);
    auto document = loadJob->releaseDocument();

    auto* const testDocument = qobject_cast<Kasten::TestDocument*>(document.get());

    QVERIFY(testDocument != nullptr);
    QCOMPARE(*testDocument->data(), data);

}

// ------------------------------------------------------------------ tests ----
void TestDocumentFileSynchronizerFactoryTest::testCreate()
{
    auto factory = std::make_unique<Kasten::TestDocumentFileSynchronizerFactory>();

    QVERIFY(factory != nullptr);
}

void TestDocumentFileSynchronizerFactoryTest::testLoadFromFile()
{
    const QByteArray testData(TestData1);
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(TestFileName1)));

    auto facrory = std::make_unique<Kasten::TestDocumentFileSynchronizerFactory>();

    auto* const loadJob = facrory->startLoad(fileUrl);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(loadSuccess);
    auto document = loadJob->releaseDocument();

    auto* const testDocument = qobject_cast<Kasten::TestDocument*>(document.get());

    QVERIFY(document != nullptr);
    QVERIFY(testDocument != nullptr);
    QVERIFY(document->synchronizer() != nullptr);
    QCOMPARE(document->synchronizer()->document(), document.get());
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);
    QCOMPARE(*testDocument->data(), testData);
    QCOMPARE(document->synchronizer()->url(), fileUrl);
}

void TestDocumentFileSynchronizerFactoryTest::testLoadFromNotExistingUrl()
{
    const QUrl fileUrl = QUrl(QLatin1String(NotExistingUrlName));

    auto facrory = std::make_unique<Kasten::TestDocumentFileSynchronizerFactory>();

    auto* const loadJob = facrory->startLoad(fileUrl);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(!loadSuccess);
    auto document = loadJob->releaseDocument();

    QVERIFY(document == nullptr);
}

void TestDocumentFileSynchronizerFactoryTest::testLoadFromNotExistingFile()
{
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(NotExistingFileName)));

    auto facrory = std::make_unique<Kasten::TestDocumentFileSynchronizerFactory>();

    auto* const loadJob = facrory->startLoad(fileUrl);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(!loadSuccess);
    auto document = loadJob->releaseDocument();

    QVERIFY(document == nullptr);
}

#if 0
void TestDocumentFileSynchronizerFactoryTest::testLoadSaveFile()
{
    const QByteArray otherData(TestData2);
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QStringLiteral(TestFileName1)));

    Kasten::TestDocumentFileSynchronizerFactory* factory = new Kasten::TestDocumentFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument(fileUrl);

    auto* const testDocument = qobject_cast<Kasten::TestDocument*>(document);
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

    auto* const testDocument = qobject_cast<Kasten::TestDocument*>(document);
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
#endif

void TestDocumentFileSynchronizerFactoryTest::testConnectToFile()
{
    const QByteArray otherData(TestData2);
    const QUrl fileUrl1 = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(TestFileName1)));
    const QString filePath2 = mFileSystem->createFilePath(QLatin1String(TestFileName2));
    const QUrl fileUrl2 = QUrl::fromLocalFile(QString(filePath2));

    auto* const testDocument = new Kasten::TestDocument();
    auto document = std::unique_ptr<Kasten::AbstractDocument>(testDocument);
    testDocument->setData(otherData);

    auto facrory = std::make_unique<Kasten::TestDocumentFileSynchronizerFactory>();

    // file 1
    auto* const connectJob1 = facrory->startConnect(document.get(), fileUrl1, Kasten::TestDocumentFileSynchronizer::ReplaceRemote);
    const bool connectSuccess1 = connectJob1->exec();
    QVERIFY(connectSuccess1);

    // file 2
    auto* const connectJob2 = facrory->startConnect(document.get(), fileUrl2, Kasten::TestDocumentFileSynchronizer::ReplaceRemote);
    const bool connectSuccess2 = connectJob2->exec();
    QVERIFY(connectSuccess2);

    // now delete document and load new
    document.reset();

    checkFileContent(fileUrl1, otherData);
    checkFileContent(fileUrl2, otherData);
}

QTEST_GUILESS_MAIN(TestDocumentFileSynchronizerFactoryTest)

#include "moc_testdocumentfilesynchronizerfactorytest.cpp"
