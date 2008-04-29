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

#include "coordrangelist.h"


namespace KHEUI {

CoordRangeList::CoordRangeList()
{
}


CoordRangeList::~CoordRangeList()
{
}


void CoordRangeList::addCoordRange( CoordRange NewCoordRange )
{
  if( !NewCoordRange.isValid() )
    return;
  // we try to insert it by ascending indizes
  // if sections are overlapping we combine them
  iterator S = begin();
  for( ; S!=end(); ++S )
  {
 // TODO: add bufferwidth to rangelist so consecutive ranges can be joined, cmp ksectionlist
    // is next CoordRange behind the new CoordRange?
    if( NewCoordRange.endsBefore(*S) )
    {
      // put the new before it
      insert( S, NewCoordRange );
      return;
    }

    // does the next CoordRange overlap?
    if( (*S).overlaps(NewCoordRange) )
    {
      // Start of the combined sections is the smaller one
      NewCoordRange.extendStartTo( (*S).start() );
      // next we search all the overlapping sections and keep the highest end index
      Coord End((*S).end());
      iterator LS = S;
      for( ++LS; LS!=end(); ++LS )
      {
        if( NewCoordRange.endsBefore((*LS).start()) )
          break;
        End = (*LS).end();
      }
      // the higher end is the end of the combined CoordRange
      NewCoordRange.extendEndTo( End );
      // remove all overlapping sections
      S = erase( S, LS );
      // and instead insert the combined one
      insert( S, NewCoordRange );
      return;
    }
  }

  // all others are before the new?
  if( S == end() )
    // add it at the end
    append( NewCoordRange );
}

}
