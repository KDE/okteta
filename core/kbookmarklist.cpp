/***************************************************************************
                          kbookmarklist.cpp  -  description
                             -------------------
    begin                : Fri Nov 30 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "kbookmarklist.h"


namespace KHECore
{

KBookmarkList::KBookmarkList() {}

void KBookmarkList::addBookmark( const KBookmark &bookmark )
{
    if( !bookmark.isValid() )
       return;

    iterator B = begin();
    for( ; B!=end(); ++B )
    {
        // new bookmark before next bookmark?
        if( bookmark.offset() < B->offset() )
        {
            // put the new before it
            insert( B, bookmark );
            return;
        }

        // bookmark already present?
        if( bookmark.offset() == B->offset() )
        {
            *B = bookmark;
            return;
        }
    }

    // all others before the new?
    if( B == end() )
        // add it at the end
        append( bookmark );
}

void KBookmarkList::addBookmarks( const QList<KHECore::KBookmark> &bookmarks )
{
    foreach( KBookmark bookmark, bookmarks )
        addBookmark( bookmark );
}

void KBookmarkList::removeBookmark( const KBookmark &bookmark )
{
    if( !bookmark.isValid() )
        return;

    iterator B = begin();
    for( ; B!=end(); ++B )
    {
        if( bookmark.offset() == B->offset() )
        {
            erase( B );
            break;
        }
    }
}

void KBookmarkList::removeBookmarks( const QList<KHECore::KBookmark> &bookmarks )
{
    foreach( KBookmark bookmark, bookmarks )
        removeBookmark( bookmark );
}

bool KBookmarkList::adjustToReplaced( int offset, int removedLength, int insertedLength )
{
    bool result = false;
    iterator bit = begin();
    while( bit!=end() && bit->offset() < offset )
        ++bit;
    // remove bookmarks in removed section
    while( bit!=end() && bit->offset() < offset+removedLength )
    {
        bit = erase( bit );
        result = true;
    }
    // adjust bookmarks in moved section
    const int diff = insertedLength - removedLength;
    if( diff != 0 )
    for( ; bit!=end(); ++bit )
    {
        (*bit).move( diff );
        result = true;
    }
    return result;
}

bool KBookmarkList::adjustToMoved( int firstPartStart, int secondPartStart, int secondPartLength )
{
    bool result = false;
    iterator bit = begin();
    while( bit!=end() && bit->offset() < firstPartStart )
        ++bit;
    QList<KHECore::KBookmark> bookmarksInFirstPart;
    // take bookmarks from first part
    while( bit!=end() && bit->offset() < secondPartStart )
    {
        bookmarksInFirstPart.append( *bit );
        bit = erase( bit );
    }
    // move bookmarks from second to first
    const int diff = firstPartStart - secondPartStart;
    const int behindLast = secondPartStart + secondPartLength;
    for( ; bit!=end() && bit->offset() < behindLast; ++bit )
    {
        (*bit).move( diff );
        result = true;
    }
    // append bookmarks from first part as second
    if( !bookmarksInFirstPart.isEmpty() )
    {
        foreach( KBookmark bookmark, bookmarksInFirstPart )
        {
            bookmark.move( secondPartLength );
            insert( bit, bookmark );
        }
        result = true;
    }

    return result;
}

QList<KHECore::KBookmark> KBookmarkList::list() const
{
    QList<KHECore::KBookmark> result;
    foreach( KBookmark bookmark, *this )
        result.append( bookmark );
    return result;
}

bool KBookmarkList::includes( int offset ) const
{
    bool result = false;
    const_iterator B = begin();
    for( ; B!=end(); ++B )
    {
        if( B->offset() == offset )
        {
            result = true;
            break;
        }
    }
    return result;
}

KBookmarkList::const_iterator KBookmarkList::previousFrom( int offset ) const
{
    const_iterator b = begin();
    const_iterator result = end();

    for( ; b!=end(); ++b )
    {
        if( offset < b->offset() )
            break;
        result = b;
    }
    return result;
}

KBookmarkList::const_iterator KBookmarkList::nextFrom( int offset ) const
{
    const_iterator b = begin();
    for( ; b!=end(); ++b )
    {
        if( offset <= b->offset() )
            break;
    }
    return b;
}

KBookmarkList::~KBookmarkList() {}

}
