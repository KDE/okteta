/***************************************************************************
                          kabstractbytearraymodel.cpp  -  description
                             -------------------
    begin                : Fri Aug 01 2003
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


// c specific
#include <ctype.h>
// lib specific
#include "kabstractbytearraymodel.h"

using namespace KHE;


int KAbstractByteArrayModel::insert( int Pos, const char* D, int Length )
{
  return replace( Pos,0,D,Length );
}


int KAbstractByteArrayModel::remove( const KSection &Remove )
{
  replace( Remove, 0, 0 );
  return Remove.width(); // TODO: check if this is true
}

int KAbstractByteArrayModel::copyTo( char* Dest, int Pos, int Length ) const
{
  return copyTo( Dest, KSection::fromWidth(Pos,Length) );
}


int KAbstractByteArrayModel::copyTo( char* Dest, const KSection &S ) const
{
  KSection Source( S );
  Source.restrictEndTo( size()-1 );
  for( int i=Source.start(); i<=Source.end(); ++i )
    *Dest++ = datum( i );
  return Source.width();
}

#include "kabstractbytearraymodel.moc"
