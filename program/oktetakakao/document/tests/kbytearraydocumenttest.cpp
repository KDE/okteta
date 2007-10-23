/***************************************************************************
                          kbytearraydocumenttest.cpp  -  description
                            -------------------
    begin                : Wed Mai 23 2007
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


#include "kbytearraydocumenttest.h"

// test object
#include <kbytearraydocument.h>
// test utils
#include "util/filesystem.h"
#include <util/fill.h>
// Okteta core
#include <kbytearraymodel.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QDataStream>


static const char testDirectory[] = "kbytearraydocumenttest";
static const char testFileName[] = "test.data";
static const int testDataSize = 50;
static const char testDataChar = 0;


void KByteArrayDocumentTest::initTestCase()
{
    QByteArray byteArray( testDataSize, testDataChar );
    ::textureByteArray( &byteArray );

    mFileSystem = new TestFileSystem( QLatin1String(testDirectory) );
    const QString filePath = mFileSystem->createFilePath( QLatin1String(testFileName) );
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

void KByteArrayDocumentTest::cleanupTestCase()
{
    delete mFileSystem;
}

#if 0
void KByteArrayDocumentTest::init()
{
    ByteArrayModel = createByteArrayModel();

    mModifiedSpy =  new QSignalSpy( ByteArrayModel, SIGNAL(modified(bool)) );
}
#endif


void KByteArrayDocumentTest::testCreateNew()
{
    KByteArrayDocument *document = new KByteArrayDocument();

    QVERIFY( document != 0 );
    QCOMPARE( document->hasLocalChanges(), false );
    QCOMPARE( document->url(), QString() );

    //TODO: check for created title "new %number"
//     QCOMPARE( document->title(), QString() );

    KHECore::KByteArrayModel *byteArray = document->content();
    QVERIFY( byteArray != 0 );
    QCOMPARE( byteArray->size(), 0 );
    QCOMPARE( byteArray->isModified(), false );

    delete document;
}

void KByteArrayDocumentTest::testCreateFromFile()
{
    const QString filePath = mFileSystem->createFilePath( QLatin1String(testFileName) );

    KByteArrayDocument *document = new KByteArrayDocument( filePath );

    QVERIFY( document != 0 );
    QCOMPARE( document->hasLocalChanges(), false );
    QCOMPARE( document->localFilePath(), filePath );

    //TODO: check for created title "new %number"
//     QCOMPARE( document->title(), QString() );

    KHECore::KByteArrayModel *byteArray = document->content();
    QVERIFY( byteArray != 0 );
    QCOMPARE( byteArray->size(), testDataSize );

    delete document;
}

void KByteArrayDocumentTest::testCreateFromNotExistingFile()
{
    KByteArrayDocument *document = new KByteArrayDocument( 0 );

    QVERIFY( document != 0 );

    delete document;
}

#if 0
void KByteArrayDocumentTest::testSaveToFile()
{
    const QString filePath = mFileSystem->createFilePath( QLatin1String(testFileName) );


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
QTEST_MAIN( KByteArrayDocumentTest )

#include "kbytearraydocumenttest.moc"
