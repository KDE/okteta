/***************************************************************************
                          kcoordrangelist.cpp  -  description
                             -------------------
    begin                : Mon Jun 30 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// lib specific
#include "kcoordrangelist.h"

using namespace KHE;

KCoordRangeList::KCoordRangeList()
{
}


KCoordRangeList::~KCoordRangeList()
{
}


void KCoordRangeList::addCoordRange( KCoordRange NewCoordRange )
{
  if( !NewCoordRange.isValid() )
    return;
  // we try to insert it by ascending indizes
  // if sections are overlapping we combine them
  iterator S = begin();
  for( ; S!=end(); ++S )
  {
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
      KBufferCoord End((*S).end());
      iterator LS = S;
      for( ++LS; LS!=end(); ++LS )
      {
        if( !(*LS).overlaps(NewCoordRange) )
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
