/***************************************************************************
                          kwrappingrobuffer.cpp  -  description
                             -------------------
    begin                : Mit Mai 14 2003
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

 
#include "kwrappingrobuffer.h"
 
using namespace KHE;

KWrappingROBuffer::KWrappingROBuffer( const char* D, int L )
{
  Data = D;
  Size = L;
}

KWrappingROBuffer::KWrappingROBuffer()
{
  Data = 0L;
  Size = 0;
}


KWrappingROBuffer::~KWrappingROBuffer()
{
}


const char *KWrappingROBuffer::dataSet( KSection S ) const
{
  return &Data[S.start()];
}


void KWrappingROBuffer::set( const char* D, int L )
{
  Data = D;
  Size = L;
}


int KWrappingROBuffer::find( const char*/*KeyData*/, int /*Length*/, KSection /*Section*/ ) const
{
  return 0;
}

int KWrappingROBuffer::rfind( const char*, int /*Length*/, int /*Pos*/ ) const
{
  return 0;
}
