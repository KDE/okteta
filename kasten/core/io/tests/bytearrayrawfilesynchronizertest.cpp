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
static constexpr int TestDataSize = 50;
static constexpr char TestDataChar = 0;

void ByteArrayRawFileSynchronizerTest::initTestCase()
{
    QByteArray byteArray(TestDataSize, TestDataChar);
    ::textureByteArray(&byteArray);

    mFileSystem = std::make_unique<TestFileSystem>(QLatin1String(TestDirectory));
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
    mFileSystem.reset();
}

#if 0
void ByteArrayRawFileSynchronizerTest::init()
{
    ByteArrayModel = createByteArrayModel();

    mModifiedSpy =  new QSignalSpy(ByteArrayModel, SIGNAL(modified(bool)));
}
#endif

}

QTEST_GUILESS_MAIN(Kasten::ByteArrayRawFileSynchronizerTest)

#include "moc_bytearrayrawfilesynchronizertest.cpp"
