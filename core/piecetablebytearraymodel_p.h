/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_PIECETABLEBYTEARRAYMODEL_P_H
#define OKTETA_PIECETABLEBYTEARRAYMODEL_P_H

// lib
#include "piecetablebytearraymodel.h"
#include "bookmarksconstiterator.h"
#include "bookmarklistconstiteratoradapter.h"
#include "bookmarklist.h"
#include "changesdatastorage.h"
#include "arraychangemetricslist.h"
// piecetable
#include "piecetable/revertablepiecetable.h"


namespace Okteta
{

class PieceTableByteArrayModelPrivate
{
  public:
    /**  */
    explicit PieceTableByteArrayModelPrivate( PieceTableByteArrayModel* parent, const QByteArray& data );
    /** creates a writeable buffer which is deleted at the end */
    explicit PieceTableByteArrayModelPrivate( PieceTableByteArrayModel* parent, int size, Byte fillByte = '\0' );

    ~PieceTableByteArrayModelPrivate();

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
    void setReadOnly( bool isReadOnly = true );

  public: // Versionable API
    int versionIndex() const;
    int versionCount() const;
    QString versionDescription( int versionIndex ) const;

  public: // set/action
    void revertToVersionByIndex( int versionIndex );

  public:
    void addBookmarks( const QList<Bookmark>& bookmarks );
    void removeBookmarks( const QList<Bookmark>& bookmarks );
    void removeAllBookmarks();
    void setBookmark( unsigned int index, const Bookmark& bookmark );

    BookmarksConstIterator createBookmarksConstIterator() const;
    const Bookmark& bookmarkAt( unsigned int index ) const;
    const Bookmark& bookmarkFor( int offset ) const;
    bool containsBookmarkFor( int offset ) const;
    unsigned int bookmarksCount() const;

  public: // ChangesDescribable API
    void openGroupedChange( const QString& description );
    void cancelGroupedChange();
    void closeGroupedChange( const QString& description );

  public: // ChangeHistory API
    QList<ByteArrayChange> changes( int firstVersionIndex, int lastVersionIndex ) const;
    const QByteArray& initialData() const;
    void doChanges( const QList<Okteta::ByteArrayChange>& changes,
                    int oldVersionIndex, int newVersionIndex );

  public:
    void setData( const QByteArray& data );

  protected:
    void doInsertChange( Address offset, const Byte* insertData, int insertLength );
    void doRemoveChange( const AddressRange& removeRange );
    void doReplaceChange( const AddressRange& removeRange, const Byte* insertData, int insertLength );
    void doSwapChange( Address firstStart, const AddressRange& secondRange );
    void doFillChange( Address offset, Size filledLength,
                       Byte fillByte, int fillLength );

    void beginChanges();
    void endChanges();

  protected: // data
    PieceTableByteArrayModel *p;
    /**  */
    bool mReadOnly :1;

    QByteArray mInitialData;
    KPieceTable::RevertablePieceTable mPieceTable;
    ChangesDataStorage mChangesDataStorage;
    /** */
    BookmarkList mBookmarks;
    /** temporary workaround for cancelling groups. If -1 no group is opened. */
    int mBeforeGroupedChangeVersionIndex;

    int mBeforeChangesVersionIndex;
    ArrayChangeMetricsList mChangeMetrics;
    QList<ByteArrayChange> mChanges;
    bool mBeforeChangesModified:1;
    bool mBookmarksModified:1;
};


inline const QByteArray& PieceTableByteArrayModelPrivate::initialData() const { return mInitialData; }
inline Size PieceTableByteArrayModelPrivate::size() const  { return mPieceTable.size(); }

inline bool PieceTableByteArrayModelPrivate::isReadOnly()   const { return mReadOnly; }
inline bool PieceTableByteArrayModelPrivate::isModified()   const { return !mPieceTable.isAtBase(); }

inline void PieceTableByteArrayModelPrivate::setReadOnly( bool readOnly )
{
    if( mReadOnly != readOnly )
    {
        mReadOnly = readOnly;
        emit p->readOnlyChanged( readOnly );
    }
}
inline void PieceTableByteArrayModelPrivate::setModified( bool modified )
{
    if( isModified() != modified )
    {
        mPieceTable.setBeforeCurrentChangeAsBase(modified);
        // TODO: is the call setModified of any use?
        // shouldn't there be only a setUnmodified(void) or else call?
        emit p->modifiedChanged( modified );
    }
}

inline int PieceTableByteArrayModelPrivate::versionIndex() const { return mPieceTable.appliedChangesCount(); }
inline int PieceTableByteArrayModelPrivate::versionCount() const { return mPieceTable.changesCount()+1; }
inline QString PieceTableByteArrayModelPrivate::versionDescription( int versionIndex ) const
{ return mPieceTable.changeDescription( versionIndex-1 ); }

inline void PieceTableByteArrayModelPrivate::addBookmarks( const QList<Bookmark> &bookmarks )
{
    mBookmarks.addBookmarks( bookmarks );
    emit p->bookmarksAdded( bookmarks );
}
inline void PieceTableByteArrayModelPrivate::removeBookmarks( const QList<Bookmark> &bookmarks )
{
    mBookmarks.removeBookmarks( bookmarks );
    emit p->bookmarksRemoved( bookmarks );
}

inline void PieceTableByteArrayModelPrivate::removeAllBookmarks()
{
    const QList<Bookmark> bookmarks = mBookmarks.list();
    mBookmarks.clear();
    emit p->bookmarksRemoved( bookmarks );
}
inline void PieceTableByteArrayModelPrivate::setBookmark( unsigned int index, const Bookmark& bookmark )
{
    mBookmarks.setBookmark( index, bookmark );

    QList<int> changedBookmarkIndizes;
    changedBookmarkIndizes.append( index );
    emit p->bookmarksModified( changedBookmarkIndizes );
}

inline BookmarksConstIterator PieceTableByteArrayModelPrivate::createBookmarksConstIterator() const
{
    return BookmarksConstIterator( new BookmarkListConstIteratorAdapter(mBookmarks) );
}

inline const Bookmark& PieceTableByteArrayModelPrivate::bookmarkAt( unsigned int index ) const
{
    return mBookmarks.at( index );
}
inline const Bookmark& PieceTableByteArrayModelPrivate::bookmarkFor( int offset ) const
{
    return mBookmarks.bookmark( offset );
}
inline bool PieceTableByteArrayModelPrivate::containsBookmarkFor( int offset ) const { return mBookmarks.contains( offset ); }
inline unsigned int PieceTableByteArrayModelPrivate::bookmarksCount() const { return mBookmarks.size(); }

}

#endif
