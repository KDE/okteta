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


#ifndef KHE_KWRAPPINGROBUFFER_H
#define KHE_KWRAPPINGROBUFFER_H

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
    virtual char datum( unsigned int Offset ) const;
    virtual int size() const;
    virtual bool isModified() const;

    virtual int insert( int Pos, const char*, int Length );
    virtual int remove( KSection S );
    virtual unsigned int replace( KSection S, const char*, unsigned int Length );
    virtual int fill( const char FillChar, int, int );
    virtual void setDatum( unsigned int Offset, const char Char );

    virtual void setModified( bool M );

    virtual int find( const char*KeyData, int Length, KSection Section ) const;
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
inline char KWrappingROBuffer::datum( unsigned int Offset ) const { return Data[Offset]; }

inline int KWrappingROBuffer::insert( int, const char*, int )        { return 0; }
inline int KWrappingROBuffer::remove( KSection )                     { return 0; }
inline unsigned int KWrappingROBuffer::replace( KSection, const char*, unsigned int )  { return 0; }
inline int KWrappingROBuffer::fill( const char , int, int )          { return 0; }
inline void KWrappingROBuffer::setDatum( unsigned int, const char )  {}

inline void KWrappingROBuffer::setModified( bool M )  { Modified = M; }
}

#endif
