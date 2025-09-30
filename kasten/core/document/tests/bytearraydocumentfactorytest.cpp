/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydocumentfactorytest.hpp"

// test object
#include <bytearraydocumentfactory.hpp>
// lib
#include <bytearraydocument.hpp>
// Qt
#include <QTest>
// Std
#include <memory>

namespace Kasten {

void ByteArrayDocumentFactoryTest::testCreate()
{
    auto factory = std::make_unique<ByteArrayDocumentFactory>();

    auto document = factory->create();
    auto* const byteArrayDocument = qobject_cast<ByteArrayDocument*>(document.get());

    QVERIFY(document != nullptr);
    QVERIFY(byteArrayDocument != nullptr);
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);
}

}

QTEST_GUILESS_MAIN(Kasten::ByteArrayDocumentFactoryTest)

#include "moc_bytearraydocumentfactorytest.cpp"
