/***************************************************************************
                          kwrappingrobuffer.h  -  description
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


#ifndef KWRAPPINGROBUFFER_H
#define KWRAPPINGROBUFFER_H

#include "kreadonlybuffer.h"

namespace KHE
{

/**
  *@author Friedrich W. H. Kossebau
  */

class KWrappingROBuffer : public KReadOnlyBuffer
{
  public:
    static const int NothingFound = -1;

  public:
    KWrappingROBuffer();
    KWrappingROBuffer( const char* D, int L );
    virtual ~KWrappingROBuffer();


  public: // KDataBuffer API
    virtual bool prepareRange( KSection Range ) const;
    virtual const char *dataSet( KSection S ) const;
    virtual char datum( int Offset ) const;
    virtual int size() const;
    virtual bool isModified() const;

    virtual int insert( int Pos, const char*, int Length );
    virtual int remove( KSection S );
    virtual int replace( KSection S, const char*, int Length );

    virtual void setModified( bool M );

    virtual int find( const char*, int Length, int Pos = 0 ) const;
    virtual int rfind( const char*, int Length, int Pos = -1 ) const;


  public: //
    void set( const char* D, int L );

  protected:
    const char* Data;
    int Size;
    bool Modified;
};

inline int KWrappingROBuffer::size() const { return Size; }

inline bool KWrappingROBuffer::isModified() const { return Modified; }

inline bool KWrappingROBuffer::prepareRange( KSection ) const     { return true; }

inline int KWrappingROBuffer::insert( int, const char*, int )        { return 0; }
inline int KWrappingROBuffer::remove( KSection )                     { return 0; }
inline int KWrappingROBuffer::replace( KSection, const char*, int )  { return 0; }

inline void KWrappingROBuffer::setModified( bool M )  { Modified = M; }
}

#endif
