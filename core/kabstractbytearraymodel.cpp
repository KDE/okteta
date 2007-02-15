/***************************************************************************
                          kabstractbytearraymodel.cpp  -  description
                             -------------------
    begin                : Fri Aug 01 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
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


namespace KHECore {

KAbstractByteArrayModel::KAbstractByteArrayModel() {}

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


int KAbstractByteArrayModel::indexOf( const char* Data, int Length, int From ) const
{
    int Result = -1;

    const int LastFrom = size() - Length;

    for( int i=From; i<=LastFrom ; ++i )
    {
        int c = 0;
        for( ; c<Length; ++c )
            if( Data[c] != datum(i+c) )
                break;
        if( c == Length )
        {
            Result = i;
            break;
        }
    }

    return Result;
}

int KAbstractByteArrayModel::lastIndexOf( const char* Data, int Length, int From ) const
{
    int Result = -1;

    const int LastFrom = size() - Length;

    if( From < 0 )
        From = LastFrom + 1 + From;
    else if( From > LastFrom )
        From = LastFrom;

    for( int i=From; i>=0 ; --i )
    {
        int c = 0;
        for( ; c<Length; ++c )
            if( Data[c] != datum(i+c) )
                break;
        if( c == Length )
        {
            Result = i;
            break;
        }
    }

    return Result;
}

KAbstractByteArrayModel::~KAbstractByteArrayModel() {}

}

#include "kabstractbytearraymodel.moc"
