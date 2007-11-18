/***************************************************************************
                          kbytearraydocumentfactorytest.cpp  -  description
                            -------------------
    begin                : Fri Nov 16 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#include "kbytearraydocumentfactorytest.h"

// test object
#include <kbytearraydocumentfactory.h>
// lib
#include <kbytearraydocument.h>
// Qt
#include <QtTest/QtTest>


void KByteArrayDocumentFactoryTest::testCreate()
{
    KByteArrayDocumentFactory *factory = new KByteArrayDocumentFactory();

    KAbstractDocument *document = factory->create();
    KByteArrayDocument *byteArrayDocument = qobject_cast<KByteArrayDocument *>( document );

    QVERIFY( document != 0 );
    QVERIFY( byteArrayDocument != 0 );
    QCOMPARE( document->hasLocalChanges(), false );

    delete document;
    delete factory;
}

QTEST_MAIN( KByteArrayDocumentFactoryTest )
