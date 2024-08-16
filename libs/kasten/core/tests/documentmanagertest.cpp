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
// Std
#include <memory>

void DocumentManagerTest::checkAdded(QSignalSpy* changedSpy, Kasten::AbstractDocument* document)
{
    QVERIFY(changedSpy->isValid());
    QCOMPARE(changedSpy->size(), 1);
    const QList<QVariant> arguments = changedSpy->takeFirst();
    QCOMPARE(arguments.size(), 1);
    const QVector<Kasten::AbstractDocument*> documents =
        arguments.at(0).value<QVector<Kasten::AbstractDocument*>>();
    QCOMPARE(documents.size(), 1);
    QCOMPARE(documents.at(0), document);
}

void DocumentManagerTest::checkRemoving(QSignalSpy* changedSpy, Kasten::AbstractDocument* document)
{
    QVERIFY(changedSpy->isValid());
    QCOMPARE(changedSpy->size(), 1);
    const QList<QVariant> arguments = changedSpy->takeFirst();
    QCOMPARE(arguments.size(), 1);
    const QVector<Kasten::AbstractDocument*> documents =
        arguments.at(0).value<QVector<Kasten::AbstractDocument*>>();
    QCOMPARE(documents.size(), 1);
    QCOMPARE(documents.at(0), document);
}

void DocumentManagerTest::initTestCase()
{
    qRegisterMetaType<QVector<Kasten::AbstractDocument*>>("QVector<Kasten::AbstractDocument*>");
}

void DocumentManagerTest::testConstructor()
{
    auto documentManager = std::make_unique<Kasten::DocumentManager>();
}

void DocumentManagerTest::testAddRemove()
{
    auto* doc1 = new Kasten::TestDocument();
    auto* doc2 = new Kasten::TestDocument();
    auto* doc3 = new Kasten::TestDocument();

    auto documentManager = std::make_unique<Kasten::DocumentManager>();
    auto addedSpy = std::make_unique<QSignalSpy>(documentManager.get(), SIGNAL(added(QVector<Kasten::AbstractDocument*>)));
    auto closingSpy = std::make_unique<QSignalSpy>(documentManager.get(), SIGNAL(closing(QVector<Kasten::AbstractDocument*>)));

    documentManager->addDocument(doc1);
    checkAdded(addedSpy.get(), doc1);

    documentManager->closeDocument(doc1);
    checkAdded(closingSpy.get(), doc1);

    documentManager->addDocument(doc2);
    checkAdded(addedSpy.get(), doc2);
    documentManager->addDocument(doc3);
    checkAdded(addedSpy.get(), doc3);

    documentManager->closeDocument(doc3);
    checkAdded(closingSpy.get(), doc3);
    documentManager->closeDocument(doc2);
    checkAdded(closingSpy.get(), doc2);
}

void DocumentManagerTest::testCanClose()
{
    auto* doc = new Kasten::TestDocument();

    auto documentManager = std::make_unique<Kasten::DocumentManager>();
    documentManager->addDocument(doc);
    QVERIFY(documentManager->canClose(doc));

//     doc->setSyncStates( Kasten::LocalHasChanges );
//     QVERIFY( !documentManager->canClose(doc) );
}

QTEST_GUILESS_MAIN(DocumentManagerTest)

#include "moc_documentmanagertest.cpp"
