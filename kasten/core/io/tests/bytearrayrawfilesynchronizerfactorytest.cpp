/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilesynchronizerfactorytest.hpp"

// test object
#include <bytearrayrawfilesynchronizerfactory.hpp>
// lib
#include <bytearraydocument.hpp>
// test utils
#include <util/filesystem.hpp>
#include <util/fill.hpp>
// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// Kasten core
#include <Kasten/AbstractLoadJob>
#include <Kasten/AbstractConnectJob>
// Qt
#include <QTest>
#include <QSignalSpy>
#include <QByteArray>
#include <QFile>
#include <QDataStream>

namespace Kasten {

static constexpr char TestDirectory[] = "bytearrayrawfilesynchronizerfactorytest";
static constexpr char TestFileName[] = "test.data";
static constexpr char NotExistingUrl[] = "notexisting://";
static constexpr int TestDataSize = 50;
static constexpr char TestDataChar = 0;

void ByteArrayRawFileSynchronizerFactoryTest::initTestCase()
{
    QByteArray byteArray(TestDataSize, TestDataChar);
    ::textureByteArray(&byteArray);

    mFileSystem = std::make_unique<TestFileSystem>(QLatin1String(TestDirectory));
    const QString filePath = mFileSystem->createFilePath(QLatin1String(TestFileName));
    QFile file;
    file.setFileName(filePath);
    const bool openSuccess = file.open(QIODevice::WriteOnly);
    QVERIFY(openSuccess);

    QDataStream outStream(&file);
    outStream.writeRawData(byteArray.data(), byteArray.size());

    file.close();

//     QDir dir(mDataDir);
//     QVERIFY(dir.mkdir("Europe"));
//     QFile::copy(QString::fromLatin1(KDESRCDIR) + QStringLiteral("/Paris"), mDataDir + QStringLiteral("/Europe/Paris"));
}

void ByteArrayRawFileSynchronizerFactoryTest::cleanupTestCase()
{
    mFileSystem.reset();
}

#if 0
void ByteArrayRawFileSynchronizerFactoryTest::init()
{
    ByteArrayModel = createByteArrayModel();

    mModifiedSpy =  new QSignalSpy(ByteArrayModel, SIGNAL(modified(bool)));
}
#endif

void ByteArrayRawFileSynchronizerFactoryTest::testCreate()
{
    auto factory = std::make_unique<Kasten::ByteArrayRawFileSynchronizerFactory>();

    QVERIFY(factory != nullptr);
}

void ByteArrayRawFileSynchronizerFactoryTest::testLoadFromUrl()
{
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(TestFileName)));

    auto factory = std::make_unique<ByteArrayRawFileSynchronizerFactory>();
    auto* loadJob = factory->startLoad(fileUrl);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(loadSuccess);
    auto document = loadJob->releaseDocument();

    auto* byteArrayDocument = qobject_cast<ByteArrayDocument*>(document.get());

    QVERIFY(document != nullptr);
    QVERIFY(byteArrayDocument != nullptr);
    QVERIFY(document->synchronizer() != nullptr);
    QCOMPARE(document->synchronizer()->document(), document.get());
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);
    QCOMPARE(document->synchronizer()->localSyncState(), Kasten::LocalInSync);
    QCOMPARE(document->synchronizer()->remoteSyncState(), Kasten::RemoteInSync);

    QCOMPARE(document->synchronizer()->url(), fileUrl);
}

void ByteArrayRawFileSynchronizerFactoryTest::testLoadFromNotExistingUrl()
{
    const QUrl fileUrl = QUrl(QLatin1String(NotExistingUrl));

    auto factory = std::make_unique<ByteArrayRawFileSynchronizerFactory>();
    auto* loadJob = factory->startLoad(fileUrl);
    const bool loadSuccess = loadJob->exec();
    QVERIFY(!loadSuccess);
    auto document = loadJob->releaseDocument();

    QVERIFY(document == nullptr);
}

void ByteArrayRawFileSynchronizerFactoryTest::testNewSaveAsToUrl()
{
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(TestFileName)));

    auto document = std::make_unique<Kasten::ByteArrayDocument>(QStringLiteral("New created for test."));
    auto* byteArray = qobject_cast<Okteta::PieceTableByteArrayModel*>(document->content());

    // fill array
    QByteArray testData(TestDataSize, TestDataChar);
    ::textureByteArray(&testData);
    byteArray->setData(testData);

    // save
    auto factory = std::make_unique<ByteArrayRawFileSynchronizerFactory>();
    factory->startConnect(document.get(), fileUrl, AbstractModelSynchronizer::ReplaceRemote)->exec();

//     // load into other and...
//     ByteArrayDocument* otherDocument = new ByteArrayDocument( filePath );

//     QVERIFY( document != 0 );

//     // compare with old
//     Okteta::PieceTableByteArrayModel *otherByteArray = document->content();
//     QCOMPARE( byteArray->size(), otherByteArray->size() );
//     QVERIFY( qstrncmp(byteArray->data(),otherByteArray->data(),byteArray->size()) == 0 );
}

#if 0
void ByteArrayRawFileSynchronizerFactoryTest::testSaveToFile()
{
    const QString filePath = mFileSystem->createFilePath(QStringLiteral(TestFileName));

    ByteArrayDocument* document = new ByteArrayDocument();
    Okteta::PieceTableByteArrayModel* byteArray = document->content();

    // fill array
    byteArray->

    // save
    document->setLocalFilePath(filePath);
    document->save();
TODO: save with path as parameter? or set separately? save as copy?

    // load into other and...
    ByteArrayDocument * otherDocument = new ByteArrayDocument(filePath);

    QVERIFY(document != 0);

    // compare with old
    Okteta::PieceTableByteArrayModel* otherByteArray = document->content();
    QCOMPARE(byteArray->size(), otherByteArray->size());
    QVERIFY(qstrncmp(byteArray->data(), otherByteArray->data(), byteArray->size()) == 0);

    delete document;
}
#endif

}

QTEST_GUILESS_MAIN(Kasten::ByteArrayRawFileSynchronizerFactoryTest)

#include "moc_bytearrayrawfilesynchronizerfactorytest.cpp"
