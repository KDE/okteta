/***************************************************************************
                          ksection.cpp  -  description
                             -------------------
    begin                : Sun Nov 20 2005
    copyright            : (C) 2005 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// lib specific
#include "ksection.h"

namespace KHE {

void KSection::adaptToChange( int Pos, int RemovedLength, int InsertedLength )
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
