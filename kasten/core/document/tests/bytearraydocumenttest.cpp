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
// Std
#include <memory>

namespace Kasten {

static constexpr char Title[] = "title";

void ByteArrayDocumentTest::testCreateNew()
{
    auto document = std::make_unique<ByteArrayDocument>(QStringLiteral("New created for test."));

    QVERIFY(document != nullptr);
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);

    auto* byteArray = qobject_cast<Okteta::PieceTableByteArrayModel*>(document->content());
    QVERIFY(byteArray != nullptr);
    QCOMPARE(byteArray->size(), 0);
    QVERIFY(!byteArray->isModified());
}

void ByteArrayDocumentTest::testSetTitle()
{
    auto document = std::make_unique<ByteArrayDocument>(QStringLiteral("New created for test."));
    auto titleChangeSpy =  std::make_unique<QSignalSpy>(document.get(), SIGNAL(titleChanged(QString)));

    const QLatin1String title(Title);
    document->setTitle(title);
    QCOMPARE(document->title(), title);
    QVERIFY(titleChangeSpy->isValid());
    QCOMPARE(titleChangeSpy->size(), 1);
    const QList<QVariant> arguments = titleChangeSpy->takeFirst();
    QCOMPARE(arguments.at(0).toString(), title);
}

}

QTEST_GUILESS_MAIN(Kasten::ByteArrayDocumentTest)

#include "moc_bytearraydocumenttest.cpp"
