/*
    This file is part of the Kasten Framework, made within the KDE community.

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

#include "testdocumenttest.h"

// test object
#include <testdocument.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

static const char TestData[] = "TestData";


void TestDocumentTest::checkTitleChanged( QSignalSpy* titleChangedSpy, const QString& title )
{
   QVERIFY( titleChangedSpy->isValid() );
   QCOMPARE( titleChangedSpy->count(), 1 );
   QList<QVariant> arguments = titleChangedSpy->takeFirst();
   QCOMPARE( arguments.at(0).toString(), title );
}

Q_DECLARE_METATYPE ( Kasten::LocalSyncState )

void TestDocumentTest::checkLocalSyncStateChanged( QSignalSpy* changedSpy, Kasten::LocalSyncState localSyncState )
{
   QVERIFY( changedSpy->isValid() );
   QCOMPARE( changedSpy->count(), 1 );
   QList<QVariant> arguments = changedSpy->takeFirst();
   QCOMPARE( arguments.at(0).value<Kasten::LocalSyncState>(), localSyncState );
}


// ------------------------------------------------------------------ tests ----

void TestDocumentTest::testPlainConstructor()
{
    Kasten::TestDocument* document = new Kasten::TestDocument();

    QVERIFY( document != 0 );
    QCOMPARE( *document->data(), QByteArray() );
    QCOMPARE( document->title(), QString() );
    QCOMPARE( document->localSyncState(), Kasten::LocalInSync );

    delete document;
}

void TestDocumentTest::testDataConstructor()
{
    const QByteArray testData( TestData );
    Kasten::TestDocument* document = new Kasten::TestDocument( testData );

    QVERIFY( document != 0 );
    QCOMPARE( *document->data(), testData );
    QCOMPARE( document->title(), QString() );
    QCOMPARE( document->localSyncState(), Kasten::LocalInSync );

    delete document;
}

void TestDocumentTest::testChangeData()
{
    qRegisterMetaType<Kasten::LocalSyncState>("Kasten::LocalSyncState");
    const QByteArray testData( TestData );

    Kasten::TestDocument* document = new Kasten::TestDocument();

    QSignalSpy* changedSpy = new QSignalSpy( document, SIGNAL(localSyncStateChanged(Kasten::LocalSyncState)) );

    QCOMPARE( *document->data(), QByteArray() );
    QCOMPARE( document->localSyncState(), Kasten::LocalInSync );

    document->setData( testData );

    const Kasten::LocalSyncState localSyncState = Kasten::LocalHasChanges;
    QCOMPARE( *document->data(), testData );
    QCOMPARE( document->localSyncState(), localSyncState );
    checkLocalSyncStateChanged( changedSpy, localSyncState );

    delete document;
    delete changedSpy;
}

void TestDocumentTest::testSetTitle()
{
    Kasten::TestDocument* document = new Kasten::TestDocument();

    QSignalSpy* titleChangedSpy = new QSignalSpy( document, SIGNAL(titleChanged(QString)) );

    const QLatin1String title( "title" );
    document->setTitle( title );

    QCOMPARE( document->title(), title );
    QCOMPARE( document->localSyncState(), Kasten::LocalInSync );
    checkTitleChanged( titleChangedSpy, title );

    delete document;
    delete titleChangedSpy;
}

void TestDocumentTest::testSetLocalSyncState()
{
    qRegisterMetaType<Kasten::LocalSyncState>("Kasten::LocalSyncState");

    Kasten::TestDocument* document = new Kasten::TestDocument();

    QSignalSpy* changedSpy = new QSignalSpy( document, SIGNAL(localSyncStateChanged( Kasten::LocalSyncState )) );

    const Kasten::LocalSyncState localSyncState = Kasten::LocalHasChanges;
    document->setLocalSyncState( localSyncState );

    QCOMPARE( document->title(), QString() );
    QCOMPARE( document->localSyncState(), localSyncState );
    checkLocalSyncStateChanged( changedSpy, localSyncState );

    delete document;
    delete changedSpy;
}

QTEST_MAIN( TestDocumentTest )
