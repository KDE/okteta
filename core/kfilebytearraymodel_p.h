/***************************************************************************
                          kfilebytearraymodel_p.h  -  description
                             -------------------
    copyright            : (C) 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_KFILEBYTEARRAYMODEL_P_H
#define KHE_CORE_KFILEBYTEARRAYMODEL_P_H

// Qt
#include <QtCore/QVector>
#include <QtCore/QFile>


namespace KHECore
{

class KFileByteArrayModelPrivate
{
    typedef QVector<char *> KPageOfChar;

  public:
    KFileByteArrayModelPrivate( int pageNumber, int pageSize );
    ~KFileByteArrayModelPrivate();

  public:
    int size() const;
    char datum( unsigned int datumOffset ) const;
    bool isReadOnly() const;
    void setReadOnly( bool readonly );

  public:
    bool isOpen() const;
    bool open( const QString &fileName );
    bool close();

  protected:
    bool ensurePageLoaded( unsigned int PageIndex ) const;
    bool freePage( unsigned int PageIndex ) const;


  protected:
    /** */
    mutable QFile m_file;
    /**  */
    bool m_readOnly:1;
    bool m_isOpen:1;
    bool m_atEOF:1;
    /** maximum number of pages which could be currently loaded */
    unsigned int m_noOfUsedPages;
    /**  number of actually not used pages (in terms of NoOfUsedPages) */
    mutable int m_noOfFreePages;
    /** number of bytes in a page */
    unsigned int m_pageSize;
    /** first currently loaded page */
    mutable int m_firstPage;
    /** last currently loaded page */
    mutable int m_lastPage;
    /** */
    mutable KPageOfChar m_data;
    /** */
    unsigned int m_size;

    /** current offset */
    mutable unsigned int m_offsetOfActualPage;
    /** points to the actual page */
    mutable char* m_actualPage;
};

inline int KFileByteArrayModelPrivate::size()        const   { return m_size; }
inline bool KFileByteArrayModelPrivate::isReadOnly() const   { return m_readOnly; }
inline void KFileByteArrayModelPrivate::setReadOnly( bool readonly ) { m_readOnly = readonly; }

inline bool KFileByteArrayModelPrivate::isOpen() const { return m_file.isOpen(); }

}

#endif
