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

//#include <kdebug.h>
// c specific
#include <string.h>
#include <stdlib.h>
// lib specific
#include "kplainbuffer.h"


static const unsigned int MinChunkSize = 512;
static const unsigned int MaxChunkSize = 1024*10; // TODO: get max. memory page size

// TODO: think about realloc & Co.
using namespace KHE;

KPlainBuffer::KPlainBuffer( char *D, unsigned int S, int RS, bool KM )
 : Data( D ),
   Size( S ),
   RawSize( RS<(int)S?S:RS ),
   MaxSize( -1 ),
   KeepsMemory( KM ),
   ReadOnly( true ),
   Modified( false )
{
}

KPlainBuffer::KPlainBuffer( const char *D, unsigned int S )
 : Data( (char *)D ),
   Size( S ),
   RawSize( S ),
   MaxSize( -1 ),
   KeepsMemory( true ),
   ReadOnly( true ),
   Modified( false )
{
}

KPlainBuffer::KPlainBuffer( int S, int MS )
  : Data( S?new char[S]:0 ),
   Size( S ),
   RawSize( S ),
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
  //kdDebug() << QString("before: Size: %1, RawSize: %2").arg(Size).arg(RawSize) << endl;
  // correct for appending
  if( Pos > (int)Size )
    Pos = Size;

  Length = addSize( Length, Pos, true );

  // copy new data to its place
  memcpy( &Data[Pos], D, Length );

  //kdDebug() << QString("after: Size: %1, RawSize: %2").arg(Size).arg(RawSize) << endl;

  Modified = true;
  return Length;
}


int KPlainBuffer::remove( KSection Remove )
{
  if( Remove.startsBehind(Size-1) || Remove.width() == 0 )
    return 0;

  Remove.restrictEndTo( Size-1 );

  unsigned int BehindRemovePos = Remove.end()+1;
  // move right data behind the input range
  memmove( &Data[Remove.start()], &Data[BehindRemovePos], Size-BehindRemovePos );

  // set new values
  Size -= Remove.width();

  Modified = true;
  return Remove.width();
}


unsigned int KPlainBuffer::replace( KSection Remove, const char* D, unsigned int InputLength )
{
  // check all parameters
  if( Remove.start() >= (int)Size || (Remove.width()==0 && InputLength==0) )
    return 0;

  Remove.restrictEndTo( Size-1 );

  int SizeDiff = InputLength - Remove.width();
  unsigned int NewSize = Size + SizeDiff;
  // check if buffer does not get to big TODO: make algo simplier and less if else
  if( MaxSize != -1 && (int)NewSize > MaxSize)
  {
    if( (int)Size == MaxSize )
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


int KPlainBuffer::move( int DestPos, KSection SourceSection )
{
  // check all parameters
  if( SourceSection.start() >= (int)Size || SourceSection.width() == 0
      || DestPos > (int)Size || SourceSection.start() == DestPos )
    return SourceSection.start();

  SourceSection.restrictEndTo( Size-1 );
  bool ToRight = DestPos > SourceSection.start();
  int MovedLength = SourceSection.width();
  int DisplacedLength = ToRight ?  DestPos - SourceSection.end()-1 : SourceSection.start() - DestPos;

  // find out section that is smaller
  int SmallPartLength, LargePartLength, SmallPartStart, LargePartStart, SmallPartDest, LargePartDest;
  // moving part is smaller?
  if( MovedLength < DisplacedLength )
  {
    SmallPartStart = SourceSection.start();
    SmallPartLength = MovedLength;
    LargePartLength = DisplacedLength;
    // moving part moves right?
    if( ToRight )
    {
      SmallPartDest = DestPos - MovedLength;
      LargePartStart = SourceSection.end()+1;
      LargePartDest = SourceSection.start();
    }
    else
    {
      SmallPartDest = DestPos;
      LargePartStart = DestPos;
      LargePartDest = DestPos + MovedLength;
    }
  }
  else
  {
    LargePartStart = SourceSection.start();
    LargePartLength = MovedLength;
    SmallPartLength = DisplacedLength;
    // moving part moves right?
    if( ToRight )
    {
      LargePartDest = DestPos - MovedLength;
      SmallPartStart = SourceSection.end()+1;
      SmallPartDest = SourceSection.start();
    }
    else
    {
      LargePartDest = DestPos;
      SmallPartStart = DestPos;
      SmallPartDest = DestPos + MovedLength;
    }
  }

  // copy smaller part to tempbuffer
  char *Temp = new char[SmallPartLength];
  memcpy( Temp, &Data[SmallPartStart], SmallPartLength );

  // move the larger part
  memmove( &Data[LargePartDest], &Data[LargePartStart], LargePartLength );

  // copy smaller part to its new dest
  memcpy( &Data[SmallPartDest], Temp, SmallPartLength );
  delete [] Temp;

  Modified = true;
  return MovedLength < DisplacedLength ? SmallPartDest : LargePartDest;
}


int KPlainBuffer::fill( const char FChar, int FillLength, unsigned int Pos )
{
  // nothing to fill
  if( Pos >= Size )
    return 0;

  int LengthToEnd = Size - Pos;

  if( FillLength < 0 )
    FillLength = LengthToEnd;
  else if( FillLength > LengthToEnd )
    FillLength = addSize( FillLength, Pos, false );

  memset( &Data[Pos], FChar, FillLength );
  Modified = true;
  return FillLength;
}


int KPlainBuffer::find( const char* SearchString, int Length, KSection Section ) const  
{
  Section.restrictEndTo( Size-1 );

  for( int i = Section.start(); i <= Section.end(); ++i )
  {
    int Result;
//    if( IgnoreCase )
//      result = strncasecmp( &data()[i], sc.key.data(), sc.key.size() );
//    else
    Result = memcmp( &Data[i], SearchString, Length );
    // found?
    if( Result == 0 )
      return i;
  }
  return -1;
}

int KPlainBuffer::rfind( const char*, int /*Length*/, int /*Pos*/ ) const { return 0; }


int KPlainBuffer::addSize( int AddSize, int SplitPos, bool SaveUpperPart )
{
  unsigned int NewSize = Size + AddSize;
  // check if buffer does not get too big
  if( MaxSize != -1 && (int)NewSize > MaxSize )
  {
    if( (int)Size == MaxSize )
      return 0;
    NewSize = MaxSize;
    AddSize = NewSize - Size;
  }
  else if( KeepsMemory && NewSize > RawSize )
  {
    if( Size == RawSize )
      return 0;
    NewSize = RawSize;
    AddSize = NewSize - Size;
  }

  int BehindSplitPos = SplitPos + AddSize;
  // raw array not big enough?
  if( RawSize < NewSize )
  {
    // get new raw size
    unsigned int ChunkSize = MinChunkSize;
    // find chunk size where newsize fits into
    while( ChunkSize < NewSize )
      ChunkSize <<= 1;
    // limit to max size
    if( ChunkSize > MaxChunkSize )
      ChunkSize = MaxChunkSize;
    // find add size
    unsigned int NewRawSize = ChunkSize;
    while( NewRawSize<NewSize )
      NewRawSize += ChunkSize;
    // create new buffer
    char *NewData = new char[NewRawSize];

    // move old data to its (new) places
    memcpy( NewData, Data, SplitPos );
    if( SaveUpperPart )
      memcpy( &NewData[BehindSplitPos], &Data[SplitPos], Size-SplitPos );

    // remove old
    delete [] Data;
    // set new values
    Data = NewData;
    RawSize = NewRawSize;
  }
  // old buffer kept
  else
  {
    if( SaveUpperPart )
      // move old data to its (new) places
      memmove( &Data[BehindSplitPos], &Data[SplitPos], Size-SplitPos );
  }

  // set new values
  Size = NewSize;

  return AddSize;
}
