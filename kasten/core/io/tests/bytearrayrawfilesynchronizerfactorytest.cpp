/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "bytearrayrawfilesynchronizerfactorytest.h"

// test object
#include <bytearrayrawfilesynchronizerfactory.h>
// lib
#include <bytearraydocument.h>
// test utils
#include <util/filesystem.h>
#include <util/fill.h>
// Okteta core
#include <piecetablebytearraymodel.h>
// KDE
#include <KUrl>
#include <qtest_kde.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QDataStream>


static const char TestDirectory[] = "bytearrayrawfilesynchronizertest";
static const char TestFileName[] = "test.data";
static const char NotExistingUrl[] = "notexisting://";
static const char FileProtocolName[] = "file://";
static const int TestDataSize = 50;
static const char TestDataChar = 0;


void ByteArrayRawFileSynchronizerFactoryTest::initTestCase()
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

void ByteArrayRawFileSynchronizerFactoryTest::cleanupTestCase()
{
    delete mFileSystem;
}

#if 0
void ByteArrayRawFileSynchronizerFactoryTest::init()
{
    ByteArrayModel = createByteArrayModel();

    mModifiedSpy =  new QSignalSpy( ByteArrayModel, SIGNAL(modified(bool)) );
}
#endif

void ByteArrayRawFileSynchronizerFactoryTest::testCreate()
{
    Kasten2::ByteArrayRawFileSynchronizerFactory *factory = new Kasten2::ByteArrayRawFileSynchronizerFactory();

    QVERIFY( factory != 0 );

    delete factory;
}
#if 0
void ByteArrayRawFileSynchronizerFactoryTest::testLoadFromUrl()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName) ).prepend( FileProtocolName );
    Kasten2::ByteArrayRawFileSynchronizerFactory *factory = new Kasten2::ByteArrayRawFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument( fileUrl );

    ByteArrayDocument *byteArrayDocument = qobject_cast<ByteArrayDocument *>( document );

    QVERIFY( document != 0 );
    QVERIFY( byteArrayDocument != 0 );
    QVERIFY( document->synchronizer() != 0 );
    QCOMPARE( document->synchronizer()->document(), document );
    QCOMPARE( document->hasLocalChanges(), false );

    QCOMPARE( document->synchronizer()->url(), fileUrl );

    delete document;
    delete factory;
}

void ByteArrayRawFileSynchronizerFactoryTest::testLoadFromNotExistingUrl()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(NotExistingUrl) );

    Kasten2::ByteArrayRawFileSynchronizerFactory *factory = new Kasten2::ByteArrayRawFileSynchronizerFactory();
    AbstractDocument* document = factory->loadNewDocument( fileUrl );

    QVERIFY( document == 0 );

    delete factory;
}
#endif
#if 0
void ByteArrayRawFileSynchronizerFactoryTest::testSaveToFile()
{
    const QString filePath = mFileSystem->createFilePath( QLatin1String(TestFileName) );


    ByteArrayDocument *document = new ByteArrayDocument();
    Okteta::PieceTableByteArrayModel *byteArray = document->content();

    // fill array
    byteArray->

    // save
    document->setLocalFilePath( filePath );
    document->save();
TODO: save mit path als Parameter? Oder separat setzen? Wie Kopie speichern?

    // load into other and...
    ByteArrayDocument *otherDocument = new ByteArrayDocument( filePath );

    QVERIFY( document != 0 );

    // compare with old
    Okteta::PieceTableByteArrayModel *otherByteArray = document->content();
    QCOMPARE( byteArray->size(), otherByteArray->size() );
    QVERIFY( qstrncmp(byteArray->data(),otherByteArray->data(),byteArray->size()) == 0 );

    delete document;
}
#endif

QTEST_KDEMAIN_CORE( ByteArrayRawFileSynchronizerFactoryTest )
