/***************************************************************************
                          kplainbuffer.h  -  description
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


#ifndef KPLAINBUFFER_H
#define KPLAINBUFFER_H

#include <kdatabuffer.h>

namespace KHE
{

/** base class for all Data buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class KPlainBuffer : public KDataBuffer
{
  public:
    KPlainBuffer( char *D, int S, int RS = -1, bool KM = true );
    KPlainBuffer( const char *D, int S );
    KPlainBuffer( int MS = -1 );
    virtual ~KPlainBuffer();

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

    virtual void setModified( bool M = true );

    virtual int find( const char*, int Length, int Pos = 0 ) const;
    virtual int rfind( const char*, int Length, int Pos = -1 ) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

  public:
    void setReadOnly( bool RO = true );
    void setMaxSize( int MS );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    void setKeepsMemory( int KM = true );

  public:
    char *data() const;
    int maxSize() const;
    /** returns whether the memory of the byte array is kept */
    bool keepsMemory() const;

  protected:
    /** */
    char *Data;
    /** size of the data */
    int Size;
    /** Size of data array */
    int RawSize;
    /** maximal size of array, unlimited if -1 */
    int MaxSize;
    /** flag whether the initially given memory should be kept */
    bool KeepsMemory:1;
    /**  */
    bool ReadOnly:1;
    /** */
    bool Modified:1;
};


inline bool KPlainBuffer::prepareRange( KSection )     const { return true; }
inline const char *KPlainBuffer::dataSet( KSection S ) const { return &Data[S.start()]; }
inline char KPlainBuffer::datum( int Offset )          const { return Data[Offset]; }
inline int KPlainBuffer::size()                        const { return Size; }

inline bool KPlainBuffer::isReadOnly()   const { return ReadOnly; }
inline bool KPlainBuffer::isModified()   const { return Modified; }

inline void KPlainBuffer::setReadOnly( bool RO )   { ReadOnly = RO; }
inline void KPlainBuffer::setModified( bool M )    { Modified = M; }
inline void KPlainBuffer::setMaxSize( int MS )     { MaxSize = MS; }
inline void KPlainBuffer::setKeepsMemory( int KM ) { KeepsMemory = KM; }

inline char *KPlainBuffer::data()       const { return Data; }
inline int KPlainBuffer::maxSize()      const { return MaxSize; }
inline bool KPlainBuffer::keepsMemory() const { return KeepsMemory; }
}

#endif
