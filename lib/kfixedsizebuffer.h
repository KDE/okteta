/***************************************************************************
                          kfixedsizebuffer.h  -  description
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

 
#ifndef KHE_KFIXEDSIZEBUFFER_H
#define KHE_KFIXEDSIZEBUFFER_H

#include "kdatabuffer.h"

namespace KHE
{

/** base class for all Data buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class KFixedSizeBuffer : public KDataBuffer
{
  public:
    KFixedSizeBuffer( char *D, int S, char FUC = '\0' );
    virtual ~KFixedSizeBuffer();

  public: // KDataBuffer API
    /** locates working range, returns false if failed */
    virtual bool prepareRange( KSection Range );
    /** expects pointer to memory, should be in prepared range */
    virtual const char *dataSet( KSection S ) const;
    /** */
    virtual char datum( int Offset ) const;
    /***/
    virtual int size() const;
    /** is the buffer changeable ?*/
    virtual bool isReadOnly() const;
    /** has the buffer been modified? */
    virtual bool isModified() const;

    /** inserts at Position */
    virtual int insert( int Pos, const char*, int Length );
    /** removes beginning with position as much as possible, returns length of removed */
    virtual int remove( KSection Remove );
    /** replaces as much as possible, returns length of substituted */
    virtual int replace( KSection Remove, const char*, int InputLength );

    virtual void setModified( bool M = true );

    /** searches beginning with byte at Pos, returns -1 if nothing found */
    virtual int find( const char*, int Length, int Pos = 0 ) const;
    /** searches backward beginning with byte at Pos, returns -1 if nothing found */
    virtual int rfind( const char*, int Length, int Pos = -1 ) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

  public:
    void setReadOnly( bool RO = true );

  protected:
    void move( int DestPos, int SourcePos, int Length );
    void reset( int Pos, int Length );
    void copy( int DestPos, const char *Source, int SourceLength );

  protected:
    /** */
    char *Data;
    /***/
    int Size;
    /** */
    char FillUpChar;
    /**  */
    bool ReadOnly:1;
    /** */
    bool Modified:1;
};


inline bool KFixedSizeBuffer::prepareRange( KSection ) { return true; }
inline const char *KFixedSizeBuffer::dataSet( KSection S ) const { return &Data[S.start()]; }

inline char KFixedSizeBuffer::datum( int Offset ) const { return Data[Offset]; }
inline int KFixedSizeBuffer::size() const  { return Size; }


inline bool KFixedSizeBuffer::isReadOnly()   const { return ReadOnly; }
inline bool KFixedSizeBuffer::isModified()   const { return Modified; }

inline void KFixedSizeBuffer::setReadOnly( bool RO )  { ReadOnly = RO; }
inline void KFixedSizeBuffer::setModified( bool M )   { Modified = M; }

}

#endif
