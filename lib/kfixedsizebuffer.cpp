/***************************************************************************
                          kfixedsizebuffer.cpp  -  description
                             -------------------
    begin                : Mit Jun 03 2003
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
#include <string.h>
// app specific
#include "kfixedsizebuffer.h"

using namespace KHE;

KFixedSizeBuffer::KFixedSizeBuffer( char *D, int S, char FUC )
 : Data( D ),
   Size( S ),
   FillUpChar( FUC ),
   ReadOnly( true ),
   Modified( false )
{
}

KFixedSizeBuffer::~KFixedSizeBuffer()
{
}



int KFixedSizeBuffer::insert( int Pos, const char* D, int InputLength )
{
  // check all parameters
  if( Pos >= Size || InputLength == 0 )
    return 0;
  if( Pos + InputLength > Size )
    InputLength = Size - Pos;

  int BehindInsertPos = Pos + InputLength;
  // move right data behind the input range
  move( BehindInsertPos, Pos, Size-BehindInsertPos );
  // insert input
  copy( Pos, D, InputLength );

  Modified = true;
  return InputLength;
}


int KFixedSizeBuffer::remove( KSection Remove )
{
  if( Remove.start() >= Size || Remove.width() == 0 )
    return 0;

  Remove.restrictEndTo( Size-1 );

  int RemoveLength = Remove.width();
  int BehindRemovePos = Remove.end()+1;;
  // move right data behind the input range
  move( Remove.start(), BehindRemovePos, Size-BehindRemovePos );
  // clear freed space
  reset( Size-RemoveLength, RemoveLength );

  Modified = true;
  return RemoveLength;
}


int KFixedSizeBuffer::replace( KSection Remove, const char* D, int InputLength )
{
  // check all parameters
  if( Remove.startsBehind( Size-1 ) || (Remove.width()==0 && InputLength==0) )
    return 0;

  Remove.restrictEndTo( Size-1 );
  if( Remove.start() + InputLength > Size )
    InputLength = Size - Remove.start();

  int SizeDiff = InputLength - Remove.width();

  // is input longer than removed?
  if( SizeDiff > 0 )
  {
    int BehindInsertPos = Remove.start() + InputLength;
    // move right data behind the input range
    move( BehindInsertPos, Remove.end()+1, Size-BehindInsertPos );
  }
  // is input smaller than removed?
  else if( SizeDiff < 0 )
  {
    int BehindRemovePos = Remove.end()+1;
    // move right data behind the input range
    move( Remove.start()+InputLength, BehindRemovePos, Size-BehindRemovePos );
    // clear freed space
    reset( Size-SizeDiff, SizeDiff );
  }
  // insert input
  copy( Remove.start(), D, InputLength );

  Modified = true;
  return InputLength;
}


inline void KFixedSizeBuffer::move( int DestPos, int SourcePos, int Length )
{
  memmove( &Data[DestPos], &Data[SourcePos], Length );
}


inline void KFixedSizeBuffer::reset( int Pos, int Length )
{
  memset( &Data[Pos], FillUpChar, Length );
}


inline void KFixedSizeBuffer::copy( int DestPos, const char *Source, int SourceLength )
{
  memcpy( &Data[DestPos], Source, SourceLength );
}


int KFixedSizeBuffer::find( const char*, int /*Length*/, int /*Pos*/ ) const  { return 0; }
int KFixedSizeBuffer::rfind( const char*, int /*Length*/, int /*Pos*/ ) const { return 0; }
