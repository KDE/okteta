/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2005 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "ksection.h"


namespace KHE {

void KSection::adaptToReplacement( int Pos, int RemovedLength, int InsertedLength )
{
  // nothing to adapt or not affected at all??
  if( !isValid() || endsBefore(Pos-1) )
    return;

  // indirectly affected?
  if( !startsBefore(Pos+RemovedLength) )
  {
    moveBy( InsertedLength-RemovedLength );
  }
  // changes overlap, oh well
  else
  {
    // only inserted?
    if( RemovedLength==0 )
    {
      if( startsBefore(Pos) && !endsBefore(Pos-1) )
        moveEndBy( InsertedLength );
    }
    // only removed?
    else if( InsertedLength==0 )
    {
      extendStartTo(Pos);
      moveEndBy( -RemovedLength );
      extendEndTo(Pos-1);
      // equals "if( End>Pos+RemovedLength ) End -= RemovedLength; else End = Pos-1;"
    }
    else
    {
      if( startsBehind(Pos) )
        setStart( Pos+InsertedLength );
      if( endsBefore(Pos+RemovedLength-1) )
        setEnd( Pos-1 );
      else
        moveEndBy( InsertedLength-RemovedLength );
    }
  }
}

}
