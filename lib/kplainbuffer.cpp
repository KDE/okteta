/***************************************************************************
                          kplainbuffer.cpp  -  description
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
#include "kplainbuffer.h"

using namespace KHE;

KPlainBuffer::KPlainBuffer( char *D, int S, int RS, bool KM )
 : Data( D ),
   Size( S ),
   RawSize( RS<S?S:RS ),
   MaxSize( -1 ),
   KeepsMemory( KM ),
   ReadOnly( true ),
   Modified( false )
{
}

KPlainBuffer::KPlainBuffer( const char *D, int S )
 : Data( (char *)D ),
   Size( S ),
   RawSize( S ),
   MaxSize( -1 ),
   KeepsMemory( true ),
   ReadOnly( true ),
   Modified( false )
{
}

KPlainBuffer::KPlainBuffer( int MS )
 : Data( 0L ),
   Size( 0 ),
   RawSize( 0 ),
   MaxSize( MS ),
   KeepsMemory( false ),
   ReadOnly( true ),
   Modified( false )
{
}

KPlainBuffer::~KPlainBuffer()
{
}



int KPlainBuffer::insert( int Pos, const char* D, int Length )
{
  // check all parameters
  if( Length == 0 )
    return 0;

  // correct for appending
  if( Pos > Size )
    Pos = Size;

  int NewSize = Size + Length;
  // check if buffer does not get to big TODO: make algo simplier and less if else
  if( MaxSize != -1 && NewSize > MaxSize)
  {
    if( Size == MaxSize )
      return 0;
    Length -= NewSize - MaxSize;
    NewSize = MaxSize;
  }
  else if( KeepsMemory && NewSize > RawSize )
  {
    if( Size == RawSize )
      return 0;
    Length -= NewSize - RawSize;
    NewSize = RawSize;
  }

  int BehindInsertPos = Pos + Length;
  // raw array not big enough?
  if( RawSize < NewSize )
  {
    // create new buffer
    char *NewData = new char[NewSize];
    if( NewData == 0 )
      return 0;

    // move old data to its (new) places
    memcpy( NewData, Data, Pos );
    memcpy( &NewData[BehindInsertPos], &Data[Pos], Size-Pos );

    // remove old
    delete [] Data;
    // set new values
    Data = NewData;
    RawSize = NewSize;
  }
  else
    // move old data to its (new) places
    memmove( &Data[BehindInsertPos], &Data[Pos], Size-Pos );

  // copy new data to its place
  memcpy( &Data[Pos], D, Length );

  // set new values
  Size = NewSize;

  Modified = true;
  return Length;
}


int KPlainBuffer::remove( KSection Remove )
{
  if( Remove.startsBehind(Size-1) || Remove.width() == 0 )
    return 0;

  Remove.restrictEndTo( Size-1 );

  int BehindRemovePos = Remove.end()+1;
  // move right data behind the input range
  memmove( &Data[Remove.start()], &Data[BehindRemovePos], Size-BehindRemovePos );

  // set new values
  Size -= Remove.width();

  Modified = true;
  return Remove.width();
}


int KPlainBuffer::replace( KSection Remove, const char* D, int InputLength )
{
  // check all parameters
  if( Remove.start() >= Size || (Remove.width()==0 && InputLength==0) )
    return 0;

  Remove.restrictEndTo( Size-1 );

  int SizeDiff = InputLength - Remove.width();
  int NewSize = Size + SizeDiff;
  // check if buffer does not get to big TODO: make algo simplier and less if else
  if( MaxSize != -1 && NewSize > MaxSize)
  {
    if( Size == MaxSize )
      return 0;
    InputLength -= NewSize - MaxSize;
    NewSize = MaxSize;
  }
  else if( KeepsMemory && NewSize > RawSize )
  {
    if( Size == RawSize )
      return 0;
    InputLength -= NewSize - RawSize;
    NewSize = RawSize;
  }

  int BehindInsertPos = Remove.start() + InputLength;
  int BehindRemovePos = Remove.end()+1;

  // raw array not big enough?
  if( RawSize < NewSize )
  {
    // create new buffer
    char *NewData = new char[NewSize];
    if( NewData == 0 )
      return 0;

    // move old data to its (new) places
    memcpy( NewData, Data, Remove.start() );
    memcpy( &NewData[BehindInsertPos], &Data[BehindRemovePos], Size-BehindRemovePos );

    // remove old
    delete [] Data;
    // set new values
    Data = NewData;
    RawSize = NewSize;
  }
  else
    // move old data to its (new) places
    memmove( &Data[BehindInsertPos], &Data[BehindRemovePos], Size-BehindRemovePos );

  // copy new data to its place
  memcpy( &Data[Remove.start()], D, InputLength );

  // set new values
  Size = NewSize;

  Modified = true;
  return InputLength;
}


int KPlainBuffer::find( const char*, int /*Length*/, int /*Pos*/ ) const  { return 0; }
int KPlainBuffer::rfind( const char*, int /*Length*/, int /*Pos*/ ) const { return 0; }
