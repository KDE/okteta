/***************************************************************************
/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearrayrawfilesynchronizertest.h"

// test object
#include <kbytearrayrawfilesynchronizer.h>
// lib
#include <kbytearraydocument.h>
// test utils
#include <util/filesystem.h>
#include <util/fill.h>
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KUrl>
#include <KComponentData>
#include <qtest_kde.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QDataStream>


static const char TestDirectory[] = "kbytearrayrawfilesynchronizertest";
static const char TestFileName[] = "test.data";
static const char NotExistingUrl[] = "notexisting://";
static const char FileProtocolName[] = "file://";
static const int TestDataSize = 50;
static const char TestDataChar = 0;


void KByteArrayRawFileSynchronizerTest::initTestCase()
{
    QByteArray byteArray( TestDataSize, TestDataChar );
    ::textureByteArray( &byteArray );

    mFileSystem = new TestFileSystem( QLatin1String(TestDirectory) );
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName) );
    QFile file;
    file.setFileName( filePath );
    file.open( QIODevice::WriteOnly );

    QDataStream outStream( &file );
    outStream.writeRawData( byteArray.data(), byteArray.size() );

    file.close();

//     QDir dir(mDataDir);
//     QVERIFY(dir.mkdir("Europe"));
//     QFile::copy(QString::fromLatin1(KDESRCDIR) + QLatin1String("/Paris"), mDataDir + QLatin1String("/Europe/Paris"));
}

void KByteArrayRawFileSynchronizerTest::cleanupTestCase()
{
    delete mFileSystem;
}

#if 0
void KByteArrayRawFileSynchronizerTest::init()
{
    ByteArrayModel = createByteArrayModel();

    mModifiedSpy =  new QSignalSpy( ByteArrayModel, SIGNAL(modified(bool)) );
}
#endif


void KByteArrayRawFileSynchronizerTest::testLoadFromUrl()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName) ).prepend( FileProtocolName );
    KByteArrayRawFileSynchronizer *synchronizer = new KByteArrayRawFileSynchronizer( fileUrl );
    KAbstractDocument *document = synchronizer->document();

    KByteArrayDocument *byteArrayDocument = qobject_cast<KByteArrayDocument *>( document );

    QVERIFY( document != 0 );
    QVERIFY( byteArrayDocument != 0 );
    QVERIFY( document->synchronizer() != 0 );
    QCOMPARE( document->synchronizer()->document(), document );
    QCOMPARE( document->hasLocalChanges(), false );

    QCOMPARE( document->synchronizer()->url(), fileUrl );

    delete document;
}

void KByteArrayRawFileSynchronizerTest::testLoadFromNotExistingUrl()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(NotExistingUrl) );

    KByteArrayRawFileSynchronizer *synchronizer = new KByteArrayRawFileSynchronizer( fileUrl );
    KAbstractDocument *document = synchronizer->document();

    QVERIFY( document == 0 );
    delete synchronizer;
}

#if 0
void KByteArrayRawFileSynchronizerTest::testSaveToFile()
{
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName) );


    KByteArrayDocument *document = new KByteArrayDocument();
    KHECore::KByteArrayModel *byteArray = document->content();

    // fill array
    byteArray->

    // save
    document->setLocalFilePath( filePath );
    document->save();
TODO: save mit path als Parameter? Oder separat setzen? Wie Kopie speichern?

    // load into other and...
    KByteArrayDocument *otherDocument = new KByteArrayDocument( filePath );

    QVERIFY( document != 0 );

    // compare with old
    KHECore::KByteArrayModel *otherByteArray = document->content();
    QCOMPARE( byteArray->size(), otherByteArray->size() );
    QVERIFY( qstrncmp(byteArray->data(),otherByteArray->data(),byteArray->size()) == 0 );

    delete document;
}
#endif

QTEST_KDEMAIN_CORE( KByteArrayRawFileSynchronizerTest )
