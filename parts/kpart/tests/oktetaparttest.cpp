/*
    This file is part of the Okteta KPart module, made within the KDE community.

    Copyright 2016 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "oktetaparttest.h"

// test object
#include <partfactory.h>
#include <part.h>
// Qt
#include <QTest>


void OktetaPartTest::testPartReuseWithAnotherUrl()
{
    KPluginFactory *factory = new OktetaPartFactory();
    KParts::ReadOnlyPart* part = factory->create<KParts::ReadOnlyPart>(0, 0);
    QVERIFY(part != 0);

    const QUrl url1 = QUrl::fromLocalFile(QStringLiteral(TESTPATH1));
    part->openUrl(url1);
    QCOMPARE(part->url(), url1);

    const QUrl url2 = QUrl::fromLocalFile(QStringLiteral(TESTPATH2));
    part->openUrl(url2);
    QCOMPARE(part->url(), url2);

    delete part;
    delete factory;
}


QTEST_MAIN( OktetaPartTest )
