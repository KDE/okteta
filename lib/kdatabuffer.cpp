/***************************************************************************
                          kdatabuffer.cpp  -  description
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
#include "kdatabuffer.h"

using namespace KHE;


int KDataBuffer::insert( int Pos, const char* D, int Length )
{
  return replace( Pos,0,D,Length );
}


int KDataBuffer::remove( KSection Remove )
{
  replace( Remove, 0, 0 );
  return Remove.width(); // TODO: check if this is true
}

int KDataBuffer::copyTo( char* Dest, int Pos, int Length ) const
{
  int LastPos = Pos + Length - 1;
  for( int i=Pos; i<=LastPos; ++i )
    *Dest++ = datum( i );
  return Length;
}


//static const char[] Separators = "\t.,:;-<>[](){}";

// TODO: would be even faster with function pointers
static bool isWordChar( const char Char, KDataBuffer::KWordCharType CharType )
{
  switch( CharType )
  {
    case KDataBuffer::Printable:  return isprint( Char );
    case KDataBuffer::Readable:   return isalnum( Char );
//  return isspace(Char) || strchr(Separators,Char)!=0;
    default:                        return true;
  }

  return false;
}


bool KDataBuffer::isWordChar( int Index, KDataBuffer::KWordCharType CharType ) const
{
  return ::isWordChar( datum(Index), CharType );
}


int KDataBuffer::indexOfPreviousWordStart( int Index, KDataBuffer::KWordCharType CharType ) const
{
  if( Index > 0 )
    --Index;

  // if after goto previous letter we are still at the end
  if( Index == size()-1 )
    return 0;

  bool LookingForFirstWordChar = false;
  for( ; Index >= 0; --Index )
  {
    if( !::isWordChar(datum( Index ),CharType) )
    {
      if( !LookingForFirstWordChar )
        continue;
      return( Index + 1 );
    }
    else if( !LookingForFirstWordChar )
      LookingForFirstWordChar = true;
  }
  return 0;
}


int KDataBuffer::indexOfNextWordStart( int Index, KDataBuffer::KWordCharType CharType ) const
{
  bool LookingForFirstWordChar = false;
  for( ; Index<size(); ++Index )
  {
    if( ::isWordChar(datum(Index),CharType) )
    {
      if( !LookingForFirstWordChar )
        continue;
      return Index;
    }
    else if( !LookingForFirstWordChar )
      LookingForFirstWordChar = true;
  }
  // if no more word found, go to the end
  return size()-1;
}


int KDataBuffer::indexOfBeforeNextWordStart( int Index, KDataBuffer::KWordCharType CharType ) const
{
  bool LookingForFirstWordChar = false;
  for( ; Index<size(); ++Index )
  {
    if( ::isWordChar(datum(Index),CharType) )
    {
      if( !LookingForFirstWordChar )
        continue;
      return Index-1;
    }
    else if( !LookingForFirstWordChar )
      LookingForFirstWordChar = true;
  }
  // if no more word found, go to the end
  return size()-1;
}


int KDataBuffer::indexOfWordStart( int Index, KDataBuffer::KWordCharType CharType ) const
{
  for( --Index; Index >= 0; --Index )
  {
    if( !::isWordChar(datum( Index ),CharType) )
      return( Index + 1 );
  }
  return 0;
}


int KDataBuffer::indexOfWordEnd( int Index, KDataBuffer::KWordCharType CharType ) const
{
  for( ++Index; Index<size(); ++Index )
  {
    if( !::isWordChar(datum(Index),CharType) )
      return Index-1;
  }
  // if no more word found, go to the end
  return size()-1;
}


int KDataBuffer::indexOfBehindWordEnd( int Index, KDataBuffer::KWordCharType CharType ) const
{
  for( ; Index<size(); ++Index )
  {
    if( !::isWordChar(datum(Index),CharType) )
      return Index;
  }
  // if no more word found, go to the end
  return size();
}


KSection KDataBuffer::wordSection( int Index, KWordCharType CharType ) const
{
  return KSection( indexOfWordStart(Index,CharType), indexOfWordEnd(Index,CharType) );
}
