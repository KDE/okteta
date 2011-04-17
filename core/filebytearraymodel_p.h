/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003,2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_FILEBYTEARRAYMODEL_P_H
#define OKTETA_FILEBYTEARRAYMODEL_P_H

// lib
#include "filebytearraymodel.h"
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
    Byte byte( Address offset ) const;
    Size size() const;
    bool isReadOnly() const;
    void setReadOnly( bool readonly );

  public:
    bool isOpen() const;
    bool open( const QString& fileName );
    bool close();

  protected:
    bool ensurePageLoaded( unsigned int pageIndex ) const;
    bool freePage( unsigned int pageIndex ) const;


  protected:
    /** */
    mutable QFile mFile;
    /**  */
    bool mReadOnly :1;
    bool mIsOpen :1;
    bool mAtEof :1;
    /** maximum number of pages which could be currently loaded */
    unsigned int mNoOfUsedPages;
    /**  number of actually not used pages (in terms of NoOfUsedPages) */
    mutable int mNoOfFreePages;
    /** number of bytes in a page */
    unsigned int mPageSize;
    /** first currently loaded page */
    mutable int mFirstPage;
    /** last currently loaded page */
    mutable int mLastPage;
    /** */
    mutable KPageOfChar mData;
    /** */
    int mSize;

    /** current offset */
    mutable unsigned int mOffsetOfActualPage;
    /** points to the actual page */
    mutable char* mActualPage;
};


inline Size FileByteArrayModelPrivate::size()        const   { return mSize; }
inline bool FileByteArrayModelPrivate::isReadOnly() const   { return mReadOnly; }
inline void FileByteArrayModelPrivate::setReadOnly( bool readonly ) { mReadOnly = readonly; }

inline bool FileByteArrayModelPrivate::isOpen() const { return mFile.isOpen(); }

}

#endif
