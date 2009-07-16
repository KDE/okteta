/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "kbytearraydocumentfactorytest.h"

// test object
#include <kbytearraydocumentfactory.h>
// lib
#include <kbytearraydocument.h>
// KDE
#include <qtest_kde.h>


namespace Kasten
{

void KByteArrayDocumentFactoryTest::testCreate()
{
    KByteArrayDocumentFactory *factory = new KByteArrayDocumentFactory();

    AbstractDocument* document = factory->create();
    KByteArrayDocument *byteArrayDocument = qobject_cast<KByteArrayDocument *>( document );

    QVERIFY( document != 0 );
    QVERIFY( byteArrayDocument != 0 );
    QCOMPARE( document->hasLocalChanges(), false );

    delete document;
    delete factory;
}

}

QTEST_KDEMAIN_CORE( Kasten::KByteArrayDocumentFactoryTest )
