/***************************************************************************
                          kbigbuffer.h  -  description
                             -------------------
    begin                : Mit Jun 02 2003
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


#ifndef KHE_KBIGBUFFER_H
#define KHE_KBIGBUFFER_H

// qt specific
#include <qvaluevector.h>
#include <qfile.h>
// lib specific
#include "kdatabuffer.h"
#include "khexedit_export.h"

namespace KHE {

/** base class for all Data buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class KHEXEDIT_EXPORT KBigBuffer : public KDataBuffer
{
    typedef QValueVector<char *> KPageOfChar;

  public:
    /** default is only 50*4k = 200k memory image */
    KBigBuffer( int NP = 50, int PS = 4096 );
    virtual ~KBigBuffer();

  public: // KDataBuffer API
    virtual bool prepareRange( KSection Range ) const;
    virtual const char *dataSet( KSection S ) const;
    virtual char datum( unsigned int Offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int Pos, const char*, int Length );
    virtual int remove( KSection S );
    virtual unsigned int replace( KSection S, const char*, unsigned int InputLength );
    virtual int move( int DestPos, KSection SourceSection );
    virtual int fill( char FillChar, int Length = -1, unsigned int Pos = 0 );
    virtual void setDatum( unsigned int Offset, const char Char );

    virtual void setModified( bool M = true );

    //virtual int find( const char*, int Length, int Pos = 0 ) const;
    virtual int find( const char*KeyData, int Length, KSection Section ) const;
    virtual int rfind( const char*, int Length, int Pos = -1 ) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

  public:
    void setReadOnly( bool RO = true );
    bool isOpen() const;
    bool open (const QString& filename );
    bool close();

  protected:
    bool ensurePageLoaded( unsigned int PageIndex ) const;
    bool freePage( unsigned int PageIndex ) const;


  protected:
    /** */
    mutable QFile File;
    /**  */
    bool ReadOnly:1;
    bool IsOpen:1;
    bool AtEOF:1;
    /** maximum number of pages which could be currently loaded */
    unsigned int NoOfUsedPages;
    /**  number of actually not used pages (in terms of NoOfUsedPages) */
    mutable int NoOfFreePages;
    /** number of bytes in a page */
    unsigned int PageSize;
    /** first currently loaded page */
    mutable int FirstPage;
    /** last currently loaded page */
    mutable int LastPage;
    /** */
    mutable KPageOfChar Data;
    /** */
    unsigned int Size;

    /** current offset */
    mutable unsigned int OffsetOfActualPage;
    /** points to the actual page */
    mutable char* ActualPage;
};

inline int KBigBuffer::size()        const   { return Size; }
inline bool KBigBuffer::isReadOnly() const   { return ReadOnly; }
inline bool KBigBuffer::isModified() const   { return false; }
inline void KBigBuffer::setReadOnly( bool RO ) { ReadOnly = RO; }
inline void KBigBuffer::setModified( bool )  {}

inline void KBigBuffer::setDatum( unsigned int, const char )  {}

inline bool KBigBuffer::isOpen() const { return File.isOpen(); }

}

#endif
