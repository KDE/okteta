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

#include "filebytearraymodel_p.h"

// C
#include <stdlib.h>


namespace Okteta
{

FileByteArrayModelPrivate::FileByteArrayModelPrivate( int pageNumber, int pageSize )
 : mNoOfUsedPages( pageNumber ),
   mNoOfFreePages( pageNumber ),
   mPageSize( pageSize ),
   mFirstPage( -1 ),
   mLastPage( -1 ),
   mSize( 0 )
{
    mIsOpen = false;

//   if( !filename.empty() )
//     open(filename);
}


Byte FileByteArrayModelPrivate::byte( Address offset ) const
{
//   std::cout << "reading datum " << offset << std::endl;
    int offsetInPage = offset - mOffsetOfActualPage;
    // there shouldn't be any need to check l
    if( offsetInPage >= 0 && offsetInPage < (int)mPageSize )
        return mActualPage[offsetInPage];

    // load the page
    unsigned int pageIndex = offset / mPageSize;
    ensurePageLoaded( pageIndex );

    return mActualPage[offset-mOffsetOfActualPage];
}


bool FileByteArrayModelPrivate::open( const QString& fileName )
{
    // clear old data
    if( isOpen() && !close() ) // only occurs if close somehow fails.
        return false;

    mFile.setFileName( fileName );
    if( !mFile.open(QIODevice::ReadOnly) )
        return false;

    int fileSize = mFile.size();
    mSize = fileSize;

    // calculate necessary number of pages
    int noOfPages = fileSize/mPageSize + 1;

    // initialize Page pointers
    mData.resize( noOfPages );
    for( KPageOfChar::iterator D=mData.begin(); D!=mData.end(); ++D )
        *D = 0;

    mFirstPage = mLastPage = 0;

    return ensurePageLoaded( 0 );
}


bool FileByteArrayModelPrivate::close()
{
    if( !isOpen() )
        return false;

    mFile.close();

    if( mFile.error() != QFile::NoError )
        return false;

    // free pages
    for( KPageOfChar::iterator D=mData.begin(); D!=mData.end(); ++D )
        delete [] *D;

    mFirstPage = mLastPage = -1;
    mNoOfFreePages = mNoOfUsedPages;

    return true;
}


bool FileByteArrayModelPrivate::ensurePageLoaded( unsigned int pageIndex ) const
{
    if( !isOpen() )
        return false;
    // page loaded?
    if( mData[pageIndex] != 0 )
    {
        mActualPage = mData[pageIndex];
        mOffsetOfActualPage = pageIndex * mPageSize;
        return true;
    }

    // no page available?
    if( mNoOfFreePages < 1 )
    {
        // free the page which is the furthest away from the page we are loading
        if( abs(mFirstPage-(long)pageIndex) > abs(mLastPage-(long)pageIndex) )
            while( !freePage(mFirstPage++) ) {
                ;
            }
        else
            while( !freePage(mLastPage--) ) {
                ;
            }
    }

    // create Page
    mData[pageIndex] = new char[mPageSize];
    --mNoOfFreePages;

    // jump to position and read the page's data in
    bool success = mFile.seek( (unsigned long)(pageIndex*mPageSize) );
    if( success )
        success = mFile.read( mData[pageIndex], mPageSize ) > 0;

    if( success )
    {
        // correct bounds
        if( (int)pageIndex < mFirstPage )
            mFirstPage = pageIndex;

        if( (int)pageIndex > mLastPage )
            mLastPage = pageIndex;

        mActualPage = mData[pageIndex];
        mOffsetOfActualPage = pageIndex * mPageSize;
    }

    return success;
}


bool FileByteArrayModelPrivate::freePage( unsigned int pageIndex ) const
{
    // check range and if is loaded at all
    if( (int)pageIndex >= mData.size() || !mData[pageIndex] )
        return false;

    delete [] mData[pageIndex];
    mData[pageIndex] = 0;
    ++mNoOfFreePages;
    return true;
}


FileByteArrayModelPrivate::~FileByteArrayModelPrivate()
{
    if( mFile.isOpen() )
        close();
}

}
