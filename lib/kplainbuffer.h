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


#ifndef KHE_KPLAINBUFFER_H
#define KHE_KPLAINBUFFER_H

#include "kdatabuffer.h"

namespace KHE
{
/*
class KPlainBufferIterator : public KDataBufferIterator
{
}
*/
/** base class for all Data buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class KPlainBuffer : public KDataBuffer
{
  friend class KPlainBufferIterator;

  public:
    KPlainBuffer( char *D, unsigned int S, int RS = -1, bool KM = true );
    KPlainBuffer( const char *D, unsigned int S );
    KPlainBuffer( int S=0, int MS = -1 );
    virtual ~KPlainBuffer();

  public: // KDataBuffer API
    virtual bool prepareRange( KSection Range ) const;
    //virtual KDataBufferIterator *iterator() const;
    virtual const char *dataSet( KSection S ) const;
    virtual char datum( unsigned int Offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int Pos, const char*, int Length );
    virtual int remove( KSection Remove );
    virtual unsigned int replace( KSection Remove, const char*, unsigned int InputLength );
    virtual int move( int DestPos, KSection SourceSection );
    virtual int fill( const char FillChar, int Length = -1, unsigned int Pos = 0 );
    virtual void setDatum( unsigned int Offset, const char Char );

    virtual void setModified( bool M = true );

    //virtual int find( const char*, int Length, int Pos = 0 ) const;
    virtual int find( const char*KeyData, int Length, KSection Section ) const;
    virtual int rfind( const char*, int Length, int Pos = -1 ) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

  public:
    void setReadOnly( bool RO = true );
    void setMaxSize( int MS );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    void setKeepsMemory( bool KM = true );

  public:
    char *data() const;
    int maxSize() const;
    /** returns whether the memory of the byte array is kept */
    bool keepsMemory() const;

  protected:
    /** resizes the buffer, if possible, saving the data and splitting the data, if demanded
     * @param AddSize additional size the buffer should grow
     * @param SplitPos if -1 does not split
     * @param SaveUpperPart true if upper part should be copied into new buffer
     * @return additional size the buffer has grown
     */
    int addSize( int AddSize, int SplitPos = -1, bool SaveUpperPart = true );

  protected:
    /** */
    char *Data;
    /** size of the data */
    unsigned int Size;
    /** Size of data array */
    unsigned int RawSize;
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
inline char KPlainBuffer::datum( unsigned int Offset ) const { return Data[Offset]; }
inline int KPlainBuffer::size()                        const { return Size; }

inline bool KPlainBuffer::isReadOnly()   const { return ReadOnly; }
inline bool KPlainBuffer::isModified()   const { return Modified; }

inline void KPlainBuffer::setDatum( unsigned int Offset, const char Char )
{ Data[Offset] = Char; Modified = true; }

inline void KPlainBuffer::setReadOnly( bool RO )    { ReadOnly = RO; }
inline void KPlainBuffer::setModified( bool M )     { Modified = M; }
inline void KPlainBuffer::setMaxSize( int MS )      { MaxSize = MS; }
inline void KPlainBuffer::setKeepsMemory( bool KM ) { KeepsMemory = KM; }

inline char *KPlainBuffer::data()       const { return Data; }
inline int KPlainBuffer::maxSize()      const { return MaxSize; }
inline bool KPlainBuffer::keepsMemory() const { return KeepsMemory; }
}

#endif
