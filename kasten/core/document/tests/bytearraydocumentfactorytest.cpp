/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

namespace Kasten {

void ByteArrayDocumentFactoryTest::testCreate()
{
    auto* factory = new ByteArrayDocumentFactory();

    AbstractDocument* document = factory->create();
    auto* byteArrayDocument = qobject_cast<ByteArrayDocument*>(document);

    QVERIFY(document != nullptr);
    QVERIFY(byteArrayDocument != nullptr);
    QCOMPARE(document->contentFlags(), Kasten::ContentStateNormal);

    delete document;
    delete factory;
}

}

QTEST_GUILESS_MAIN(Kasten::ByteArrayDocumentFactoryTest)
