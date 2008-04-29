/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kcharcoltextexport.h"

// lib
#include "kdatalayout.h"
#include "charbytearraycolumnrenderer.h"
#include "kcharcodec.h"


namespace KHEUI {

KCharColTextExport::KCharColTextExport( const CharByteArrayColumnRenderer* TC, const char *D, const CoordRange &CR, const QString &CodecName )
 : KDataColTextExport( TC, D, CR, 1 ),
   CharCodec( KHECore::KCharCodec::createCodec(CodecName) ),
   SubstituteChar( TC->substituteChar() ),
   UndefinedChar( TC->undefinedChar() )
{
}


KCharColTextExport::~KCharColTextExport()
{
  delete CharCodec;
}


void KCharColTextExport::print( QString *T ) const
{
  int p = 0;
  int pEnd = NoOfBytesPerLine;
  // correct boundaries
  if( PrintLine == mCoordRange.start().line() )
    p = mCoordRange.start().pos();
  if( PrintLine == mCoordRange.end().line() )
    pEnd = mCoordRange.end().pos()+1;

  // draw individual chars
  uint e = 0;
  for( ; p<pEnd; ++p, ++PrintData )
  {
    // get next position
    uint t = Pos[p];
    // clear spacing
    T->append( whiteSpace(t-e) );

    // print char
    KHECore::KChar B = CharCodec->decode( *PrintData );

    T->append( B.isUndefined() ? KHECore::KChar(UndefinedChar) : !B.isPrint() ? KHECore::KChar(SubstituteChar) : B );
    e = t + 1;
  }

  T->append( whiteSpace(NoOfCharsPerLine-e) );

  ++PrintLine;
}

}
