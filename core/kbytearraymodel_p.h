/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#ifndef KHE_CORE_KBYTEARRAYMODEL_P_H
#define KHE_CORE_KBYTEARRAYMODEL_P_H


// lib
#include "kbytearraymodel.h"
#include "kbookmarksconstiterator.h"
#include "bookmarklistconstiteratoradapter.h"
#include "bookmarklist.h"
// 
#include <arraychangemetricslist.h>


namespace KHECore
{

class KByteArrayModelPrivate
{
  public:
    KByteArrayModelPrivate( KByteArrayModel *parent, char *D, unsigned int S, int RS, bool KM );
    KByteArrayModelPrivate( KByteArrayModel *parent, const char *D, unsigned int S );
    KByteArrayModelPrivate( KByteArrayModel *parent, int S, int MS );
    ~KByteArrayModelPrivate();

  public:
    char datum( unsigned int offset ) const;
    int size() const;
    bool isReadOnly() const;
    bool isModified() const;

    int insert( int at, const char *data, int length );
    int remove( const Section &section );
    unsigned int replace( const Section &before, const char *after, unsigned int afterLength );
    bool swap( int firstStart, const Section &secondSection );
    int fill( const char fillChar, unsigned int from = 0, int length = -1 );
    void setDatum( unsigned int offset, const char datum );

    void setModified( bool modified = true );

    int indexOf( const char *searchString, int length, int from = 0 ) const;
//     virtual int find( const char*KeyData, int length, const Section &Section ) const;
    int lastIndexOf( const char *searchString, int length, int from = -1 ) const;

  public:
    void setReadOnly( bool readOnly = true );
    void setMaxSize( int maxSize );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    void setKeepsMemory( bool keepsMemory = true );
    void setAutoDelete( bool autoDelete = true );
    void setData( char *data, unsigned int size, int rawSize = -1, bool keepMemory = true );

  public:
    char *data() const;
    int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
    bool keepsMemory() const;
    bool autoDelete() const;

  public:
    void addBookmarks( const QList<KHECore::Bookmark> &bookmarks );
    void removeBookmarks( const QList<KHECore::Bookmark> &bookmarks );
    void removeAllBookmarks();
    void setBookmark( unsigned int index, const KHECore::Bookmark& bookmark );

    KHECore::BookmarksConstIterator createBookmarksConstIterator() const;
    const KHECore::Bookmark& bookmarkAt( unsigned int index ) const;
    const KHECore::Bookmark& bookmarkFor( int offset ) const;
    bool containsBookmarkFor( int offset ) const;
    unsigned int bookmarksCount() const;

  protected:
    /** resizes the buffer, if possible, saving the data and splitting the data, if demanded
     * @param AddSize additional size the buffer should grow
     * @param SplitPos if -1 does not split
     * @param SaveUpperPart true if upper part should be copied into new buffer
     * @return additional size the buffer has grown
     */
    int addSize( int AddSize, int SplitPos = -1, bool SaveUpperPart = true );

  protected:
    KByteArrayModel *p;
    /** */
    char *m_data;
    /** size of the data */
    unsigned int m_size;
    /** m_size of data array */
    unsigned int m_rawSize;
    /** maximal size of array, unlimited if -1 */
    int m_maxSize;
    /** flag whether the initially given memory should be kept */
    bool m_keepsMemory:1;
    /** flag whether the  */
    bool m_autoDelete:1;
    /**  */
    bool m_readOnly:1;
    /** */
    bool m_modified:1;
    /** */
    BookmarkList m_bookmarks;
};


inline KByteArrayModelPrivate::~KByteArrayModelPrivate() { if( m_autoDelete ) delete m_data; }

inline char KByteArrayModelPrivate::datum( unsigned int Offset ) const { return m_data[Offset]; }
inline int KByteArrayModelPrivate::size()                        const { return m_size; }

inline bool KByteArrayModelPrivate::isReadOnly()   const { return m_readOnly; }
inline bool KByteArrayModelPrivate::isModified()   const { return m_modified; }

inline void KByteArrayModelPrivate::setReadOnly( bool isReadOnly )
{
    if( m_readOnly != isReadOnly )
    {
        m_readOnly = isReadOnly;
        emit p->readOnlyChanged( isReadOnly );
    }
}
inline void KByteArrayModelPrivate::setMaxSize( int MS )      { m_maxSize = MS; }
inline void KByteArrayModelPrivate::setKeepsMemory( bool KM ) { m_keepsMemory = KM; }
inline void KByteArrayModelPrivate::setAutoDelete( bool AD )  { m_autoDelete = AD; }
inline void KByteArrayModelPrivate::setDatum( unsigned int offset, const char datum )
{
    m_data[offset] = datum;
    m_modified = true;
    emit p->contentsChanged( KHE::ArrayChangeMetricsList::oneReplacement(offset,1,1) );
    emit p->modificationChanged( true );
}
inline void KByteArrayModelPrivate::setModified( bool modified )
{
    m_modified = modified;
    emit p->modificationChanged( m_modified );
}



inline char *KByteArrayModelPrivate::data()       const { return m_data; }
inline int KByteArrayModelPrivate::maxSize()      const { return m_maxSize; }
inline bool KByteArrayModelPrivate::keepsMemory() const { return m_keepsMemory; }
inline bool KByteArrayModelPrivate::autoDelete()  const { return m_autoDelete; }

inline void KByteArrayModelPrivate::addBookmarks( const QList<KHECore::Bookmark> &bookmarks )
{
    m_bookmarks.addBookmarks( bookmarks );
    emit p->bookmarksAdded( bookmarks );
}
inline void KByteArrayModelPrivate::removeBookmarks( const QList<KHECore::Bookmark> &bookmarks )
{
    m_bookmarks.removeBookmarks( bookmarks );
    emit p->bookmarksRemoved( bookmarks );
}

inline void KByteArrayModelPrivate::removeAllBookmarks()
{
    QList<KHECore::Bookmark> bookmarks = m_bookmarks.list();
    m_bookmarks.clear();
    emit p->bookmarksRemoved( bookmarks );
}
inline void KByteArrayModelPrivate::setBookmark( unsigned int index, const KHECore::Bookmark& bookmark )
{
    m_bookmarks.setBookmark( index, bookmark );

    QList<int> changedBookmarkIndizes;
    changedBookmarkIndizes.append( index );
    emit p->bookmarksModified( changedBookmarkIndizes );
}

inline KHECore::BookmarksConstIterator KByteArrayModelPrivate::createBookmarksConstIterator() const
{
    return BookmarksConstIterator( new BookmarkListConstIteratorAdapter(m_bookmarks) );
}

inline const KHECore::Bookmark& KByteArrayModelPrivate::bookmarkFor( int offset ) const
{
    return m_bookmarks.bookmark( offset );
}
inline const KHECore::Bookmark& KByteArrayModelPrivate::bookmarkAt( unsigned int index ) const
{
    return m_bookmarks.at( index );
}
inline bool KByteArrayModelPrivate::containsBookmarkFor( int offset ) const { return m_bookmarks.contains( offset ); }
inline unsigned int KByteArrayModelPrivate::bookmarksCount() const { return m_bookmarks.size(); }

}

#endif
