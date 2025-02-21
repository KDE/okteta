/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilesynchronizertest.hpp"

// test object
#include <testdocumentfilesynchronizer.hpp>
#include "testdocumentfileloadjob.hpp"
// test utils
#include <util/filesystem.hpp>
#include <testdocument.hpp>
// lib
#include <abstractloadjob.hpp>
#include <abstractconnectjob.hpp>
#include <abstractsynctoremotejob.hpp>
#include <abstractsyncfromremotejob.hpp>
#include <abstractsyncwithremotejob.hpp>
// Qt
#include <QTest>
#include <QSignalSpy>
#include <QByteArray>
#include <QFile>
#include <QDataStream>
#include <QUrl>

static constexpr char TestDirectory[] = "testdocumentfilesynchronizertest";
static constexpr char TestFileName1[] = "test1.data";
static constexpr char TestFileName2[] = "test2.data";
static constexpr char TestData1[] = "TestData1";
static constexpr char TestData2[] = "TestData2";
static constexpr char Header1[] = "Header1";

void TestDocumentFileSynchronizerTest::writeToFile(const QString& filePath, const QByteArray& data, const QByteArray& header)
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

void TestDocumentFileSynchronizerTest::initTestCase()
{
    mFileSystem = std::make_unique<TestFileSystem>(QLatin1String(TestDirectory));
}

void TestDocumentFileSynchronizerTest::init()
{
    const QByteArray testData(TestData1);
    const QString filePath = mFileSystem->createFilePath(QLatin1String(TestFileName1));

    writeToFile(filePath, testData);
}

void TestDocumentFileSynchronizerTest::cleanupTestCase()
{
    mFileSystem.reset();
}

void TestDocumentFileSynchronizerTest::checkFileContent(const QUrl& fileUrl, const QByteArray& data,
                                                        const QByteArray& header)
{
    auto synchronizer = std::make_unique<Kasten::TestDocumentFileSynchronizer>(header);
    auto* loadJob = new Kasten::TestDocumentFileLoadJob(std::move(synchronizer), fileUrl);

    const bool loadSuccess = loadJob->exec();
    QVERIFY(loadSuccess);
    auto document = loadJob->releaseDocument();

    auto* testDocument = qobject_cast<Kasten::TestDocument*>(document.get());

    QVERIFY(testDocument != nullptr);
    QCOMPARE(*testDocument->data(), data);
}

// ------------------------------------------------------------------ tests ----

void TestDocumentFileSynchronizerTest::testLoadSaveFile()
{
    const QByteArray otherData(TestData2);
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(TestFileName1)));

    auto synchronizer = std::make_unique<Kasten::TestDocumentFileSynchronizer>();
    auto* loadJob = new Kasten::TestDocumentFileLoadJob(std::move(synchronizer), fileUrl);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(loadSuccess);
    auto document = loadJob->releaseDocument();

    auto* testDocument = qobject_cast<Kasten::TestDocument*>(document.get());
    QVERIFY(testDocument != nullptr);
    // change and save
    testDocument->setData(otherData);
    document->synchronizer()->startSyncToRemote()->exec();

    // now delete document and load new
    document.reset();

    checkFileContent(fileUrl, otherData);
}

void TestDocumentFileSynchronizerTest::testLoadReloadFile()
{
    const QByteArray otherData(TestData2);
    const QString filePath = mFileSystem->createFilePath(QLatin1String(TestFileName1));
    const QUrl fileUrl = QUrl::fromLocalFile(QString(filePath));

    auto synchronizer = std::make_unique<Kasten::TestDocumentFileSynchronizer>();
    auto* loadJob = new Kasten::TestDocumentFileLoadJob(std::move(synchronizer), fileUrl);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(loadSuccess);
    auto document = loadJob->releaseDocument();

    auto* testDocument = qobject_cast<Kasten::TestDocument*>(document.get());
    QVERIFY(testDocument != nullptr);

    // change on disc and reload
    writeToFile(filePath, otherData);
    // ? use QTest::kWaitForSignal (QObject* obj, const char* signal, int timeout=0)
//     QCOMPARE( document->syncStates(), Kasten::TestDocument::RemoteHasChanges );

    document->synchronizer()->startSyncFromRemote()->exec();

    QCOMPARE(*testDocument->data(), otherData);
}

void TestDocumentFileSynchronizerTest::testChangeFile()
{
    const QByteArray data(TestData1);
    const QByteArray otherData(TestData2);
    const QUrl fileUrl1 = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(TestFileName1)));
    const QString filePath2 = mFileSystem->createFilePath(QLatin1String(TestFileName2));
    const QUrl fileUrl2 = QUrl::fromLocalFile(filePath2);

    // load from 1
    auto synchronizer = std::make_unique<Kasten::TestDocumentFileSynchronizer>();
    auto* loadJob = new Kasten::TestDocumentFileLoadJob(std::move(synchronizer), fileUrl1);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(loadSuccess);
    auto document = loadJob->releaseDocument();

    // prepare 2 and overwrite
    writeToFile(filePath2, otherData);
    document->synchronizer()->startSyncWithRemote(fileUrl2, Kasten::AbstractModelSynchronizer::ReplaceRemote)->exec();

    // now delete document and load new
    document.reset();

    checkFileContent(fileUrl2, data);
}

void TestDocumentFileSynchronizerTest::testHeader()
{
    const QByteArray header(Header1);
    const QByteArray otherData(TestData2);
    const QString filePath = mFileSystem->createFilePath(QLatin1String(TestFileName1));
    const QUrl fileUrl = QUrl::fromLocalFile(filePath);

// TODO: failing calls in AbstractDocumentFileSystemSynchronizer trigger GUI here, so far it worked ;)
#if 0
    // try to load false header
    auto* synchronizer = new Kasten::TestDocumentFileSynchronizer(fileUrl, header);
    Kasten::AbstractDocument* document = synchronizer->document();
    QVERIFY(document == 0);
    delete synchronizer;

    // try to reload false header
    synchronizer = new Kasten::TestDocumentFileSynchronizer(fileUrl);
    document = synchronizer->document();
    QVERIFY(document != 0);

    writeToFile(filePath, otherData, header);
    bool success = document->synchronizer()->syncFromRemote();
    QCOMPARE(success, false);

    delete document;
#endif
    // try to connect to false header
    // TODO: we overwrite anyway
//     Kasten::TestDocument* testDocument = new Kasten::TestDocument();
//     document = testDocument;
//     testDocument->setData( otherData );

//     Kasten::TestDocumentFileSynchronizer* synchronizer =
//         new Kasten::TestDocumentFileSynchronizer( document, fileUrl, Kasten::TestDocumentFileSynchronizer::ReplaceRemote );
//     QVERIFY( synchronizer->document() == 0 );

//     delete document;
}

QTEST_GUILESS_MAIN(TestDocumentFileSynchronizerTest)

#include "moc_testdocumentfilesynchronizertest.cpp"
