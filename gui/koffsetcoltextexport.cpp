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

#include "koffsetcoltextexport.h"

// lib
#include "koffsetcolumn.h"
// Qt
#include <QtCore/QString>


namespace KHEUI {

KOffsetColTextExport::KOffsetColTextExport( const KOffsetColumn *OffsetColumn )
  : CodingWidth( OffsetColumn->codingWidth() ),
    FirstLineOffset( OffsetColumn->firstLineOffset() ),
    Delta( OffsetColumn->delta() ),
    printFunction( OffsetColumn->printFunction() )
{
}

int KOffsetColTextExport::charsPerLine() const
{
  return CodingWidth;
}


void KOffsetColTextExport::printFirstLine( QString *T, int Line ) const
{
  PrintLine = Line;
  print( T );
}

void KOffsetColTextExport::printNextLine( QString *T ) const
{
  print( T );
}

void KOffsetColTextExport::print( QString *T ) const
{
  // TODO: fix me (no more printFunction)
  char *B = new char[CodingWidth+1];
  printFunction( B, FirstLineOffset + Delta*PrintLine );
  T->append( B );
  delete [] B;

  ++PrintLine;
}

}
