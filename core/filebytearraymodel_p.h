/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2003,2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILEBYTEARRAYMODEL_P_H
#define FILEBYTEARRAYMODEL_P_H

// Qt
#include <QtCore/QVector>
#include <QtCore/QFile>


namespace Okteta
{

class FileByteArrayModelPrivate
{
    typedef QVector<char*> KPageOfChar;

  public:
    FileByteArrayModelPrivate( int pageNumber, int pageSize );
    ~FileByteArrayModelPrivate();

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

inline int FileByteArrayModelPrivate::size()        const   { return m_size; }
inline bool FileByteArrayModelPrivate::isReadOnly() const   { return m_readOnly; }
inline void FileByteArrayModelPrivate::setReadOnly( bool readonly ) { m_readOnly = readonly; }

inline bool FileByteArrayModelPrivate::isOpen() const { return m_file.isOpen(); }

}

#endif
