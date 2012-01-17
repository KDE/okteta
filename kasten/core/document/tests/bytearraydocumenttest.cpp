/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "bytearraydocumenttest.h"

// test object
#include <bytearraydocument.h>
// Okteta core
#include <piecetablebytearraymodel.h>
// Qt
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QByteArray>


namespace Kasten2
{

static const char Title[] = "title";


void ByteArrayDocumentTest::testCreateNew()
{
    ByteArrayDocument* document = new ByteArrayDocument(QLatin1String("New created for test."));

    QVERIFY( document != 0 );
    QCOMPARE( document->contentFlags(), Kasten2::ContentStateNormal );

    Okteta::PieceTableByteArrayModel *byteArray = qobject_cast<Okteta::PieceTableByteArrayModel*>( document->content() );
    QVERIFY( byteArray != 0 );
    QCOMPARE( byteArray->size(), 0 );
    QVERIFY( !byteArray->isModified() );

    delete document;
}

void ByteArrayDocumentTest::testSetTitle()
{
    ByteArrayDocument* document = new ByteArrayDocument(QLatin1String("New created for test."));
    QSignalSpy* titleChangeSpy =  new QSignalSpy( document, SIGNAL(titleChanged(QString)) );

    const QLatin1String title( Title );
    document->setTitle( title );
    QCOMPARE( document->title(), title );
    QVERIFY( titleChangeSpy->isValid() );
    QCOMPARE( titleChangeSpy->count(), 1 );
    const QList<QVariant> arguments = titleChangeSpy->takeFirst();
    QCOMPARE( arguments.at(0).toString(), title );

    delete document;
    delete titleChangeSpy;
}

}

QTEST_MAIN( Kasten2::ByteArrayDocumentTest )
