/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filebytearraymodel_p.hpp"

namespace Okteta {

FileByteArrayModelPrivate::FileByteArrayModelPrivate(FileByteArrayModel* parent, int pageNumber, int pageSize)
    : AbstractByteArrayModelPrivate(parent)
    , mNoOfUsedPages(pageNumber)
    , mNoOfFreePages(pageNumber)
    , mPageSize(pageSize)
{
    mIsOpen = false;

//   if( !filename.empty() )
//     open(filename);
}

FileByteArrayModelPrivate::~FileByteArrayModelPrivate()
{
    if (mFile.isOpen()) {
        std::ignore = close();
    }
}

Byte FileByteArrayModelPrivate::byte(Address offset) const
{
//   std::cout << "reading datum " << offset << std::endl;
    int offsetInPage = offset - mOffsetOfActualPage;
    // there shouldn't be any need to check l
    if (offsetInPage >= 0 && offsetInPage < (int)mPageSize) {
        return mActualPage[offsetInPage];
    }

    // load the page
    unsigned int pageIndex = offset / mPageSize;
    std::ignore = ensurePageLoaded(pageIndex);

    return mActualPage[offset - mOffsetOfActualPage];
}

bool FileByteArrayModelPrivate::open(const QString& fileName)
{
    // clear old data
    if (isOpen() && !close()) { // only occurs if close somehow fails.
        return false;
    }

    mFile.setFileName(fileName);
    if (!mFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    int fileSize = mFile.size();
    mSize = fileSize;

    // calculate necessary number of pages
    int noOfPages = fileSize / mPageSize + 1;

    // initialize Page pointers
    mData.resize(noOfPages);

    mFirstPage = mLastPage = 0;

    return ensurePageLoaded(0);
}

bool FileByteArrayModelPrivate::close()
{
    if (!isOpen()) {
        return false;
    }

    mFile.close();

    if (mFile.error() != QFile::NoError) {
        return false;
    }

    // free pages
    for (auto& pageData : mData) {
        pageData.reset();
    }

    mFirstPage = mLastPage = -1;
    mNoOfFreePages = mNoOfUsedPages;

    return true;
}

bool FileByteArrayModelPrivate::ensurePageLoaded(unsigned int pageIndex) const
{
    if (!isOpen()) {
        return false;
    }
    // page loaded?
    if (mData[pageIndex]) {
        mActualPage = mData[pageIndex].get();
        mOffsetOfActualPage = pageIndex * mPageSize;
        return true;
    }

    // no page available?
    if (mNoOfFreePages < 1) {
        // free the page which is the furthest away from the page we are loading
        if (std::abs(mFirstPage - (long)pageIndex) > std::abs(mLastPage - (long)pageIndex)) {
            while (!freePage(mFirstPage++)) {
                ;
            }
        } else {
            while (!freePage(mLastPage--)) {
                ;
            }
        }
    }

    // create Page
    mData[pageIndex] = std::unique_ptr<char[]>(new char[mPageSize]); // no make_unique, no need for initialization
    --mNoOfFreePages;

    // jump to position and read the page's data in
    bool success = mFile.seek((unsigned long)(pageIndex * mPageSize));
    if (success) {
        success = mFile.read(mData[pageIndex].get(), mPageSize) > 0;
    }

    if (success) {
        // correct bounds
        if ((int)pageIndex < mFirstPage) {
            mFirstPage = pageIndex;
        }

        if ((int)pageIndex > mLastPage) {
            mLastPage = pageIndex;
        }

        mActualPage = mData[pageIndex].get();
        mOffsetOfActualPage = pageIndex * mPageSize;
    }

    return success;
}

bool FileByteArrayModelPrivate::freePage(unsigned int pageIndex) const
{
    // check range and if is loaded at all
    if (pageIndex >= static_cast<unsigned int>(mData.size()) || !mData[pageIndex]) {
        return false;
    }

    mData[pageIndex].reset();
    ++mNoOfFreePages;
    return true;
}

}
