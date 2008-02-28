/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kcharcodec.h"

// lib
#include "ktextcharcodec.h"
#include "kebcdic1047charcodec.h"
// Qt
#include <QtCore/QStringList>


namespace KHECore {


const QStringList &KCharCodec::codecNames()
{
  static QStringList CodecNames;
  // first call?
  if( CodecNames.isEmpty() )
  {
    CodecNames = KTextCharCodec::codecNames();
    CodecNames.append( KEBCDIC1047CharCodec::codecName() );
  }

  return CodecNames;
}


KCharCodec *KCharCodec::createCodec( const QString &Name )
{
  KCharCodec *Codec = 0;

  if( KTextCharCodec::codecNames().indexOf(Name) != -1 )
    Codec = KTextCharCodec::createCodec( Name );
  else if( KEBCDIC1047CharCodec::codecName() == Name )
    Codec = KEBCDIC1047CharCodec::create();

  // ensure at least a codec
  if( Codec == 0 )
    Codec = KTextCharCodec::createLocalCodec();

  return Codec;
}


KCharCodec *KCharCodec::createCodec( KEncoding C )
{
  KCharCodec *Codec;
  if( C == EBCDIC1047Encoding )
    Codec = KEBCDIC1047CharCodec::create();
  else if( C == ISO8859_1Encoding )
    Codec = KTextCharCodec::createCodec( "ISO-8859-1" );
  // LocalEncoding
  else
    Codec = 0;

  // ensure at least a codec
  if( Codec == 0 )
    Codec = KTextCharCodec::createLocalCodec();

  return Codec;
}

}
