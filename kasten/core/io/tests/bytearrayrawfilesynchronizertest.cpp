/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilesynchronizertest.hpp"

// test object
#include <bytearrayrawfilesynchronizer.hpp>
// lib
#include <bytearraydocument.hpp>
// test utils
#include <util/filesystem.hpp>
#include <util/fill.hpp>
// Kasten core
#include <Kasten/AbstractLoadJob>
#include <Kasten/AbstractConnectJob>
#include <Kasten/AbstractSyncToRemoteJob>
#include <Kasten/AbstractSyncWithRemoteJob>
// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// Qt
#include <QTest>
#include <QSignalSpy>
#include <QByteArray>
#include <QFile>
#include <QDataStream>

namespace Kasten {

static constexpr char TestDirectory[] = "bytearrayrawfilesynchronizertest";
static constexpr char TestFileName[] = "test.data";
static constexpr char NotExistingUrl[] = "notexisting://";
static constexpr int TestDataSize = 50;
static constexpr char TestDataChar = 0;

void ByteArrayRawFileSynchronizerTest::initTestCase()
{
    QByteArray byteArray(TestDataSize, TestDataChar);
    ::textureByteArray(&byteArray);

    mFileSystem = new TestFileSystem(QLatin1String(TestDirectory));
    const QString filePath = mFileSystem->createFilePath(QLatin1String(TestFileName));
    QFile file;
    file.setFileName(filePath);
    file.open(QIODevice::WriteOnly);

    QDataStream outStream(&file);
    outStream.writeRawData(byteArray.data(), byteArray.size());

    file.close();

//     QDir dir(mDataDir);
//     QVERIFY(dir.mkdir("Europe"));
//     QFile::copy(QString::fromLatin1(KDESRCDIR) + QStringLiteral("/Paris"), mDataDir + QStringLiteral("/Europe/Paris"));
}

void ByteArrayRawFileSynchronizerTest::cleanupTestCase()
{
    delete mFileSystem;
}

#if 0
void ByteArrayRawFileSynchronizerTest::init()
{
    ByteArrayModel = createByteArrayModel();

    mModifiedSpy =  new QSignalSpy(ByteArrayModel, SIGNAL(modified(bool)));
}
#endif

void ByteArrayRawFileSynchronizerTest::testLoadFromUrl()
{
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(TestFileName)));
    auto* synchronizer = new ByteArrayRawFileSynchronizer();
    synchronizer->startLoad(fileUrl)->exec();
    AbstractDocument* document = synchronizer->document();

    auto* byteArrayDocument = qobject_cast<ByteArrayDocument*>(document);

    QVERIFY(document != nullptr);
    QVERIFY(byteArrayDocument != nullptr);
    QVERIFY(document->synchronizer() != nullptr);
    QCOMPARE(document->synchronizer()->document(), document);
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);
    QCOMPARE(document->synchronizer()->localSyncState(), Kasten::LocalInSync);
    QCOMPARE(document->synchronizer()->remoteSyncState(), Kasten::RemoteInSync);

    QCOMPARE(document->synchronizer()->url(), fileUrl);

    delete document;
}

void ByteArrayRawFileSynchronizerTest::testLoadFromNotExistingUrl()
{
    const QUrl fileUrl = QUrl(QLatin1String(NotExistingUrl));

    auto* synchronizer = new ByteArrayRawFileSynchronizer();
    synchronizer->startLoad(fileUrl)->exec();
    AbstractDocument* document = synchronizer->document();

    QVERIFY(document == nullptr);
    delete synchronizer;
}

void ByteArrayRawFileSynchronizerTest::testNewSaveAsToUrl()
{
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QLatin1String(TestFileName)));

    ByteArrayDocument* document =
        new Kasten::ByteArrayDocument(QStringLiteral("New created for test."));
    auto* byteArray = qobject_cast<Okteta::PieceTableByteArrayModel*>(document->content());

    // fill array
    QByteArray testData(TestDataSize, TestDataChar);
    ::textureByteArray(&testData);
    byteArray->setData(testData);

    // save
    auto* synchronizer = new ByteArrayRawFileSynchronizer();
    synchronizer->startConnect(document, fileUrl, AbstractModelSynchronizer::ReplaceRemote)->exec();
    QCOMPARE(synchronizer->document(), document);

//     // load into other and...
//     ByteArrayDocument* otherDocument = new ByteArrayDocument( filePath );

//     QVERIFY( document != 0 );

//     // compare with old
//     Okteta::PieceTableByteArrayModel *otherByteArray = document->content();
//     QCOMPARE( byteArray->size(), otherByteArray->size() );
//     QVERIFY( qstrncmp(byteArray->data(),otherByteArray->data(),byteArray->size()) == 0 );

    delete document;
}

}

QTEST_GUILESS_MAIN(Kasten::ByteArrayRawFileSynchronizerTest)
