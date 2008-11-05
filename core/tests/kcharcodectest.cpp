/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kcharcodectest.h"

// test object
#include <charcodec.h>
// lib
#include <khechar.h>
// KDE
#include <kcomponentdata.h>
// Qt
#include <QtTest/QtTest>

// namespaces
using namespace KHECore;


void KCharCodecTest::initTestCase()
{
  Instance = new KComponentData( "KTextCharCodecTest" );
}

void KCharCodecTest::cleanupTestCase()
{
  delete Instance;
}

//---------------------------------------------------------------------------- Tests -----


void KCharCodecTest::testCodecNames()
{
  QStringList List( CharCodec::codecNames() );

  for( int i=0; i<List.size(); ++i )
  {
    // name exists
    const QString &Name = List[i];
    CharCodec* Codec = CharCodec::createCodec( Name );
    QCOMPARE( Codec->name(), Name );
    delete Codec;
  }
}


QTEST_MAIN( KCharCodecTest )
