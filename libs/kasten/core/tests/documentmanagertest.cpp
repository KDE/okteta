/*
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
#include <utility>

void DocumentManagerTest::checkAdded(QSignalSpy* changedSpy, Kasten::AbstractDocument* document)
{
    QVERIFY(changedSpy->isValid());
    QCOMPARE(changedSpy->size(), 1);
    const QList<QVariant> arguments = changedSpy->takeFirst();
    QCOMPARE(arguments.size(), 1);
    const auto documents = arguments.at(0).value<QList<Kasten::AbstractDocument*>>();
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
    const auto documents = arguments.at(0).value<QList<Kasten::AbstractDocument*>>();
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
    auto documentManager = std::make_unique<Kasten::DocumentManager>();
}

void DocumentManagerTest::testAddRemove()
{
    auto doc1 = std::make_unique<Kasten::TestDocument>();
    auto* const rawDoc1 = doc1.get();
    auto doc2 = std::make_unique<Kasten::TestDocument>();
    auto* const rawDoc2 = doc2.get();
    auto doc3 = std::make_unique<Kasten::TestDocument>();
    auto* const rawDoc3 = doc3.get();

    auto documentManager = std::make_unique<Kasten::DocumentManager>();
    auto addedSpy = std::make_unique<QSignalSpy>(documentManager.get(), &Kasten::DocumentManager::added);
    auto closingSpy = std::make_unique<QSignalSpy>(documentManager.get(), &Kasten::DocumentManager::closing);

    documentManager->addDocument(std::move(doc1));
    checkAdded(addedSpy.get(), rawDoc1);

    documentManager->closeDocument(rawDoc1);
    checkAdded(closingSpy.get(), rawDoc1);

    documentManager->addDocument(std::move(doc2));
    checkAdded(addedSpy.get(), rawDoc2);
    documentManager->addDocument(std::move(doc3));
    checkAdded(addedSpy.get(), rawDoc3);

    documentManager->closeDocument(rawDoc3);
    checkAdded(closingSpy.get(), rawDoc3);
    documentManager->closeDocument(rawDoc2);
    checkAdded(closingSpy.get(), rawDoc2);
}

void DocumentManagerTest::testCanClose()
{
    auto doc = std::make_unique<Kasten::TestDocument>();
    auto* const rawDoc = doc.get();

    auto documentManager = std::make_unique<Kasten::DocumentManager>();
    documentManager->addDocument(std::move(doc));
    QVERIFY(documentManager->canClose(rawDoc));

//     rawDoc->setSyncStates( Kasten::LocalHasChanges );
//     QVERIFY( !documentManager->canClose(rawDoc) );
}

QTEST_GUILESS_MAIN(DocumentManagerTest)

#include "moc_documentmanagertest.cpp"
