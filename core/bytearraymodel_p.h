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

#ifndef OKTETA_BYTEARRAYMODEL_P_H
#define OKTETA_BYTEARRAYMODEL_P_H


// lib
#include "bytearraymodel.h"
#include "bookmarksconstiterator.h"
#include "bookmarklistconstiteratoradapter.h"
#include "bookmarklist.h"
// 
#include <arraychangemetricslist.h>


namespace Okteta
{

class ByteArrayModelPrivate
{
  public:
    ByteArrayModelPrivate( ByteArrayModel* parent, Byte* data, int size, int rawSize, bool keepsMemory );
    ByteArrayModelPrivate( ByteArrayModel* parent, const Byte* data, int size );
    ByteArrayModelPrivate( ByteArrayModel* parent, int size, int maxSize );

    ~ByteArrayModelPrivate();

  public: // AbstractByteArrayModel API
    Byte byte( Address offset ) const;
    Size size() const;
    bool isReadOnly() const;
    bool isModified() const;

    Size insert( Address offset, const Byte* insertData, int insertLength );
    Size remove( const AddressRange& removeRange );
    Size replace( const AddressRange& removeRange, const Byte* insertData, int insertLength );
    bool swap( Address firstStart, const AddressRange& secondRange );
    Size fill( Byte fillByte, Address offset = 0, Size fillLength = -1 );
    void setByte( Address offset, Byte byte );

    void setModified( bool modified = true );

  public:
    void setReadOnly( bool readOnly = true );
    void setMaxSize( int maxSize );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    void setKeepsMemory( bool keepsMemory = true );
    void setAutoDelete( bool autoDelete = true );
    void setData( Byte* data, int size, int rawSize = -1, bool keepsMemory = true );

  public:
    Byte* data() const;
    int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
    bool keepsMemory() const;
    bool autoDelete() const;

  public: // Bookmarkable API
    void addBookmarks( const QList<Bookmark>& bookmarks );
    void removeBookmarks( const QList<Bookmark>& bookmarks );
    void removeAllBookmarks();
    void setBookmark( unsigned int index, const Bookmark& bookmark );

    BookmarksConstIterator createBookmarksConstIterator() const;
    const Bookmark& bookmarkAt( unsigned int index ) const;
    const Bookmark& bookmarkFor( int offset ) const;
    bool containsBookmarkFor( int offset ) const;
    unsigned int bookmarksCount() const;

  protected:
    /** resizes the buffer, if possible, saving the data and splitting the data, if demanded
     * @param AddSize additional size the buffer should grow
     * @param splitOffset if -1 does not split
     * @param saveUpperPart true if upper part should be copied into new buffer
     * @return additional size the buffer has grown
     */
    int addSize( int AddSize, int splitOffset = -1, bool saveUpperPart = true );

  protected:
    ByteArrayModel* p;
    /** */
    Byte* mData;
    /** size of the data */
    int mSize;
    /** mSize of data array */
    int mRawSize;
    /** maximal size of array, unlimited if -1 */
    int mMaxSize;
    /** flag whether the initially given memory should be kept */
    bool mKeepsMemory :1;
    /** flag whether the  */
    bool mAutoDelete :1;
    /**  */
    bool mReadOnly :1;
    /** */
    bool mModified :1;
    /** */
    BookmarkList m_bookmarks;
};


inline ByteArrayModelPrivate::~ByteArrayModelPrivate() { if( mAutoDelete ) delete mData; }

inline Byte ByteArrayModelPrivate::byte( Address offset ) const { return mData[offset]; }
inline Size ByteArrayModelPrivate::size()                 const { return mSize; }

inline bool ByteArrayModelPrivate::isReadOnly()   const { return mReadOnly; }
inline bool ByteArrayModelPrivate::isModified()   const { return mModified; }

inline void ByteArrayModelPrivate::setReadOnly( bool isReadOnly )
{
    if( mReadOnly != isReadOnly )
    {
        mReadOnly = isReadOnly;
        emit p->readOnlyChanged( isReadOnly );
    }
}
inline void ByteArrayModelPrivate::setMaxSize( int maxSize )          { mMaxSize = maxSize; }
inline void ByteArrayModelPrivate::setKeepsMemory( bool keepsMemory ) { mKeepsMemory = keepsMemory; }
inline void ByteArrayModelPrivate::setAutoDelete( bool autoDelete )   { mAutoDelete = autoDelete; }
inline void ByteArrayModelPrivate::setByte( Address offset, Byte byte )
{
    const bool wasModifiedBefore = mModified;
    mData[offset] = byte;
    mModified = true;
    emit p->contentsChanged( ArrayChangeMetricsList::oneReplacement(offset,1,1) );
    if( ! wasModifiedBefore )
        emit p->modifiedChanged( true );
}
inline void ByteArrayModelPrivate::setModified( bool modified )
{
    mModified = modified;
    emit p->modifiedChanged( mModified );
}



inline Byte* ByteArrayModelPrivate::data()       const { return mData; }
inline int ByteArrayModelPrivate::maxSize()      const { return mMaxSize; }
inline bool ByteArrayModelPrivate::keepsMemory() const { return mKeepsMemory; }
inline bool ByteArrayModelPrivate::autoDelete()  const { return mAutoDelete; }

inline void ByteArrayModelPrivate::addBookmarks( const QList<Bookmark> &bookmarks )
{
    m_bookmarks.addBookmarks( bookmarks );
    emit p->bookmarksAdded( bookmarks );
}
inline void ByteArrayModelPrivate::removeBookmarks( const QList<Bookmark> &bookmarks )
{
    m_bookmarks.removeBookmarks( bookmarks );
    emit p->bookmarksRemoved( bookmarks );
}

inline void ByteArrayModelPrivate::removeAllBookmarks()
{
    QList<Bookmark> bookmarks = m_bookmarks.list();
    m_bookmarks.clear();
    emit p->bookmarksRemoved( bookmarks );
}
inline void ByteArrayModelPrivate::setBookmark( unsigned int index, const Bookmark& bookmark )
{
    m_bookmarks.setBookmark( index, bookmark );

    QList<int> changedBookmarkIndizes;
    changedBookmarkIndizes.append( index );
    emit p->bookmarksModified( changedBookmarkIndizes );
}

inline BookmarksConstIterator ByteArrayModelPrivate::createBookmarksConstIterator() const
{
    return BookmarksConstIterator( new BookmarkListConstIteratorAdapter(m_bookmarks) );
}

inline const Bookmark& ByteArrayModelPrivate::bookmarkFor( int offset ) const
{
    return m_bookmarks.bookmark( offset );
}
inline const Bookmark& ByteArrayModelPrivate::bookmarkAt( unsigned int index ) const
{
    return m_bookmarks.at( index );
}
inline bool ByteArrayModelPrivate::containsBookmarkFor( int offset ) const { return m_bookmarks.contains( offset ); }
inline unsigned int ByteArrayModelPrivate::bookmarksCount() const { return m_bookmarks.size(); }

}

#endif
