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


#include <iostream>

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
//   std::cout << "addCoordRange: ";
  // we try to insert it by ascending indizes
  // if sections are overlapping we combine them
  iterator S = begin();
  for( ; S!=end(); ++S )
  {
    // is next CoordRange before the new CoordRange?
    if( (*S).endsBefore(NewCoordRange) )
    {
//       std::cout << "inserting before " << (*S).start().pos()<<"/"<<(*S).start().line()<<"-"<<(*S).end().pos()<<"/"<<(*S).end().line() << std::endl;
      // put the new before it
      insert( S, NewCoordRange );
      return;
    }

    // does the next CoordRange overlap?
    if( (*S).overlaps(NewCoordRange) )
    {
//       std::cout << "overlapping with " << (*S).start().pos()<<"/"<<(*S).start().line()<<"-"<<(*S).end().pos()<<"/"<<(*S).end().line();
      // Start of the combined sections is the smaller one
      NewCoordRange.extendStartTo( (*S).start() );
      // next we search all the overlapping sections and keep the highest end index
      KBufferCoord End((*S).end());
      iterator LS = S;
      for( ++LS; LS!=end(); ++LS )
      {
        if( !(*LS).overlaps(NewCoordRange) )
          break;
//         std::cout << "overlapping with " << (*LS).start().pos()<<"/"<<(*LS).start().line()<<"-"<<(*LS).end().pos()<<"/"<<(*LS).end().line();
        End = (*LS).end();
      }
      // the higher end is the end of the combined CoordRange
      NewCoordRange.extendEndTo( End );
      // remove all overlapping sections
      S = erase( S, LS );
      // and instead insert the combined one
      insert( S, NewCoordRange );
//       std::cout << "inserting now " << NewCoordRange.start().pos()<<"/"<<NewCoordRange.start().line()
//                 <<"-"<<NewCoordRange.end().pos()<<"/"<<NewCoordRange.end().line()<< std::endl;
      return;
    }
  }

  // all others are before the new?
  if( S == end() )
    // add it at the end
    append( NewCoordRange );
 //     std::cout << "appending"<< std::endl;
}
