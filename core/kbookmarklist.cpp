/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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
    iterator bIt = begin();
    while( bIt!=end() && bIt->offset() < offset )
        ++bIt;
    // remove bookmarks in removed section
    while( bIt!=end() && bIt->offset() < offset+removedLength )
    {
        bIt = erase( bIt );
        result = true;
    }
    // adjust bookmarks in moved section
    const int diff = insertedLength - removedLength;
    if( diff != 0 )
    for( ; bIt!=end(); ++bIt )
    {
        (*bIt).move( diff );
        result = true;
    }
    return result;
}

bool KBookmarkList::adjustToSwapped( int firstPartStart, int secondPartStart, int secondPartLength )
{
    bool result = false;
    iterator bIt = begin();
    while( bIt!=end() && bIt->offset() < firstPartStart )
        ++bIt;
    QList<KHECore::KBookmark> bookmarksInFirstPart;
    // take bookmarks from first part
    while( bIt!=end() && bIt->offset() < secondPartStart )
    {
        bookmarksInFirstPart.append( *bIt );
        bIt = erase( bIt );
    }
    // move bookmarks from second to first
    const int diff = firstPartStart - secondPartStart;
    const int behindLast = secondPartStart + secondPartLength;
    for( ; bIt!=end() && bIt->offset() < behindLast; ++bIt )
    {
        (*bIt).move( diff );
        result = true;
    }
    // append bookmarks from first part as second
    if( !bookmarksInFirstPart.isEmpty() )
    {
        foreach( KBookmark bookmark, bookmarksInFirstPart )
        {
            bookmark.move( secondPartLength );
            insert( bIt, bookmark );
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

bool KBookmarkList::contains( int offset ) const
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
