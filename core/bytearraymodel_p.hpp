/*
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
    ByteArrayModelPrivate(ByteArrayModel* parent, const Byte* data, int size);
    ByteArrayModelPrivate(ByteArrayModel* parent, Byte* data, int size, int capacity);
    ByteArrayModelPrivate(ByteArrayModel* parent, std::unique_ptr<Okteta::Byte[]>&& data, int size, int capacity);
    ByteArrayModelPrivate(ByteArrayModel* parent, int size, Byte fillByte);
    ByteArrayModelPrivate(ByteArrayModel* parent, int size);

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

    void setData(const Byte* data, int size);
    void setData(Byte* data, int size, int capacity);
    void setData(std::unique_ptr<Okteta::Byte[]>&& data, int size, int capacity);

    void setMaxSize(int maxSize);

    [[nodiscard]]
    std::unique_ptr<Okteta::Byte[]> releaseData();

public:
    [[nodiscard]]
    int maxSize() const;
    [[nodiscard]]
    int capacity() const;

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
    /** capacity of data array */
    int m_capacity;
    /** maximal size of array, unlimited if -1 */
    int mMaxSize = -1;

    Byte m_fillByte = '\0';

    /** flag whether the is owned or just shared with */
    bool m_ownsMemory : 1;
    /**  */
    bool m_isWritable : 1;
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
    if (!m_ownsMemory) {
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

    if (!m_isWritable) {
        return;
    }

    if (mReadOnly != isReadOnly) {
        mReadOnly = isReadOnly;
        Q_EMIT q->readOnlyChanged(isReadOnly);
    }
}
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

inline int ByteArrayModelPrivate::maxSize()      const { return mMaxSize; }
inline int ByteArrayModelPrivate::capacity()     const { return m_capacity; }

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
