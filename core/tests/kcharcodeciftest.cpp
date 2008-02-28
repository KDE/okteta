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

#include "kcharcodeciftest.h"

// test object
#include <kcharcodec.h>
// lib
#include <khechar.h>
// Qt
#include <QtTest/QtTest>

using namespace KHECore;

static const char QTextCodecWhiteSpace = 63;


void KCharCodecIfTest::init()
{
  CharCodec = createCodec();
}

void KCharCodecIfTest::cleanup()
{
  deleteCodec( CharCodec );
}


void KCharCodecIfTest::testEncodeDecode()
{
  unsigned char c = 0;
  do
  {
    ++c;
    KChar UTF8 = CharCodec->decode( c );
    char r = 0;
    bool Success = CharCodec->encode( &r, UTF8 );
    QCOMPARE( (unsigned char)r, c );
    QVERIFY( Success );
  }
  while( c < 255 );
}
