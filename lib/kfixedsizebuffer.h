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
    /** creates a readonly buffer around the given data */
    KFixedSizeBuffer( char *D, int S, char FUC = '\0' );
    /** creates a writeable buffer which is deleted at the end */
    KFixedSizeBuffer( int S, char FUC = '\0' );
    virtual ~KFixedSizeBuffer();

  public: // KDataBuffer API
    virtual bool prepareRange( KSection Range ) const;
    virtual const char *dataSet( KSection S ) const;
    virtual char datum( int Offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int Pos, const char*, int Length );
    virtual int remove( KSection Remove );
    virtual int replace( KSection Remove, const char*, int InputLength );
    virtual int move( int DestPos, KSection SourceSection );
    virtual int fill( const char FillChar, int Length = -1, unsigned int Pos = 0 );

    virtual void setModified( bool M = true );

    virtual int find( const char*KeyData, int Length, KSection Section ) const;
    virtual int rfind( const char*, int Length, int Pos = -1 ) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

  public:
    void setReadOnly( bool RO = true );
    int compare( const KDataBuffer &Other, KSection Range, unsigned int Pos = 0 );
    int compare( const KDataBuffer &Other, int OtherPos, int Length, unsigned int Pos = 0 );
    int compare( const KDataBuffer &Other );

  public:
    char *rawData() const;

  protected:
    void fmove( int DestPos, int SourcePos, int Length );
    void reset( int Pos, int Length );
    void fcopy( int DestPos, const char *Source, int SourceLength );

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
    /** */
    bool AutoDelete:1;
};


inline bool KFixedSizeBuffer::prepareRange( KSection ) const { return true; }
inline const char *KFixedSizeBuffer::dataSet( KSection S ) const { return &Data[S.start()]; }

inline char KFixedSizeBuffer::datum( int Offset ) const { return Data[Offset]; }
inline int KFixedSizeBuffer::size() const  { return Size; }


inline bool KFixedSizeBuffer::isReadOnly()   const { return ReadOnly; }
inline bool KFixedSizeBuffer::isModified()   const { return Modified; }

inline void KFixedSizeBuffer::setReadOnly( bool RO )  { ReadOnly = RO; }
inline void KFixedSizeBuffer::setModified( bool M )   { Modified = M; }

inline int KFixedSizeBuffer::compare( const KDataBuffer &Other )
{ return compare( Other, KSection(0,Other.size()-1),0 ); }

inline int KFixedSizeBuffer::compare( const KDataBuffer &Other, int OtherPos, int Length, unsigned int Pos )
{ return compare( Other, KSection(OtherPos,Length),Pos ); }

inline char *KFixedSizeBuffer::rawData() const { return Data; }

}

#endif
