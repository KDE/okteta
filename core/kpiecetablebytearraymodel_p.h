/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_CORE_KPIECETABLEBYTEARRAYMODEL_PRIVATE_H
#define KHE_CORE_KPIECETABLEBYTEARRAYMODEL_PRIVATE_H

// lib
#include "kpiecetablebytearraymodel.h"
#include "changesdatastorage.h"
// piecetable
#include "piecetable/revertablepiecetable.h"


namespace KHECore
{

/** base class for all mInitialData buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class KPieceTableByteArrayModel::Private
{
  public:
    /** creates a readonly buffer around the given data */
    Private( KPieceTableByteArrayModel *parent, const char *data, unsigned int size,
             bool careForMemory = true );
    /** creates a writeable buffer which is deleted at the end */
    explicit Private( KPieceTableByteArrayModel *parent, unsigned int size, char fillByte = '\0' );
    ~Private();

  public: // KAbstractByteArrayModel API
    char datum( unsigned int offset ) const;
    int size() const;
    bool isReadOnly() const;
    bool isModified() const;

    int insert( int offset, const char *insertData, int insertLength );
    int remove( const KSection &removeSection );
    unsigned int replace( const KSection &removeSection, const char *insertData, unsigned int insertLength );
    bool swap( int firstStart, const KSection &secondSection );
    int fill( const char fillByte, unsigned int Pos = 0, int fillLength = -1 );
    void setDatum( unsigned int offset, const char byte );

    void setModified( bool modified = true );
    void setReadOnly( bool readonly = true );

  public: // Versionable API
    int versionIndex() const;
    int versionCount() const;
    QString versionDescription( int versionIndex ) const;

  public: // set/action
    void revertToVersionByIndex( int versionIndex );

  public:
    void addBookmarks( const QList<KHECore::KBookmark> &bookmarks );
    void removeBookmarks( const QList<KHECore::KBookmark> &bookmarks );
    void removeAllBookmarks();

    KHECore::KBookmarkList bookmarkList() const;

  public: // ChangesDescribable API
    void openGroupedChange( const QString &description );
    void cancelGroupedChange();
    void closeGroupedChange( const QString &description );

  public: // ChangeHistory API
    QList<ByteArrayChange> changes( int firstVersionIndex, int lastVersionIndex ) const;
    QByteArray initialData() const;
    void doChanges( const QList<KHECore::ByteArrayChange>& changes,
                    int oldVersionIndex, int newVersionIndex );

  public:
    void setData( const char *data, unsigned int size, bool careForMemory = true );

  protected:
    KPieceTableByteArrayModel *p;
    /**  */
    bool mReadOnly:1;
    /** */
    bool mAutoDelete:1;

    const char *mInitialData;
    int mInitialSize;
    KPieceTable::RevertablePieceTable mPieceTable;
    ChangesDataStorage mChangesDataStorage;
    /** */
    KBookmarkList mBookmarks;
    /** temporary workaround for cancelling groups. If -1 no group is opened. */
    int mBeforeGroupedChangeVersionIndex;
};


inline int KPieceTableByteArrayModel::Private::size() const  { return mPieceTable.size(); }

inline bool KPieceTableByteArrayModel::Private::isReadOnly()   const { return mReadOnly; }
inline bool KPieceTableByteArrayModel::Private::isModified()   const { return !mPieceTable.isAtBase(); }

inline void KPieceTableByteArrayModel::Private::setReadOnly( bool readOnly )
{
    if( mReadOnly != readOnly )
    {
        mReadOnly = readOnly;
        emit p->readOnlyChanged( readOnly );
    }
}
inline void KPieceTableByteArrayModel::Private::setModified( bool modified )
{
    if( isModified() != modified )
    {
        mPieceTable.setBeforeCurrentChangeAsBase(modified);
        // TODO: is the call setModified of any use?
        // shouldn't there be only a setUnmodified(void) or else call?
        emit p->modificationChanged( modified );
    }
}

inline int KPieceTableByteArrayModel::Private::versionIndex() const { return mPieceTable.appliedChangesCount(); }
inline int KPieceTableByteArrayModel::Private::versionCount() const { return mPieceTable.changesCount()+1; }
inline QString KPieceTableByteArrayModel::Private::versionDescription( int versionIndex ) const
{ return mPieceTable.changeDescription( versionIndex-1 ); }

inline void KPieceTableByteArrayModel::Private::addBookmarks( const QList<KHECore::KBookmark> &bookmarks )
{
    mBookmarks.addBookmarks( bookmarks );
    emit p->bookmarksAdded( bookmarks );
}
inline void KPieceTableByteArrayModel::Private::removeBookmarks( const QList<KHECore::KBookmark> &bookmarks )
{
    mBookmarks.removeBookmarks( bookmarks );
    emit p->bookmarksRemoved( bookmarks );
}

inline void KPieceTableByteArrayModel::Private::removeAllBookmarks()
{
    const QList<KHECore::KBookmark> bookmarks = mBookmarks.list();
    mBookmarks.clear();
    emit p->bookmarksRemoved( bookmarks );
}

inline KHECore::KBookmarkList KPieceTableByteArrayModel::Private::bookmarkList() const { return mBookmarks; }

}

#endif
