/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentmanagertest.hpp"

// sut
#include <documentmanager.hpp>
// test
#include <testdocument.hpp>
// Qt
#include <QTest>
#include <QSignalSpy>

void DocumentManagerTest::checkAdded(QSignalSpy* changedSpy, Kasten::AbstractDocument* document)
{
    QVERIFY(changedSpy->isValid());
    QCOMPARE(changedSpy->size(), 1);
    const QList<QVariant> arguments = changedSpy->takeFirst();
    QCOMPARE(arguments.size(), 1);
    const QList<Kasten::AbstractDocument*> documents =
        arguments.at(0).value<QList<Kasten::AbstractDocument*>>();
    QCOMPARE(documents.size(), 1);
    // cast to non-QObject pointer, to avoid QTest trying to access the object instance for nicer logging
    // in the case of failures, where in our case the objects could be deleted already.
    QCOMPARE(static_cast<void*>(documents.at(0)), static_cast<void*>(document));
}

void DocumentManagerTest::checkRemoving(QSignalSpy* changedSpy, Kasten::AbstractDocument* document)
{
    QVERIFY(changedSpy->isValid());
    QCOMPARE(changedSpy->size(), 1);
    const QList<QVariant> arguments = changedSpy->takeFirst();
    QCOMPARE(arguments.size(), 1);
    const QList<Kasten::AbstractDocument*> documents =
        arguments.at(0).value<QList<Kasten::AbstractDocument*>>();
    QCOMPARE(documents.size(), 1);
    // cast to non-QObject pointer, to avoid QTest trying to access the object instance for nicer logging
    // in the case of failures, where in our case the objects could be deleted already.
    QCOMPARE(static_cast<void*>(documents.at(0)), static_cast<void*>(document));
}

void DocumentManagerTest::initTestCase()
{
    qRegisterMetaType<QList<Kasten::AbstractDocument*>>("QList<Kasten::AbstractDocument*>");
}

void DocumentManagerTest::testConstructor()
{
    auto* documentManager = new Kasten::DocumentManager();
    delete documentManager;
}

void DocumentManagerTest::testAddRemove()
{
    auto* doc1 = new Kasten::TestDocument();
    auto* doc2 = new Kasten::TestDocument();
    auto* doc3 = new Kasten::TestDocument();

    auto* documentManager = new Kasten::DocumentManager();
    auto* addedSpy = new QSignalSpy(documentManager, SIGNAL(added(QList<Kasten::AbstractDocument*>)));
    auto* closingSpy = new QSignalSpy(documentManager, SIGNAL(closing(QList<Kasten::AbstractDocument*>)));

    documentManager->addDocument(doc1);
    checkAdded(addedSpy, doc1);

    documentManager->closeDocument(doc1);
    checkAdded(closingSpy, doc1);

    documentManager->addDocument(doc2);
    checkAdded(addedSpy, doc2);
    documentManager->addDocument(doc3);
    checkAdded(addedSpy, doc3);

    documentManager->closeDocument(doc3);
    checkAdded(closingSpy, doc3);
    documentManager->closeDocument(doc2);
    checkAdded(closingSpy, doc2);

    delete documentManager;
    delete addedSpy;
    delete closingSpy;
}

void DocumentManagerTest::testCanClose()
{
    auto* doc = new Kasten::TestDocument();

    auto* documentManager = new Kasten::DocumentManager();
    documentManager->addDocument(doc);
    QVERIFY(documentManager->canClose(doc));

//     doc->setSyncStates( Kasten::LocalHasChanges );
//     QVERIFY( !documentManager->canClose(doc) );

    delete documentManager;
}

QTEST_GUILESS_MAIN(DocumentManagerTest)

#include "moc_documentmanagertest.cpp"
