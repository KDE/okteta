/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "fixedsizebytearraymodel.h"

// lib
#include <arraychangemetricslist.h>
// C
#include <string.h>


namespace Okteta
{

FixedSizeByteArrayModel::FixedSizeByteArrayModel( char *D, unsigned int S, char FUC )
  : Data( D ),
   Size( S ),
   FillUpChar( FUC ),
   ReadOnly( true ),
   Modified( false ),
   AutoDelete( false )
{
}

FixedSizeByteArrayModel::FixedSizeByteArrayModel( unsigned int S, char FUC )
  : Data( new char[S] ),
   Size( S ),
   FillUpChar( FUC ),
   ReadOnly( false ),
   Modified( false ),
   AutoDelete( true )
{
  reset( 0, S );
}

FixedSizeByteArrayModel::~FixedSizeByteArrayModel()
{
  if( AutoDelete )
    delete [] Data;
}



void FixedSizeByteArrayModel::setDatum( unsigned int Offset, const char Char )
{
    const bool wasModifiedBefore = Modified;

  Data[Offset] = Char;
  Modified = true;

  emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(Offset, 1, 1) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );
}



int FixedSizeByteArrayModel::insert( int Pos, const char* D, int InputLength )
{
  // check all parameters
  if( Pos >= (int)Size || InputLength == 0 )
    return 0;

    const bool wasModifiedBefore = Modified;

  if( Pos + InputLength > (int)Size )
    InputLength = Size - Pos;

  unsigned int BehindInsertPos = Pos + InputLength;
  // fmove right data behind the input range
  memmove( &Data[BehindInsertPos], &Data[Pos], Size-BehindInsertPos );
  // insert input
  memcpy( &Data[Pos], D, InputLength );

  Modified = true;

  emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(Pos, 0, InputLength) );
  //emit contentsReplaced( Pos, , 0 ); TODO: how to signal the removed data?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );
  return InputLength;
}


int FixedSizeByteArrayModel::remove( const KDE::Section& R )
{
  KDE::Section Remove( R );
  if( Remove.start() >= (int)Size || Remove.width() == 0 )
    return 0;

    const bool wasModifiedBefore = Modified;

  Remove.restrictEndTo( Size-1 );

  int RemoveLength = Remove.width();
  int BehindRemovePos = Remove.nextBehindEnd();
  // fmove right data behind the input range
  memmove( &Data[Remove.start()], &Data[BehindRemovePos], Size-BehindRemovePos );
  // clear freed space
  reset( Size-RemoveLength, RemoveLength );

  Modified = true;

  emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(Remove.start(), Remove.width(), 0) );
  //emit contentsReplaced( Pos, 0,  ); TODO: how to signal the inserted data?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );
  return RemoveLength;
}


unsigned int FixedSizeByteArrayModel::replace( const KDE::Section& R, const char* D, unsigned int InputLength )
{
  KDE::Section Remove( R );
  // check all parameters
  if( Remove.startsBehind( Size-1 ) || (Remove.width()==0 && InputLength==0) )
    return 0;

    const bool wasModifiedBefore = Modified;

  Remove.restrictEndTo( Size-1 );
  if( Remove.start() + InputLength > Size )
    InputLength = Size - Remove.start();

  int SizeDiff = InputLength - Remove.width();

  // is input longer than removed?
  if( SizeDiff > 0 )
  {
    unsigned int BehindInsertPos = Remove.start() + InputLength;
    // fmove right data behind the input range
    memmove( &Data[BehindInsertPos], &Data[Remove.nextBehindEnd()], Size-BehindInsertPos );
  }
  // is input smaller than removed?
  else if( SizeDiff < 0 )
  {
    unsigned int BehindRemovePos = Remove.nextBehindEnd();
    // fmove right data behind the input range
    memmove( &Data[Remove.start()+InputLength], &Data[BehindRemovePos], Size-BehindRemovePos );
   // clear freed space
    reset( Size+SizeDiff, -SizeDiff );
  }
  // insert input
  memcpy( &Data[Remove.start()], D, InputLength );

  Modified = true;

  emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(Remove.start(), Remove.width(), InputLength) );
  //emit contentsReplaced( Pos, 0,  ); TODO: how to signal the changed data at the end?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );
  return InputLength;
}


bool FixedSizeByteArrayModel::swap( int firstStart, const KDE::Section& secondSection )
{
  KDE::Section SourceSection( secondSection );
  // check all parameters
  if( SourceSection.start() >= (int)Size || SourceSection.width() == 0
      || firstStart > (int)Size || SourceSection.start() == firstStart )
    return false;

    const bool wasModifiedBefore = Modified;

  SourceSection.restrictEndTo( Size-1 );
  bool ToRight = firstStart > SourceSection.start();
  int MovedLength = SourceSection.width();
  int DisplacedLength = ToRight ?  firstStart - SourceSection.end()-1 : SourceSection.start() - firstStart;

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
      SmallPartDest = firstStart - MovedLength;
      LargePartStart = SourceSection.nextBehindEnd();
      LargePartDest = SourceSection.start();
    }
    else
    {
      SmallPartDest = firstStart;
      LargePartStart = firstStart;
      LargePartDest = firstStart + MovedLength;
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
      LargePartDest = firstStart - MovedLength;
      SmallPartStart = SourceSection.nextBehindEnd();
      SmallPartDest = SourceSection.start();
    }
    else
    {
      LargePartDest = firstStart;
      SmallPartStart = firstStart;
      SmallPartDest = firstStart + MovedLength;
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

  emit contentsChanged( KDE::ArrayChangeMetricsList::oneSwapping(firstStart, SourceSection.start(),SourceSection.width()) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );
  return true;
}


int FixedSizeByteArrayModel::fill( const char FChar, unsigned int Pos, int FillLength )
{
  // nothing to fill
  if( Pos >= Size )
    return 0;

    const bool wasModifiedBefore = Modified;

  unsigned int LengthToEnd = Size - Pos;

  if( FillLength < 0 || FillLength > (int)LengthToEnd )
    FillLength = LengthToEnd;

  memset( &Data[Pos], FChar, FillLength );
  Modified = true;

  emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(Pos, FillLength, FillLength) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );
  return FillLength;
}


int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel &Other, const KDE::Section& OR, unsigned int Pos )
{
  KDE::Section OtherRange( OR );
  //kDebug() << QString("Pos: %1, OtherRange: (%3/%4)" ).arg(Pos).arg(OtherRange.start()).arg(OtherRange.end())
  //    << endl;
  // test other values
  if( OtherRange.startsBehind(Other.size()-1) )
    return 1;

  // check own values
  if( Pos >= Size )
    return -1;

  int ValueByLength = 0; // default: equal

  KDE::Section Range = KDE::Section::fromWidth( Pos, OtherRange.width() );
  int Last = Other.size()-1;
  // 
  if( OtherRange.endsBehind(Last) )
  {
    // make shorter
    OtherRange.setEnd( Last );
    if( OtherRange.width() < Range.width() )
      ValueByLength = 1;
  }
  Last = Size-1;
  if( Range.endsBehind(Last) )
  {
    // make shorter
    Range.setEnd( Last );
    if( OtherRange.width() > Range.width() )
      ValueByLength = -1;
  }
  //kDebug()
  //    << QString( "Range: (%1/%2), OtherRange: (%3/%4)" ).arg(Range.start()).arg(Range.end()).arg(OtherRange.start()).arg(OtherRange.end())
  //    << endl;
  int oi = OtherRange.start();
  for( int i=Range.start(); i<=Range.end(); ++i,++oi )
  {
    char OD = Other.datum(oi);
    char D = Data[i];
    //kDebug() << QString("%1==%2").arg((int)D).arg((int)OD) ;
    if( OD == D )
      continue;
    return OD < D ? 1 : -1;
  }

  return ValueByLength;
}


void FixedSizeByteArrayModel::reset( unsigned int Pos, unsigned int Length )
{
  memset( &Data[Pos], FillUpChar, Length );
}

}
