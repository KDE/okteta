/*
    This file is part of the Kasten Framework, part of the KDE project.

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

#include "kdocumentmanagertest.h"

// sut
#include <kdocumentmanager.h>
// test
#include <testdocument.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>


Q_DECLARE_METATYPE ( Kasten::KAbstractDocument * )

void KDocumentManagerTest::checkAdded( QSignalSpy *changedSpy, Kasten::KAbstractDocument *document )
{
   QVERIFY( changedSpy->isValid() );
   QCOMPARE( changedSpy->count(), 1 );
   QList<QVariant> arguments = changedSpy->takeFirst();
   QCOMPARE( arguments.at(0).value<Kasten::KAbstractDocument *>(), document );
}

void KDocumentManagerTest::checkRemoving( QSignalSpy *changedSpy, Kasten::KAbstractDocument *document )
{
   QVERIFY( changedSpy->isValid() );
   QCOMPARE( changedSpy->count(), 1 );
   QList<QVariant> arguments = changedSpy->takeFirst();
   QCOMPARE( arguments.at(0).value<Kasten::KAbstractDocument *>(), document );
}


void KDocumentManagerTest::testConstructor()
{
    Kasten::KDocumentManager *documentManager = new Kasten::KDocumentManager();
    delete documentManager;
}

void KDocumentManagerTest::testAddRemove()
{
    qRegisterMetaType<Kasten::KAbstractDocument *>("Kasten::KAbstractDocument *");

    Kasten::TestDocument *doc1 = new Kasten::TestDocument();
    Kasten::TestDocument *doc2 = new Kasten::TestDocument();
    Kasten::TestDocument *doc3 = new Kasten::TestDocument();

    Kasten::KDocumentManager *documentManager = new Kasten::KDocumentManager();
    QSignalSpy *addedSpy = new QSignalSpy( documentManager, SIGNAL(added( Kasten::KAbstractDocument * )) );
    QSignalSpy *closingSpy = new QSignalSpy( documentManager, SIGNAL(closing( Kasten::KAbstractDocument * )) );

    documentManager->addDocument( doc1 );
    checkAdded( addedSpy, doc1 );

    documentManager->closeDocument( doc1 );
    checkAdded( closingSpy, doc1 );

    documentManager->addDocument( doc2 );
    checkAdded( addedSpy, doc2 );
    documentManager->addDocument( doc3 );
    checkAdded( addedSpy, doc3 );

    documentManager->closeDocument( doc3 );
    checkAdded( closingSpy, doc3 );
    documentManager->closeDocument( doc2 );
    checkAdded( closingSpy, doc2 );

    delete documentManager;
    delete addedSpy;
    delete closingSpy;
}

void KDocumentManagerTest::testCanClose()
{
    Kasten::TestDocument *doc = new Kasten::TestDocument();

    Kasten::KDocumentManager *documentManager = new Kasten::KDocumentManager();
    documentManager->addDocument( doc );
    QVERIFY( documentManager->canClose(doc) );

//     doc->setSynchronizationStates( Kasten::KAbstractDocument::LocalHasChanges );
//     QVERIFY( !documentManager->canClose(doc) );

    delete documentManager;
}

QTEST_MAIN( KDocumentManagerTest )
