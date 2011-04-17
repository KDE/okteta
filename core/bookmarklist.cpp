/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bookmarklist.h"

// Qt
#include <QtCore/QList>


namespace Okteta
{

BookmarkList::BookmarkList() {}

void BookmarkList::addBookmark( const Bookmark &bookmark )
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

void BookmarkList::addBookmarks( const QList<Okteta::Bookmark> &bookmarks )
{
    foreach( const Bookmark &bookmark, bookmarks )
        addBookmark( bookmark );
}

void BookmarkList::removeBookmark( const Bookmark &bookmark )
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

void BookmarkList::removeBookmarks( const QList<Okteta::Bookmark> &bookmarks )
{
    foreach( const Bookmark &bookmark, bookmarks )
        removeBookmark( bookmark );
}

void BookmarkList::setBookmark( unsigned int index, const Bookmark& bookmark )
{
    const Iterator endIt = end();
    unsigned int i = 0;
    for( Iterator it = begin(); it!=endIt; ++it,++i )
    {
        if( i == index )
        {
            *it = bookmark;
            break;
        }
    }
}

bool BookmarkList::adjustToReplaced( Address offset, Size removedLength, Size insertedLength )
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
    const Size diff = insertedLength - removedLength;
    if( diff != 0 )
    for( ; bIt!=end(); ++bIt )
    {
        (*bIt).move( diff );
        result = true;
    }

    return result;
}

bool BookmarkList::adjustToSwapped( Address firstPartStart, Address secondPartStart, Size secondPartLength )
{
    bool result = false;

    iterator bIt = begin();
    while( bIt!=end() && bIt->offset() < firstPartStart )
        ++bIt;
    QList<Okteta::Bookmark> bookmarksInFirstPart;
    // take bookmarks from first part
    while( bIt!=end() && bIt->offset() < secondPartStart )
    {
        bookmarksInFirstPart.append( *bIt );
        bIt = erase( bIt );
    }
    // move bookmarks from second to first
    const Size diff = firstPartStart - secondPartStart;
    const Address behindLast = secondPartStart + secondPartLength;
    for( ; bIt!=end() && bIt->offset() < behindLast; ++bIt )
    {
        (*bIt).move( diff );
        result = true;
    }
    // append bookmarks from first part as second
    if( !bookmarksInFirstPart.isEmpty() )
    {
        foreach( Bookmark bookmark, bookmarksInFirstPart ) // krazy:exclude=foreach
        {
            bookmark.move( secondPartLength );
            insert( bIt, bookmark );
        }
        result = true;
    }

    return result;
}

QList<Okteta::Bookmark> BookmarkList::list() const
{
    QList<Okteta::Bookmark> result;

    foreach( const Bookmark &bookmark, *this )
        result.append( bookmark );

    return result;
}

const Bookmark& BookmarkList::bookmark( Address offset ) const
{
    const ConstIterator endIt = end();
    for( ConstIterator it = begin(); it!=endIt; ++it )
    {
        if( it->offset() == offset )
            return *it;
    }
    static const Bookmark* const noBookmark = 0;

    return (const Bookmark&)*noBookmark;
}

bool BookmarkList::contains( Address offset ) const
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

const Bookmark& BookmarkList::at( unsigned int index ) const
{
    Q_ASSERT_X( (int)index < size(), "BookmarkList::at", "index out of range" );

    const ConstIterator endIt = end();
    unsigned int i = 0;
    for( ConstIterator it = begin(); it!=endIt; ++it,++i )
    {
        if( i == index )
            return *it;
    }
    static const Bookmark* const noBookmark = 0;
    return (const Bookmark&)*noBookmark;
}

BookmarkList::~BookmarkList() {}

}
