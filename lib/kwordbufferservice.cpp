/***************************************************************************
                          kwordbufferservice.cpp  -  description
                             -------------------
    begin                : Di Jan 18 2005
    copyright            : (C) 2005 by Friedrich W. H. Kossebau
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
#include "kdatabuffer.h"
#include "kcharcodec.h"
#include "kwordbufferservice.h"

using namespace KHE;


bool KWordBufferService::isWordChar( unsigned int Index ) const
{
  KHEChar C = CharCodec->decode( Buffer->datum(Index) );
  return !C.isUndefined() && C.isLetterOrNumber();
}


int KWordBufferService::indexOfPreviousWordStart( unsigned int Index ) const
{
  unsigned int Size = Buffer->size();
  // already at the start or can the result only be 0?
  if( Index == 0 || Size < 3 )
    return 0;

  // search in two rounds: first for the next char, than for the next nonchar
  // after that return the index of the one before
  bool LookingForFirstWordChar = false;
  for( ; Index>0; --Index )
  {
    if( !isWordChar(Index-1) )
    {
      if( !LookingForFirstWordChar )
        continue;
      return( Index );
    }
    else if( !LookingForFirstWordChar )
      LookingForFirstWordChar = true;
  }
  return 0;
}


int KWordBufferService::indexOfNextWordStart( unsigned int Index ) const
{
  unsigned int Size = Buffer->size();
  bool LookingForFirstWordChar = false;
  for( ; Index<Size; ++Index )
  {
    if( isWordChar(Index) )
    {
      if( !LookingForFirstWordChar )
        continue;
      return Index;
    }
    else if( !LookingForFirstWordChar )
      LookingForFirstWordChar = true;
  }
  // if no more word found, go to the end
  return Size;
}


int KWordBufferService::indexOfBeforeNextWordStart( unsigned int Index ) const
{
  unsigned int Size = Buffer->size();
  bool LookingForFirstWordChar = false;
  for( ; Index<Size; ++Index )
  {
    if( isWordChar(Index) )
    {
      if( !LookingForFirstWordChar )
        continue;
      return Index-1;
    }
    else if( !LookingForFirstWordChar )
      LookingForFirstWordChar = true;
  }
  // if no more word found, go to the end
  return Size-1;
}


int KWordBufferService::indexOfWordStart( unsigned int Index ) const
{
  for( ; Index > 0; --Index )
  {
    if( !isWordChar(Index-1) )
      return( Index );
  }
  return 0;
}


int KWordBufferService::indexOfWordEnd( unsigned int Index ) const
{
  unsigned int Size = Buffer->size();
  for( ++Index; Index<Size; ++Index )
  {
    if( !isWordChar(Index) )
      return Index-1;
  }
  // word reaches the end
  return Size-1;
}


int KWordBufferService::indexOfLeftWordSelect( unsigned int Index ) const
{
  // word at Index?
  if( isWordChar(Index) )
  {
    // search for word start to the left
    for( ; Index>0; --Index )
    {
      if( !isWordChar(Index-1) )
        return Index;
    }
    // reached start, so return it
    return 0;
  }
  else
  {
    unsigned int Size = Buffer->size();
    // search for word start to the right
    for( ++Index; Index<Size; ++Index )
    {
      if( isWordChar(Index) )
        return Index;
    }
    // word reaches the end, so step behind
    return Size;
  }
}


int KWordBufferService::indexOfRightWordSelect( unsigned int Index ) const
{
  // no word at Index?
  if( !isWordChar(Index) )
  {
    // search for word end to the left
    for( ; Index>0; --Index )
    {
      if( isWordChar(Index-1) )
        return Index;
    }
    // reached start, so return it
    return 0;
  }
  else
  {
    unsigned int Size = Buffer->size();
    for( ++Index; Index<Size; ++Index )
    {
      // search for word end to the right
      if( !isWordChar(Index) )
        return Index;
    }
    // word reaches the end, so step behind
    return Size;
  }
}

/*
int KWordBufferService::indexOfBehindWordEnd( unsigned int Index ) const
{
  // no word at Index?
  return !::isWordChar(datum(Index)) ? indexOfBehindLeftWordEnd(Index) : indexOfBehindRightWordEnd(Index+1)
}


int KWordBufferService::indexOfBehindRightWordEnd( unsigned int Index ) const
{
  for( ; Index<size(); ++Index )
{
    if( !::isWordChar(datum(Index)) )
      return Index;
}
  // word reaches the end, so step behind
  return size();
}


int KWordBufferService::indexOfBehindLeftWordEnd( unsigned int Index ) const
{
  for( --Index; Index>=0; --Index )
{
    if( ::isWordChar(datum(Index)) )
      return Index+1;
}
  // word reaches the end, so step behind
  return 0;
}
*/
