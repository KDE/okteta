/*
    SPDX-FileCopyrightText: 2003, 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYMODEL_HPP
#define OKTETA_BYTEARRAYMODEL_HPP

// lib
#include "abstractbytearraymodel.hpp"
#include "bookmarkable.hpp"
// Qt
#include <QByteArray>

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
    /// Pass shared data as read-only
    /// The range pointed to by @p data pointer is expected to be valid the whole life-time,
    /// or until @c setData() is called.
    ByteArrayModel(const Byte* data, int size, QObject* parent = nullptr);
    /// Overload: shared data as read-only
    /// The range pointed to by @p byteArray's data() when the constructor is invoked
    /// is expected to be valid the whole life-time, or until @c setData() is called.
    explicit ByteArrayModel(const QByteArray& byteArray, QObject* parent = nullptr);
    /// Overload: shared data as read-only
    /// The range pointed to by @p data's data() when the constructor is invoked
    /// is expected to be valid the whole life-time, or until @c setData() is called.
    explicit ByteArrayModel(const std::vector<Okteta::Byte>& data, QObject* parent = nullptr);
    /// Overload: shared data as read-only
    /// The range pointed to by @p data's data() when the constructor is invoked
    /// is expected to be valid the whole life-time, or until @c setData() is called.
    template <std::size_t N>
    explicit ByteArrayModel(const std::array<Okteta::Byte, N>& data, QObject* parent = nullptr);

    /// Pass shared data as read-write, reusing memory
    /// The range pointed to by @p data pointer is expected to be valid the whole life-time,
    /// or until @c setData() is called.
    /// @param capacity The number of bytes that could be hold with the allocated memory.
    ///                 Also defaults the maximum number of bytes the model will allow.
    ///                 -1 will default to the value of @p size.
    ByteArrayModel(Byte* data, int size, int capacity = -1, QObject* parent = nullptr);
    /// Overload: shared data as read-write
    /// The range pointed to by @p data's data() when the constructor is invoked
    /// is expected to be valid the whole life-time, or until @c setData() is called.
    explicit ByteArrayModel(std::vector<Okteta::Byte>& data, QObject* parent = nullptr);
    /// Overload: shared data as read-write
    /// The range pointed to by @p data's data() when the constructor is invoked
    /// is expected to be valid the whole life-time, or until @c setData() is called.
    template <std::size_t N>
    explicit ByteArrayModel(std::array<Okteta::Byte, N>& data, QObject* parent = nullptr);

    /// Passes data as managed
    /// @see releaseData
    explicit ByteArrayModel(std::unique_ptr<Okteta::Byte[]>&& data, int size, int capacity = -1,
                            QObject* parent = nullptr);

    /// Creates managed byte array model
    /// @see releaseData
    explicit ByteArrayModel(int size, Byte fillByte, QObject* parent = nullptr);
    // not preset
    explicit ByteArrayModel(int size, QObject* parent = nullptr);
    explicit ByteArrayModel(QObject* parent = nullptr);

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
    Size removeBytes(const AddressRange& removeRange) override;
    [[nodiscard]]
    Size replaceBytes(const AddressRange& removeRange, const Byte* insertData, int insertLength) override;
    [[nodiscard]]
    bool swapBytes(Address firstStart, const AddressRange& secondRange) override;
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
    void setData(const Byte* data, int size);
    void setData(const QByteArray& byteArray);
    void setData(const std::vector<Okteta::Byte>& data);
    template <std::size_t N>
    void setData(const std::array<Okteta::Byte, N>& data);

    // pass shared data as read-write, reusing memory
    void setData(Byte* data, int size, int capacity = -1);
    void setData(std::vector<Okteta::Byte>& data);
    template <std::size_t N>
    void setData(std::array<Okteta::Byte, N>& data);

    // Passes data as managed
    void setData(std::unique_ptr<Okteta::Byte[]>&& data, int size, int capacity = -1);

    // TODO: setMinSize?
    /// Value -1 means full capacity
    /// If the current data is only shared with the model and @p maxSize is larger than
    /// the current capacity, it will be capped to thzat.
    /// If the current size is larger than the new maximal size, the size is capped to it.
    void setMaxSize(int maxSize);

    /**
     * @param start first position
     * @param end last position
     */
    void signalContentsChanged(int start, int end);

    [[nodiscard]]
    std::unique_ptr<Okteta::Byte[]> releaseData();

public:
    [[nodiscard]]
    int maxSize() const;
    [[nodiscard]]
    int capacity() const;

private:
    Q_DECLARE_PRIVATE(ByteArrayModel)
};

inline ByteArrayModel::ByteArrayModel(const QByteArray& byteArray, QObject* parent)
    : ByteArrayModel(reinterpret_cast<const Byte*>(byteArray.data()), byteArray.size(), parent)
{
}

inline ByteArrayModel::ByteArrayModel(const std::vector<Okteta::Byte>& data, QObject* parent)
        : ByteArrayModel(data.data(),  static_cast<int>(data.size()), parent)
{
}

template <std::size_t N>
inline ByteArrayModel::ByteArrayModel(const std::array<Okteta::Byte, N>& data, QObject* parent)
    : ByteArrayModel(data.data(),  static_cast<int>(data.size()), parent)
{
}

inline ByteArrayModel::ByteArrayModel(std::vector<Okteta::Byte>& data, QObject* parent)
    : ByteArrayModel(data.data(),  static_cast<int>(data.size()), parent)
{
}

template <std::size_t N>
inline ByteArrayModel::ByteArrayModel(std::array<Okteta::Byte, N>& data, QObject* parent)
    : ByteArrayModel(data.data(),  static_cast<int>(data.size()), parent)
{
}

inline ByteArrayModel::ByteArrayModel(QObject* parent)
    : ByteArrayModel(0, parent)
{
}

inline void ByteArrayModel::setData(const QByteArray& byteArray)
{
    setData(reinterpret_cast<const Byte*>(byteArray.data()), byteArray.size());
}

inline void ByteArrayModel::setData(const std::vector<Okteta::Byte>& data)
{
    setData(data.data(),  static_cast<int>(data.size()));
}

template <std::size_t N>
inline void ByteArrayModel::setData(const std::array<Okteta::Byte, N>& data)
{
    setData(data.data(),  static_cast<int>(data.size()));
}

inline void ByteArrayModel::setData(std::vector<Okteta::Byte>& data)
{
    setData(data.data(),  static_cast<int>(data.size()));
}

template <std::size_t N>
inline void ByteArrayModel::setData(std::array<Okteta::Byte, N>& data)
{
    setData(data.data(),  static_cast<int>(data.size()));
}

}

#endif
