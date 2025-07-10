/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYMODEL_P_HPP
#define OKTETA_BYTEARRAYMODEL_P_HPP

// lib
#include "abstractbytearraymodel_p.hpp"
#include "bytearraymodel.hpp"
#include "bookmarksconstiterator.hpp"
#include "bookmarklistconstiteratoradapter.hpp"
#include "bookmarklist.hpp"
#include "arraychangemetricslist.hpp"
// Std
#include <memory>

namespace Okteta {

class ByteArrayModelPrivate : public AbstractByteArrayModelPrivate
{
public:
    ByteArrayModelPrivate(ByteArrayModel* parent, Byte* data, int size, int rawSize, bool keepsMemory);
    ByteArrayModelPrivate(ByteArrayModel* parent, const Byte* data, int size);
    ByteArrayModelPrivate(ByteArrayModel* parent, int size, int maxSize);

    ~ByteArrayModelPrivate() override;

public: // AbstractByteArrayModel API
    [[nodiscard]]
    Byte byte(Address offset) const;
    [[nodiscard]]
    Size size() const;
    [[nodiscard]]
    bool isReadOnly() const;
    [[nodiscard]]
    bool isModified() const;

    [[nodiscard]]
    Size insertBytes(Address offset, const Byte* insertData, int insertLength);
    [[nodiscard]]
    Size removeBytes(const AddressRange& removeRange);
    [[nodiscard]]
    Size replaceBytes(const AddressRange& removeRange, const Byte* insertData, int insertLength);
    [[nodiscard]]
    bool swapBytes(Address firstStart, const AddressRange& secondRange);
    [[nodiscard]]
    Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1);
    void setByte(Address offset, Byte byte);

    void setModified(bool modified = true);

public:
    void setReadOnly(bool readOnly = true);
    void setMaxSize(int maxSize);
    /** sets whether the memory given by setData or in the constructor should be kept on resize
     */
    void setKeepsMemory(bool keepsMemory = true);
    void setAutoDelete(bool autoDelete = true);
    void setData(Byte* data, int size, int rawSize = -1, bool keepMemory = true);

public:
    [[nodiscard]]
    Byte* data() const;
    [[nodiscard]]
    int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
    [[nodiscard]]
    bool keepsMemory() const;
    // TODO 0.10: turn this bool flag into a real flag which also tells how to free the memory if "autodeleted"
    // perhaps merge keepsMemory flag into that new flag as well
    [[nodiscard]]
    bool autoDelete() const;

public: // Bookmarkable API
    void addBookmarks(const QVector<Bookmark>& bookmarks);
    void removeBookmarks(const QVector<Bookmark>& bookmarks);
    void removeAllBookmarks();
    void setBookmark(unsigned int index, const Bookmark& bookmark);

    [[nodiscard]]
    BookmarksConstIterator createBookmarksConstIterator() const;
    [[nodiscard]]
    const Bookmark& bookmarkAt(unsigned int index) const;
    [[nodiscard]]
    const Bookmark& bookmarkFor(Address offset) const;
    [[nodiscard]]
    bool containsBookmarkFor(Address offset) const;
    [[nodiscard]]
    unsigned int bookmarksCount() const;

private:
    /** resizes the buffer, if possible, saving the data and splitting the data, if demanded
     * @param AddSize additional size the buffer should grow
     * @param splitOffset if -1 does not split
     * @param saveUpperPart true if upper part should be copied into new buffer
     * @return additional size the buffer has grown
     */
    int addSize(int AddSize, int splitPosition = -1, bool saveUpperPart = true);

private:
    /** */
    std::unique_ptr<Byte[]> mData;
    /** size of the data */
    int mSize;
    /** mSize of data array */
    int mRawSize;
    /** maximal size of array, unlimited if -1 */
    int mMaxSize = -1;
    /** flag whether the initially given memory should be kept */
    bool mKeepsMemory : 1;
    /** flag whether the  */
    bool mAutoDelete : 1;
    /**  */
    bool mReadOnly : 1;
    /** */
    bool mModified : 1;
    /** */
    BookmarkList m_bookmarks;

private:
    Q_DECLARE_PUBLIC(ByteArrayModel)
};

inline ByteArrayModelPrivate::~ByteArrayModelPrivate()
{
    if (!mAutoDelete) {
        mData.release();
    }
}

inline Byte ByteArrayModelPrivate::byte(Address offset) const { return mData[offset]; }
inline Size ByteArrayModelPrivate::size()                 const { return mSize; }

inline bool ByteArrayModelPrivate::isReadOnly()   const { return mReadOnly; }
inline bool ByteArrayModelPrivate::isModified()   const { return mModified; }

inline void ByteArrayModelPrivate::setReadOnly(bool isReadOnly)
{
    Q_Q(ByteArrayModel);

    if (mReadOnly != isReadOnly) {
        mReadOnly = isReadOnly;
        Q_EMIT q->readOnlyChanged(isReadOnly);
    }
}
inline void ByteArrayModelPrivate::setMaxSize(int maxSize)          { mMaxSize = maxSize; }
inline void ByteArrayModelPrivate::setKeepsMemory(bool keepsMemory) { mKeepsMemory = keepsMemory; }
inline void ByteArrayModelPrivate::setAutoDelete(bool autoDelete)   { mAutoDelete = autoDelete; }
inline void ByteArrayModelPrivate::setByte(Address offset, Byte byte)
{
    Q_Q(ByteArrayModel);

    const bool wasModifiedBefore = mModified;
    mData[offset] = byte;
    mModified = true;
    Q_EMIT q->contentsChanged(ArrayChangeMetricsList::oneReplacement(offset, 1, 1));
    if (!wasModifiedBefore) {
        Q_EMIT q->modifiedChanged(true);
    }
}
inline void ByteArrayModelPrivate::setModified(bool modified)
{
    Q_Q(ByteArrayModel);

    mModified = modified;
    Q_EMIT q->modifiedChanged(mModified);
}

inline Byte* ByteArrayModelPrivate::data()       const { return mData.get(); }
inline int ByteArrayModelPrivate::maxSize()      const { return mMaxSize; }
inline bool ByteArrayModelPrivate::keepsMemory() const { return mKeepsMemory; }
inline bool ByteArrayModelPrivate::autoDelete()  const { return mAutoDelete; }

inline void ByteArrayModelPrivate::addBookmarks(const QVector<Bookmark>& bookmarks)
{
    Q_Q(ByteArrayModel);

    m_bookmarks.addBookmarks(bookmarks);
    Q_EMIT q->bookmarksAdded(bookmarks);
}
inline void ByteArrayModelPrivate::removeBookmarks(const QVector<Bookmark>& bookmarks)
{
    Q_Q(ByteArrayModel);

    m_bookmarks.removeBookmarks(bookmarks);
    Q_EMIT q->bookmarksRemoved(bookmarks);
}

inline void ByteArrayModelPrivate::removeAllBookmarks()
{
    Q_Q(ByteArrayModel);

    const QVector<Bookmark> bookmarks = m_bookmarks.list();
    m_bookmarks.clear();
    Q_EMIT q->bookmarksRemoved(bookmarks);
}
inline void ByteArrayModelPrivate::setBookmark(unsigned int index, const Bookmark& bookmark)
{
    Q_Q(ByteArrayModel);

    m_bookmarks.setBookmark(index, bookmark);

    const QVector<int> changedBookmarkIndizes {
        static_cast<int>(index)
    };
    Q_EMIT q->bookmarksModified(changedBookmarkIndizes);
}

inline BookmarksConstIterator ByteArrayModelPrivate::createBookmarksConstIterator() const
{
    return BookmarksConstIterator(new BookmarkListConstIteratorAdapter(m_bookmarks));
}

inline const Bookmark& ByteArrayModelPrivate::bookmarkFor(Address offset) const
{
    return m_bookmarks.bookmark(offset);
}
inline const Bookmark& ByteArrayModelPrivate::bookmarkAt(unsigned int index) const
{
    return m_bookmarks.at(index);
}
inline bool ByteArrayModelPrivate::containsBookmarkFor(Address offset) const { return m_bookmarks.contains(offset); }
inline unsigned int ByteArrayModelPrivate::bookmarksCount() const { return m_bookmarks.size(); }

}

#endif
