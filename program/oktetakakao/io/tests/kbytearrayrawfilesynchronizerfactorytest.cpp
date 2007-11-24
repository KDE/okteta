/***************************************************************************
                          kbytearrayrawfilesynchronizerfactorytest.cpp  -  description
                            -------------------
    begin                : Mon Nov 19 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/
/*
    Part creating temporary data file is
    Copyright 2005,2006 David Jarvie <software@astrojar.org.uk>
*/
/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#include "kbytearrayrawfilesynchronizerfactorytest.h"

// test object
#include <kbytearrayrawfilesynchronizerfactory.h>
// lib
#include <kbytearraydocument.h>
// test utils
#include <util/filesystem.h>
#include <util/fill.h>
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KUrl>
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


void KByteArrayRawFileSynchronizerFactoryTest::initTestCase()
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

void KByteArrayRawFileSynchronizerFactoryTest::cleanupTestCase()
{
    delete mFileSystem;
}

#if 0
void KByteArrayRawFileSynchronizerFactoryTest::init()
{
    ByteArrayModel = createByteArrayModel();

    mModifiedSpy =  new QSignalSpy( ByteArrayModel, SIGNAL(modified(bool)) );
}
#endif

void KByteArrayRawFileSynchronizerFactoryTest::testCreate()
{
    KByteArrayRawFileSynchronizerFactory *factory = new KByteArrayRawFileSynchronizerFactory();

    QVERIFY( factory != 0 );

    delete factory;
}

void KByteArrayRawFileSynchronizerFactoryTest::testLoadFromUrl()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(TestFileName) ).prepend( FileProtocolName );
    KByteArrayRawFileSynchronizerFactory *factory = new KByteArrayRawFileSynchronizerFactory();
    KAbstractDocument *document = factory->loadNewDocument( fileUrl );

    KByteArrayDocument *byteArrayDocument = qobject_cast<KByteArrayDocument *>( document );

    QVERIFY( document != 0 );
    QVERIFY( byteArrayDocument != 0 );
    QVERIFY( document->synchronizer() != 0 );
    QCOMPARE( document->synchronizer()->document(), document );
    QCOMPARE( document->hasLocalChanges(), false );

    QCOMPARE( document->synchronizer()->url(), fileUrl );

    delete document;
    delete factory;
}

void KByteArrayRawFileSynchronizerFactoryTest::testLoadFromNotExistingUrl()
{
    const KUrl fileUrl = mFileSystem->createFilePath( QLatin1String(NotExistingUrl) );

    KByteArrayRawFileSynchronizerFactory *factory = new KByteArrayRawFileSynchronizerFactory();
    KAbstractDocument *document = factory->loadNewDocument( fileUrl );

    QVERIFY( document == 0 );

    delete factory;
}

#if 0
void KByteArrayRawFileSynchronizerFactoryTest::testSaveToFile()
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

QTEST_KDEMAIN_CORE( KByteArrayRawFileSynchronizerFactoryTest )
