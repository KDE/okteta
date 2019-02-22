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
