/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_PIECETABLEBYTEARRAYMODEL_HPP
#define OKTETA_PIECETABLEBYTEARRAYMODEL_HPP

// lib
#include "abstractbytearraymodel.hpp"
#include "versionable.hpp"
#include "bookmarkable.hpp"
#include "changesdescribable.hpp"
#include "changehistory.hpp"

namespace Okteta {

class PieceTableByteArrayModelPrivate;

/**
 * @author Friedrich W. H. Kossebau
 */

class OKTETACORE_EXPORT PieceTableByteArrayModel : public AbstractByteArrayModel
                                                 , public Versionable
                                                 , public Bookmarkable
                                                 , public ChangesDescribable
                                                 , public ChangeHistory
{
    Q_OBJECT
    Q_INTERFACES(
        Okteta::Versionable
        Okteta::Bookmarkable
        Okteta::ChangesDescribable
        Okteta::ChangeHistory
    )

    friend class PieceTableByteArrayModelPrivate;

public:
    explicit PieceTableByteArrayModel(const QByteArray& data, QObject* parent = nullptr);
    explicit PieceTableByteArrayModel(int size = 0, Byte fillByte = '\0', QObject* parent = nullptr);

    ~PieceTableByteArrayModel() override;

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

//     virtual int indexOf( const char *searchString, int length, int from  = 0 ) const;
//     virtual int lastIndexOf( const char *searchString, int length, int from = -1 ) const;

public: // Versionable API
    [[nodiscard]]
    int versionIndex() const override;
    [[nodiscard]]
    int versionCount() const override;
    [[nodiscard]]
    QString versionDescription(int versionIndex) const override;

public: // set/action
    void revertToVersionByIndex(int versionIndex) override;

public: // Bookmarkable API
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

public: // ChangesDescribable API
    void openGroupedChange(const QString& description) override;
    void cancelGroupedChange() override;
    void closeGroupedChange(const QString& description) override;

public: // ChangeHistory API
    [[nodiscard]]
    QVector<ByteArrayChange> changes(int firstVersionIndex, int lastVersionIndex) const override;
    [[nodiscard]]
    QByteArray initialData() const override;
    void doChanges(const QVector<Okteta::ByteArrayChange>& changes,
                   int oldVersionIndex, int newVersionIndex) override;

public:
    void setData(const QByteArray& data);

Q_SIGNALS: // Versionable signals
    void revertedToVersionIndex(int versionIndex) override;
    void headVersionDescriptionChanged(const QString& versionDescription) override;
    void headVersionChanged(int newHeadVersionIndex) override;

Q_SIGNALS: // Bookmarkable signals
    void bookmarksAdded(const QVector<Okteta::Bookmark>& bookmarks) override;
    void bookmarksRemoved(const QVector<Okteta::Bookmark>& bookmarks) override;
    void bookmarksModified(bool modified) override;
    void bookmarksModified(const QVector<int>& indizes) override;

Q_SIGNALS: // ChangeHistory signals
    void changesDone(const QVector<Okteta::ByteArrayChange>& changes,
                     int oldVersionIndex, int newVersionIndex) override;

private:
    Q_DECLARE_PRIVATE(PieceTableByteArrayModel)
};

}

#endif
