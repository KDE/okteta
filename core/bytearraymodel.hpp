/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYMODEL_HPP
#define OKTETA_BYTEARRAYMODEL_HPP

// lib
#include "abstractbytearraymodel.hpp"
#include "bookmarkable.hpp"
#include "bookmarksconstiterator.hpp"

namespace Okteta {

class ByteArrayModelPrivate;

/**
 * @author Friedrich W. H. Kossebau
*/

class OKTETACORE_EXPORT ByteArrayModel : public AbstractByteArrayModel
                                       , public Bookmarkable
{
    Q_OBJECT
    Q_INTERFACES(
        Okteta::Bookmarkable
    )

    friend class ByteArrayModelPrivate;

public:
    ByteArrayModel(Byte* data, int size, int rawSize = -1, bool keepsMemory = true, QObject* parent = nullptr);
    ByteArrayModel(const Byte* data, int size, QObject* parent = nullptr);
    explicit ByteArrayModel(int size = 0, int maxSize = -1, QObject* parent = nullptr);

    ~ByteArrayModel() override;

public: // AbstractByteArrayModel API
    Byte byte(Address offset) const override;
    Size size() const override;
    bool isReadOnly() const override;
    bool isModified() const override;

    Size insert(Address offset, const Byte* insertData, int insertLength) override;
    Size remove(const AddressRange& removeRange) override;
    Size replace(const AddressRange& removeRange, const Byte* insertData, int insertLength) override;
    bool swap(Address firstStart, const AddressRange& secondRange) override;
    Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1) override;
    void setByte(Address offset, Byte byte) override;

    void setModified(bool modified = true) override;
    void setReadOnly(bool isReadOnly = true) override;

public: // Okteta::Bookmarkable API
    void addBookmarks(const QVector<Okteta::Bookmark>& bookmarks) override;
    void removeBookmarks(const QVector<Okteta::Bookmark>& bookmarks) override;
    void removeAllBookmarks() override;
    void setBookmark(unsigned int index, const Okteta::Bookmark& bookmark) override;

    Okteta::BookmarksConstIterator createBookmarksConstIterator() const override;
    const Okteta::Bookmark& bookmarkAt(unsigned int index) const override;
    const Okteta::Bookmark& bookmarkFor(int offset) const override;
    bool containsBookmarkFor(int offset) const override;
    unsigned int bookmarksCount() const override;

Q_SIGNALS: // Okteta::Bookmarkable API
    void bookmarksAdded(const QVector<Okteta::Bookmark>& bookmarks) override;
    void bookmarksRemoved(const QVector<Okteta::Bookmark>& bookmarks) override;
    void bookmarksModified(bool modified) override;
    void bookmarksModified(const QVector<int>& indizes) override;

public:
    void setMaxSize(int maxSize);
    /** sets whether the memory given by setData or in the constructor should be kept on resize
     */
    void setKeepsMemory(bool keepsMemory = true);
    /** sets whether the memory given by setData or in the constructor gets deleted in destructor
     * or when new data is set. The data MUST have been allocated using new[] otherwise behaviour
     * is undefined */
    void setAutoDelete(bool autoDelete = true);
    void setData(Byte* data, int size, int rawSize = -1, bool keepsMemory = true);
    void signalContentsChanged(int i1, int i2);

public:
    Byte* data() const;
    int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
    bool keepsMemory() const;
    bool autoDelete() const;

private:
    Q_DECLARE_PRIVATE(ByteArrayModel)
};

}

#endif
