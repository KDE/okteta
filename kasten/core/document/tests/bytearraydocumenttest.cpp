/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydocumenttest.hpp"

// test object
#include <bytearraydocument.hpp>
// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// Qt
#include <QTest>
#include <QSignalSpy>
#include <QByteArray>

namespace Kasten {

static constexpr char Title[] = "title";

void ByteArrayDocumentTest::testCreateNew()
{
    ByteArrayDocument* document = new ByteArrayDocument(QStringLiteral("New created for test."));

    QVERIFY(document != nullptr);
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);

    auto* byteArray = qobject_cast<Okteta::PieceTableByteArrayModel*>(document->content());
    QVERIFY(byteArray != nullptr);
    QCOMPARE(byteArray->size(), 0);
    QVERIFY(!byteArray->isModified());

    delete document;
}

void ByteArrayDocumentTest::testSetTitle()
{
    ByteArrayDocument* document = new ByteArrayDocument(QStringLiteral("New created for test."));
    auto* titleChangeSpy =  new QSignalSpy(document, SIGNAL(titleChanged(QString)));

    const QLatin1String title(Title);
    document->setTitle(title);
    QCOMPARE(document->title(), title);
    QVERIFY(titleChangeSpy->isValid());
    QCOMPARE(titleChangeSpy->count(), 1);
    const QList<QVariant> arguments = titleChangeSpy->takeFirst();
    QCOMPARE(arguments.at(0).toString(), title);

    delete document;
    delete titleChangeSpy;
}

}

QTEST_GUILESS_MAIN(Kasten::ByteArrayDocumentTest)
