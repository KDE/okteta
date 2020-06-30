/*
    This file is part of the Okteta KPart module, made within the KDE community.

    SPDX-FileCopyrightText: 2016 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "oktetaparttest.hpp"

// test object
#include <partfactory.hpp>
#include <part.hpp>
// Qt
#include <QTest>

void OktetaPartTest::testPartReuseWithAnotherUrl()
{
    KPluginFactory* factory = new OktetaPartFactory();
    auto* part = factory->create<KParts::ReadOnlyPart>(nullptr, nullptr);
    QVERIFY(part != nullptr);

    const QUrl url1 = QUrl::fromLocalFile(QStringLiteral(TESTPATH1));
    part->openUrl(url1);
    QCOMPARE(part->url(), url1);

    const QUrl url2 = QUrl::fromLocalFile(QStringLiteral(TESTPATH2));
    part->openUrl(url2);
    QCOMPARE(part->url(), url2);

    delete part;
    delete factory;
}

QTEST_MAIN(OktetaPartTest)
