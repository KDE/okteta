/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "documentmanagertest.hpp"

// sut
#include <documentmanager.hpp>
// test
#include <testdocument.hpp>
// Qt
#include <QTest>
#include <QSignalSpy>

Q_DECLARE_METATYPE(QVector<Kasten::AbstractDocument*>)

void DocumentManagerTest::checkAdded(QSignalSpy* changedSpy, Kasten::AbstractDocument* document)
{
    QVERIFY(changedSpy->isValid());
    QCOMPARE(changedSpy->count(), 1);
    const QList<QVariant> arguments = changedSpy->takeFirst();
    QCOMPARE(arguments.count(), 1);
    const QVector<Kasten::AbstractDocument*> documents =
        arguments.at(0).value<QVector<Kasten::AbstractDocument*>>();
    QCOMPARE(documents.count(), 1);
    QCOMPARE(documents.at(0), document);
}

void DocumentManagerTest::checkRemoving(QSignalSpy* changedSpy, Kasten::AbstractDocument* document)
{
    QVERIFY(changedSpy->isValid());
    QCOMPARE(changedSpy->count(), 1);
    const QList<QVariant> arguments = changedSpy->takeFirst();
    QCOMPARE(arguments.count(), 1);
    const QVector<Kasten::AbstractDocument*> documents =
        arguments.at(0).value<QVector<Kasten::AbstractDocument*>>();
    QCOMPARE(documents.count(), 1);
    QCOMPARE(documents.at(0), document);
}

void DocumentManagerTest::initTestCase()
{
    qRegisterMetaType<QVector<Kasten::AbstractDocument*>>("QVector<Kasten::AbstractDocument*>");
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
    auto* addedSpy = new QSignalSpy(documentManager, SIGNAL(added(QVector<Kasten::AbstractDocument*>)));
    auto* closingSpy = new QSignalSpy(documentManager, SIGNAL(closing(QVector<Kasten::AbstractDocument*>)));

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
