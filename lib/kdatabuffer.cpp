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
  return copyTo( Dest, KSection(Pos,Length,false) );
}



int KDataBuffer::copyTo( char* Dest, KSection Source ) const
{
  Source.restrictEndTo( size()-1 );
  for( int i=Source.start(); i<=Source.end(); ++i )
    *Dest++ = datum( i );
  return Source.width();
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
  // already at the start or can the result only be 0?
  if( Index <= 0 || size() < 3 )
    return 0;

  // search in two rounds: first for the next char, than for the next nonchar
  // after that return the index of the one before
  bool LookingForFirstWordChar = false;
  for( --Index; Index >= 0; --Index )
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
  return size();
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
  // word reaches the end
  return size()-1;
}


int KDataBuffer::indexOfLeftWordSelect( int Index, KDataBuffer::KWordCharType CharType ) const
{
  // word at Index?
  if( ::isWordChar(datum(Index),CharType) )
  {
    // search for word start to the left
    for( --Index; Index>=0; --Index )
    {
      if( !::isWordChar(datum(Index),CharType) )
        return Index+1;
    }
    // reached start, so return it
    return 0;
  }
  else
  {
    // search for word start to the right
    for( ++Index; Index<size(); ++Index )
    {
      if( ::isWordChar(datum(Index),CharType) )
        return Index;
    }
    // word reaches the end, so step behind
    return size();
  }
}


int KDataBuffer::indexOfRightWordSelect( int Index, KDataBuffer::KWordCharType CharType ) const
{
  // no word at Index?
  if( !::isWordChar(datum(Index),CharType) )
  {
    // search for word end to the left
    for( --Index; Index>=0; --Index )
    {
      if( ::isWordChar(datum(Index),CharType) )
        return Index+1;
    }
    // reached start, so return it
    return 0;
  }
  else
  {
    for( ++Index; Index<size(); ++Index )
    {
      // search for word end to the right
      if( !::isWordChar(datum(Index),CharType) )
        return Index;
    }
    // word reaches the end, so step behind
    return size();
  }
}

/*
int KDataBuffer::indexOfBehindWordEnd( int Index, KDataBuffer::KWordCharType CharType ) const
{
  // no word at Index?
  return !::isWordChar(datum(Index),CharType) ? indexOfBehindLeftWordEnd(Index,CharType) : indexOfBehindRightWordEnd(Index+1,CharType)
}


int KDataBuffer::indexOfBehindRightWordEnd( int Index, KDataBuffer::KWordCharType CharType ) const
{
  for( ; Index<size(); ++Index )
  {
    if( !::isWordChar(datum(Index),CharType) )
      return Index;
  }
  // word reaches the end, so step behind
  return size();
}


int KDataBuffer::indexOfBehindLeftWordEnd( int Index, KDataBuffer::KWordCharType CharType ) const
{
  for( --Index; Index>=0; --Index )
  {
    if( ::isWordChar(datum(Index),CharType) )
      return Index+1;
  }
  // word reaches the end, so step behind
  return 0;
}
*/

KSection KDataBuffer::wordSection( int Index, KWordCharType CharType ) const
{
  return ( isWordChar(Index, CharType) ) ? 
         KSection( indexOfWordStart(Index,CharType), indexOfWordEnd(Index,CharType) ) : 
	 KSection();
}
