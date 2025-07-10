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
    ByteArrayModel(Byte* data, int size, int rawSize = -1, bool keepMemory = true, QObject* parent = nullptr);
    ByteArrayModel(const Byte* data, int size, QObject* parent = nullptr);
    explicit ByteArrayModel(int size = 0, int maxSize = -1, QObject* parent = nullptr);

    ~ByteArrayModel() override;

public: // AbstractByteArrayModel API
    [[nodiscard]]
    Byte byte(Address offset) const override;
    [[nodiscard]]
    Size size() const override;
    [[nodiscard]]
    bool isReadOnly() const override;
    [[nodiscard]]
    bool isModified() const override;

    [[nodiscard]]
    Size insertBytes(Address offset, const Byte* insertData, int insertLength) override;
    [[nodiscard]]
    Size remove(const AddressRange& removeRange) override;
    [[nodiscard]]
    Size replace(const AddressRange& removeRange, const Byte* insertData, int insertLength) override;
    [[nodiscard]]
    bool swap(Address firstStart, const AddressRange& secondRange) override;
    [[nodiscard]]
    Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1) override;
    void setByte(Address offset, Byte byte) override;

    void setModified(bool modified = true) override;
    void setReadOnly(bool isReadOnly = true) override;

public: // Okteta::Bookmarkable API
    void addBookmarks(const QVector<Okteta::Bookmark>& bookmarks) override;
    void removeBookmarks(const QVector<Okteta::Bookmark>& bookmarks) override;
    void removeAllBookmarks() override;
    void setBookmark(unsigned int index, const Okteta::Bookmark& bookmark) override;

    [[nodiscard]]
    Okteta::BookmarksConstIterator createBookmarksConstIterator() const override;
    [[nodiscard]]
    const Okteta::Bookmark& bookmarkAt(unsigned int index) const override;
    [[nodiscard]]
    const Okteta::Bookmark& bookmarkFor(Address offset) const override;
    [[nodiscard]]
    bool containsBookmarkFor(Address offset) const override;
    [[nodiscard]]
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
    void setData(Byte* data, int size, int rawSize = -1, bool keepMemory = true);
    void signalContentsChanged(int start, int end);

public:
    [[nodiscard]]
    Byte* data() const;
    [[nodiscard]]
    int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
    [[nodiscard]]
    bool keepsMemory() const;
    [[nodiscard]]
    bool autoDelete() const;

private:
    Q_DECLARE_PRIVATE(ByteArrayModel)
};

}

#endif
