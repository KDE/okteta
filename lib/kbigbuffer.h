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


#ifndef KBIGBUFFER_H
#define KBIGBUFFER_H

// qt specific
#include <qvaluevector.h>
#include <qfile.h>
// lib specific
#include "kdatabuffer.h"

namespace KHE {

/** base class for all Data buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class KBigBuffer : public KDataBuffer
{
    typedef QValueVector<char *> KPageOfChar;

  public:
    /** default is only 50*4k = 200k memory image */
    KBigBuffer( int NP = 50, int PS = 4096 );
    virtual ~KBigBuffer();

  public: // KDataBuffer API
    /** locates working range, returns false if failed */
    virtual bool prepareRange( KSection Range ) const;
    /** expects pointer to memory, should be in prepared range */
    virtual const char *dataSet( KSection S ) const;
    /** */
    virtual char datum( int Offset ) const;
    /** */
    virtual int size() const;
    /** is the buffer changeable? */
    virtual bool isReadOnly() const;
    /** has the buffer been modified? */
    virtual bool isModified() const;

    /** inserts at Position */
    virtual int insert( int Pos, const char*, int Length );
    /** removes beginning with position as much as possible, returns length of removed */
    virtual int remove( KSection S );
    /** replaces as much as possible, returns length of substituted */
    virtual int replace( KSection S, const char*, int InputLength );

    virtual void setModified( bool M = true );

    /** searches beginning with byte at Pos, returns -1 if nothing found */
    virtual int find( const char*, int Length, int Pos = 0 ) const;
    /** searches backward beginning with byte at Pos, returns -1 if nothing found */
    virtual int rfind( const char*, int Length, int Pos = -1 ) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

  public:
    void setReadOnly( bool RO = true );
    bool isOpen() const;
    bool open (const QString& filename );
    bool close();

  protected:
    bool ensurePageLoaded( int PageIndex ) const;
    bool freePage( int PageIndex ) const;


  protected:
    /** */
    mutable QFile File;
    /**  */
    bool ReadOnly:1;
    bool IsOpen:1;
    bool AtEOF:1;
    /** maximum number of pages which could be currently loaded */
    int NoOfUsedPages;
    /**  number of actually not used pages (in terms of NoOfUsedPages) */
    mutable int NoOfFreePages;
    /** number of bytes in a page */
    int PageSize;
    /** first currently loaded page */
    mutable int FirstPage;
    /** last currently loaded page */
    mutable int LastPage;
    /** */
    mutable KPageOfChar Data;
    /** */
    int Size;

    /** current offset */
    mutable int OffsetOfActualPage;
    /** points to the actual page */
    mutable char* ActualPage;
};

inline int KBigBuffer::size()        const   { return Size; }
inline bool KBigBuffer::isReadOnly() const   { return ReadOnly; }
inline bool KBigBuffer::isModified() const   { return false; }
inline void KBigBuffer::setReadOnly( bool RO ) { ReadOnly = RO; }
inline void KBigBuffer::setModified( bool )  {}

inline bool KBigBuffer::isOpen() const { return File.isOpen(); }

}

#endif
