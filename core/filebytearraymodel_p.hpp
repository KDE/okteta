/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_FILEBYTEARRAYMODEL_P_HPP
#define OKTETA_FILEBYTEARRAYMODEL_P_HPP

// lib
#include "abstractbytearraymodel_p.hpp"
#include "filebytearraymodel.hpp"
// Qt
#include <QVector>
#include <QFile>

namespace Okteta {

class FileByteArrayModelPrivate : public AbstractByteArrayModelPrivate
{
    using KPageOfChar = QVector<char*>;

public:
    FileByteArrayModelPrivate(FileByteArrayModel* parent, int pageNumber, int pageSize);
    FileByteArrayModelPrivate() = delete;

    ~FileByteArrayModelPrivate() override;

public:
    Byte byte(Address offset) const;
    Size size() const;
    bool isReadOnly() const;
    void setReadOnly(bool readonly);

public:
    bool isOpen() const;
    bool open(const QString& fileName);
    bool close();

private:
    bool ensurePageLoaded(unsigned int pageIndex) const;
    bool freePage(unsigned int pageIndex) const;

private:
    /** */
    mutable QFile mFile;
    /**  */
    bool mReadOnly : 1;
    bool mIsOpen : 1;
    /** maximum number of pages which could be currently loaded */
    unsigned int mNoOfUsedPages;
    /**  number of actually not used pages (in terms of NoOfUsedPages) */
    mutable int mNoOfFreePages;
    /** number of bytes in a page */
    unsigned int mPageSize;
    /** first currently loaded page */
    mutable int mFirstPage = -1;
    /** last currently loaded page */
    mutable int mLastPage = -1;
    /** */
    mutable KPageOfChar mData;
    /** */
    int mSize = 0;

    /** current offset */
    mutable unsigned int mOffsetOfActualPage;
    /** points to the actual page */
    mutable char* mActualPage;
};

inline Size FileByteArrayModelPrivate::size()        const { return mSize; }
inline bool FileByteArrayModelPrivate::isReadOnly() const { return mReadOnly; }
inline void FileByteArrayModelPrivate::setReadOnly(bool readonly) { mReadOnly = readonly; }

inline bool FileByteArrayModelPrivate::isOpen() const { return mFile.isOpen(); }

}

#endif
