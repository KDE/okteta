/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
// Qt
#include <QTest>
#include <QSignalSpy>
#include <QByteArray>
#include <QFile>
#include <QDataStream>

static constexpr char TestDirectory[] = "bytearrayrawfilesynchronizertest";
static constexpr char TestFileName[] = "test.data";
#if 0
static constexpr char NotExistingUrl[] = "notexisting://";
#endif
static constexpr int TestDataSize = 50;
static constexpr char TestDataChar = 0;

void ByteArrayRawFileSynchronizerFactoryTest::initTestCase()
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

void ByteArrayRawFileSynchronizerFactoryTest::cleanupTestCase()
{
    delete mFileSystem;
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
    auto* factory = new Kasten::ByteArrayRawFileSynchronizerFactory();

    QVERIFY(factory != nullptr);

    delete factory;
}
#if 0
void ByteArrayRawFileSynchronizerFactoryTest::testLoadFromUrl()
{
    const QUrl fileUrl = QUrl::fromLocalFile(mFileSystem->createFilePath(QStringLiteral(TestFileName)));
    Kasten::ByteArrayRawFileSynchronizerFactory* factory = new Kasten::ByteArrayRawFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument(fileUrl);

    ByteArrayDocument* byteArrayDocument = qobject_cast<ByteArrayDocument*>(document);

    QVERIFY(document != 0);
    QVERIFY(byteArrayDocument != 0);
    QVERIFY(document->synchronizer() != 0);
    QCOMPARE(document->synchronizer()->document(), document);
    QCOMPARE(document->hasLocalChanges(), false);

    QCOMPARE(document->synchronizer()->url(), fileUrl);

    delete document;
    delete factory;
}

void ByteArrayRawFileSynchronizerFactoryTest::testLoadFromNotExistingUrl()
{
    const QUrl fileUrl = QUrl(QStringLiteral(NotExistingUrl));

    Kasten::ByteArrayRawFileSynchronizerFactory* factory = new Kasten::ByteArrayRawFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument(fileUrl);

    QVERIFY(document == 0);

    delete factory;
}
#endif
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
TODO: save mit path als Parameter ? Oder separat setzen ? Wie Kopie speichern ?

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

QTEST_GUILESS_MAIN(ByteArrayRawFileSynchronizerFactoryTest)
