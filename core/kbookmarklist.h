/***************************************************************************
                          kbookmarklist.h  -  description
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


#ifndef KHE_CORE_KBOOKMARKLIST_H
#define KHE_CORE_KBOOKMARKLIST_H

// lib
#include "kbookmark.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QLinkedList>


namespace KHECore {

/**
@author Friedrich W. H.  Kossebau
*/
class KHECORE_EXPORT KBookmarkList : public QLinkedList<KBookmark>
{
  public:
    KBookmarkList();
    ~KBookmarkList();

  public:
    void addBookmark( const KBookmark &bookmark );
    void addBookmarks( const QList<KHECore::KBookmark> &bookmarks );
    void removeBookmark( const KBookmark &bookmark );
    void removeBookmarks( const QList<KHECore::KBookmark> &bookmarks );

    bool adjustToReplaced( int offset, int removedLength, int insertedLength );
    bool adjustToMoved( int firstPartStart, int secondPartStart, int secondPartLength );

  public:
    bool contains( int offset ) const;
    using QLinkedList<KBookmark>::contains;
    const_iterator previousFrom( int offset ) const;
    const_iterator nextFrom( int offset ) const;
//     const_iterator begin( int offset ) const;
//     using QLinkedList<KBookmark>::begin;
    QList<KHECore::KBookmark> list() const;
};

}

#endif
