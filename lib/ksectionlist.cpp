/***************************************************************************
                          ksectionlist.cpp  -  description
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
#include "ksectionlist.h"

using namespace KHE;

KSectionList::KSectionList()
{
}


KSectionList::~KSectionList()
{
}


void KSectionList::addSection( KSection NewSection )
{
  if( !NewSection.isValid() )
    return;

  // we try to insert it by ascending indizes
  // if sections are overlapping we combine them
  iterator S = begin();
  for( ; S!=end(); ++S )
  {
    // is new section before the next section?
    if( NewSection.endsBefore(*S) )
    {
      // put the new before it
      insert( S, NewSection );
      return;
    }

    // does the next section overlap?
    if( (*S).overlaps(NewSection) )
    {
      // Start of the combined sections is the smaller one
      NewSection.extendStartTo( (*S).start() );
      // next we search all the overlapping sections and keep the highest end index
      int End = (*S).end();
      iterator LS = S;
      for( ++LS; LS!=end(); ++LS )
      {
        if( !(*LS).overlaps(NewSection) )
          break;
        End = (*LS).end();
      }
      // the higher end is the end of the combined section
      NewSection.extendEndTo( End );
      // remove all overlapping sections
      S = erase( S, LS );
      // and instead insert the combined one
      insert( S, NewSection );
      return;
    }
  }

  // all others are before the new?
  if( S == end() )
    // add it at the end
    append( NewSection );
}
