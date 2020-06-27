/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumenttest.hpp"

// test object
#include <testdocument.hpp>
// Qt
#include <QTest>
#include <QSignalSpy>

static constexpr char TestData[] = "TestData";

void TestDocumentTest::checkTitleChanged(QSignalSpy* titleChangedSpy, const QString& title)
{
    QVERIFY(titleChangedSpy->isValid());
    QCOMPARE(titleChangedSpy->count(), 1);
    QList<QVariant> arguments = titleChangedSpy->takeFirst();
    QCOMPARE(arguments.at(0).toString(), title);
}

Q_DECLARE_METATYPE(Kasten::ContentFlags)

void TestDocumentTest::checkContentFlagsChanged(QSignalSpy* changedSpy, Kasten::ContentFlags contentFlags)
{
    QVERIFY(changedSpy->isValid());
    QCOMPARE(changedSpy->count(), 1);
    QList<QVariant> arguments = changedSpy->takeFirst();
    QCOMPARE(arguments.at(0).value<Kasten::ContentFlags>(), contentFlags);
}

//------------------------------------------------------------------ tests ----

void TestDocumentTest::testPlainConstructor()
{
    auto* document = new Kasten::TestDocument();

    QVERIFY(document != nullptr);
    QCOMPARE(*document->data(), QByteArray());
    QCOMPARE(document->title(), QString());
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);

    delete document;
}

void TestDocumentTest::testDataConstructor()
{
    const QByteArray testData(TestData);
    auto* document = new Kasten::TestDocument(testData);

    QVERIFY(document != nullptr);
    QCOMPARE(*document->data(), testData);
    QCOMPARE(document->title(), QString());
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);

    delete document;
}

void TestDocumentTest::testChangeData()
{
    qRegisterMetaType<Kasten::ContentFlags>("Kasten::ContentFlags");
    const QByteArray testData(TestData);

    auto* document = new Kasten::TestDocument();

    auto* changedSpy = new QSignalSpy(document, SIGNAL(contentFlagsChanged(Kasten::ContentFlags)));

    QCOMPARE(*document->data(), QByteArray());
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);

    document->setData(testData);

    const Kasten::ContentFlags contentFlags = Kasten::ContentHasUnstoredChanges;
    QCOMPARE(*document->data(), testData);
    QCOMPARE(document->contentFlags(), contentFlags);
    checkContentFlagsChanged(changedSpy, contentFlags);

    delete document;
    delete changedSpy;
}

void TestDocumentTest::testSetTitle()
{
    auto* document = new Kasten::TestDocument();

    auto* titleChangedSpy = new QSignalSpy(document, SIGNAL(titleChanged(QString)));

    const QLatin1String title("title");   // TODO QStringLiteral
    document->setTitle(title);

    QCOMPARE(document->title(), title);
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);
    checkTitleChanged(titleChangedSpy, title);

    delete document;
    delete titleChangedSpy;
}

void TestDocumentTest::testSetContentFlags()
{
    qRegisterMetaType<Kasten::ContentFlags>("Kasten::ContentFlags");

    auto* document = new Kasten::TestDocument();

    auto* changedSpy = new QSignalSpy(document, SIGNAL(contentFlagsChanged(Kasten::ContentFlags)));

    const Kasten::ContentFlags contentFlags = Kasten::ContentHasUnstoredChanges;
    document->setContentFlags(contentFlags);

    QCOMPARE(document->title(), QString());
    QCOMPARE(document->contentFlags(), contentFlags);
    checkContentFlagsChanged(changedSpy, contentFlags);

    delete document;
    delete changedSpy;
}

QTEST_GUILESS_MAIN(TestDocumentTest)
