/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_PIECETABLEBYTEARRAYMODEL_P_HPP
#define OKTETA_PIECETABLEBYTEARRAYMODEL_P_HPP

// lib
#include "abstractbytearraymodel_p.hpp"
#include "piecetablebytearraymodel.hpp"
#include "bookmarksconstiterator.hpp"
#include "bookmarklistconstiteratoradapter.hpp"
#include "bookmarklist.hpp"
#include "changesdatastorage.hpp"
#include "arraychangemetricslist.hpp"
// piecetable
#include "piecetable/revertablepiecetable.hpp"

namespace Okteta {

class PieceTableByteArrayModelPrivate : public AbstractByteArrayModelPrivate
{
public:
    /**  */
    explicit PieceTableByteArrayModelPrivate(PieceTableByteArrayModel* parent, const QByteArray& data);
    /** creates a writeable buffer which is deleted at the end */
    explicit PieceTableByteArrayModelPrivate(PieceTableByteArrayModel* parent, int size, Byte fillByte = '\0');
    PieceTableByteArrayModelPrivate() = delete;

    ~PieceTableByteArrayModelPrivate() override;

public: // AbstractByteArrayModel API
    Byte byte(Address offset) const;
    Size size() const;
    bool isReadOnly() const;
    bool isModified() const;

    Size insert(Address offset, const Byte* insertData, int insertLength);
    Size remove(const AddressRange& removeRange);
    Size replace(const AddressRange& removeRange, const Byte* insertData, int insertLength);
    bool swap(Address firstStart, const AddressRange& secondRange);
    Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1);
    void setByte(Address offset, Byte byte);

    void setModified(bool modified = true);
    void setReadOnly(bool isReadOnly = true);

public: // Versionable API
    int versionIndex() const;
    int versionCount() const;
    QString versionDescription(int versionIndex) const;

public: // set/action
    void revertToVersionByIndex(int versionIndex);

public:
    void addBookmarks(const QVector<Bookmark>& bookmarks);
    void removeBookmarks(const QVector<Bookmark>& bookmarks);
    void removeAllBookmarks();
    void setBookmark(unsigned int index, const Bookmark& bookmark);

    BookmarksConstIterator createBookmarksConstIterator() const;
    const Bookmark& bookmarkAt(unsigned int index) const;
    const Bookmark& bookmarkFor(int offset) const;
    bool containsBookmarkFor(int offset) const;
    unsigned int bookmarksCount() const;

public: // ChangesDescribable API
    void openGroupedChange(const QString& description);
    void cancelGroupedChange();
    void closeGroupedChange(const QString& description);

public: // ChangeHistory API
    QVector<ByteArrayChange> changes(int firstVersionIndex, int lastVersionIndex) const;
    const QByteArray& initialData() const;
    void doChanges(const QVector<Okteta::ByteArrayChange>& changes,
                   int oldVersionIndex, int newVersionIndex);

public:
    void setData(const QByteArray& data);

private:
    void doInsertChange(Address offset, const Byte* insertData, int insertLength);
    void doRemoveChange(const AddressRange& removeRange);
    void doReplaceChange(const AddressRange& removeRange, const Byte* insertData, int insertLength);
    void doSwapChange(Address firstStart, const AddressRange& secondRange);
    void doFillChange(Address offset, Size filledLength,
                      Byte fillByte, int fillLength);

    void beginChanges();
    void endChanges();

private: // data
    /**  */
    bool mReadOnly : 1;

    QByteArray mInitialData;
    KPieceTable::RevertablePieceTable mPieceTable;
    ChangesDataStorage mChangesDataStorage;
    /** */
    BookmarkList mBookmarks;
    /** temporary workaround for cancelling groups. If -1 no group is opened. */
    int mBeforeGroupedChangeVersionIndex;

    int mBeforeChangesVersionIndex;
    ArrayChangeMetricsList mChangeMetrics;
    QVector<ByteArrayChange> mChanges;
    bool mBeforeChangesModified : 1;
    bool mBookmarksModified : 1;

private:
    Q_DECLARE_PUBLIC(PieceTableByteArrayModel)
};

inline const QByteArray& PieceTableByteArrayModelPrivate::initialData() const { return mInitialData; }
inline Size PieceTableByteArrayModelPrivate::size() const { return mPieceTable.size(); }

inline bool PieceTableByteArrayModelPrivate::isReadOnly()   const { return mReadOnly; }
inline bool PieceTableByteArrayModelPrivate::isModified()   const { return !mPieceTable.isAtBase(); }

inline void PieceTableByteArrayModelPrivate::setReadOnly(bool readOnly)
{
    Q_Q(PieceTableByteArrayModel);

    if (mReadOnly != readOnly) {
        mReadOnly = readOnly;
        emit q->readOnlyChanged(readOnly);
    }
}
inline void PieceTableByteArrayModelPrivate::setModified(bool modified)
{
    Q_Q(PieceTableByteArrayModel);

    if (isModified() != modified) {
        mPieceTable.setBeforeCurrentChangeAsBase(modified);
        // TODO: is the call setModified of any use?
        // shouldn't there be only a setUnmodified(void) or else call?
        emit q->modifiedChanged(modified);
    }
}

inline int PieceTableByteArrayModelPrivate::versionIndex() const { return mPieceTable.appliedChangesCount(); }
inline int PieceTableByteArrayModelPrivate::versionCount() const { return mPieceTable.changesCount() + 1; }
inline QString PieceTableByteArrayModelPrivate::versionDescription(int versionIndex) const
{ return mPieceTable.changeDescription(versionIndex - 1); }

inline void PieceTableByteArrayModelPrivate::addBookmarks(const QVector<Bookmark>& bookmarks)
{
    Q_Q(PieceTableByteArrayModel);

    mBookmarks.addBookmarks(bookmarks);
    emit q->bookmarksAdded(bookmarks);
}
inline void PieceTableByteArrayModelPrivate::removeBookmarks(const QVector<Bookmark>& bookmarks)
{
    Q_Q(PieceTableByteArrayModel);

    mBookmarks.removeBookmarks(bookmarks);
    emit q->bookmarksRemoved(bookmarks);
}

inline void PieceTableByteArrayModelPrivate::removeAllBookmarks()
{
    Q_Q(PieceTableByteArrayModel);

    const QVector<Bookmark> bookmarks = mBookmarks.list();
    mBookmarks.clear();
    emit q->bookmarksRemoved(bookmarks);
}
inline void PieceTableByteArrayModelPrivate::setBookmark(unsigned int index, const Bookmark& bookmark)
{
    Q_Q(PieceTableByteArrayModel);

    mBookmarks.setBookmark(index, bookmark);

    const QVector<int> changedBookmarkIndizes {
        static_cast<int>(index)
    };
    emit q->bookmarksModified(changedBookmarkIndizes);
}

inline BookmarksConstIterator PieceTableByteArrayModelPrivate::createBookmarksConstIterator() const
{
    return BookmarksConstIterator(new BookmarkListConstIteratorAdapter(mBookmarks));
}

inline const Bookmark& PieceTableByteArrayModelPrivate::bookmarkAt(unsigned int index) const
{
    return mBookmarks.at(index);
}
inline const Bookmark& PieceTableByteArrayModelPrivate::bookmarkFor(int offset) const
{
    return mBookmarks.bookmark(offset);
}
inline bool PieceTableByteArrayModelPrivate::containsBookmarkFor(int offset) const { return mBookmarks.contains(offset); }
inline unsigned int PieceTableByteArrayModelPrivate::bookmarksCount() const { return mBookmarks.size(); }

}

#endif
