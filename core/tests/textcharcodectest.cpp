/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2006,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "textcharcodectest.h"

// test object
#include <codecs/textcharcodec.h>
// KDE
#include <qtest_kde.h>


namespace Okteta
{

//---------------------------------------------------------------------------- Tests -----

void TextCharCodecTest::testCreateCodec_data()
{
    QTest::addColumn<QString>("codecName");

    foreach( const QString& codecName, TextCharCodec::codecNames() )
        QTest::newRow(codecName.toLatin1().constData()) << codecName;
}

void TextCharCodecTest::testCreateCodec()
{
    QFETCH(QString, codecName);

    CharCodec* codec = TextCharCodec::createCodec( codecName );

    QVERIFY( codec != 0 );
    QCOMPARE( codec->name(), codecName );

    delete codec;
}

void TextCharCodecTest::testCreateLocalCodec()
{
    TextCharCodec* codec = TextCharCodec::createLocalCodec();
    QVERIFY( codec != 0 );
    delete codec;
}


void TextCharCodecTest::testCreateNonexistingCodec()
{
    TextCharCodec* codec = TextCharCodec::createCodec( QLatin1String("NonexistingCode") );
    QVERIFY( codec == 0 );
}

}

QTEST_KDEMAIN_CORE( Okteta::TextCharCodecTest )
