/***************************************************************************
                          kbytearraymodel.h  -  description
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


#ifndef KHE_KBYTEARRAYMODEL_H
#define KHE_KBYTEARRAYMODEL_H

#include "kabstractbytearraymodel.h"

namespace KHECore
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

class KByteArrayModel : public KAbstractByteArrayModel
{
  friend class KPlainBufferIterator;

  public:
    KByteArrayModel( char *D, unsigned int S, int RS = -1, bool KM = true );
    KByteArrayModel( const char *D, unsigned int S );
    KByteArrayModel( int S=0, int MS = -1 );
    virtual ~KByteArrayModel();

  public: // KDataBuffer API
    //virtual KDataBufferIterator *iterator() const;
    virtual const char *dataSet( const KSection &S ) const;
    virtual char datum( unsigned int Offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int Pos, const char*, int Length );
    virtual int remove( const KSection &Remove );
    virtual unsigned int replace( const KSection &Remove, const char*, unsigned int InputLength );
    virtual int move( int DestPos, const KSection &SourceSection );
    virtual int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 );
    virtual void setDatum( unsigned int Offset, const char Char );

    virtual void setModified( bool M = true );

    //virtual int find( const char*, int Length, int Pos = 0 ) const;
    virtual int find( const char*KeyData, int Length, const KSection &Section ) const;
    virtual int rfind( const char*, int Length, int Pos = -1 ) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

  public:
    void setReadOnly( bool RO = true );
    void setMaxSize( int MS );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    void setKeepsMemory( bool KM = true );
    void setAutoDelete( bool AD = true );

  public:
    char *data() const;
    int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
    bool keepsMemory() const;
    bool autoDelete() const;

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
    /** flag whether the  */
    bool AutoDelete:1;
    /**  */
    bool ReadOnly:1;
    /** */
    bool Modified:1;
};


inline const char *KByteArrayModel::dataSet( const KSection &S ) const { return &Data[S.start()]; }
inline char KByteArrayModel::datum( unsigned int Offset ) const { return Data[Offset]; }
inline int KByteArrayModel::size()                        const { return Size; }

inline bool KByteArrayModel::isReadOnly()   const { return ReadOnly; }
inline bool KByteArrayModel::isModified()   const { return Modified; }

inline void KByteArrayModel::setReadOnly( bool RO )    { ReadOnly = RO; }
inline void KByteArrayModel::setModified( bool M )     { Modified = M; }
inline void KByteArrayModel::setMaxSize( int MS )      { MaxSize = MS; }
inline void KByteArrayModel::setKeepsMemory( bool KM ) { KeepsMemory = KM; }
inline void KByteArrayModel::setAutoDelete( bool AD )  { AutoDelete = AD; }

inline char *KByteArrayModel::data()       const { return Data; }
inline int KByteArrayModel::maxSize()      const { return MaxSize; }
inline bool KByteArrayModel::keepsMemory() const { return KeepsMemory; }
inline bool KByteArrayModel::autoDelete()  const { return AutoDelete; }
}

#endif
