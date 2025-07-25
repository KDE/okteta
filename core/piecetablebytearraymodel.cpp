/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetablebytearraymodel.hpp"
#include "piecetablebytearraymodel_p.hpp"

namespace Okteta {
// TODO: a wrapper to a raw data, plus a function to dump the current version to the original data

PieceTableByteArrayModel::PieceTableByteArrayModel(const QByteArray& data, QObject* parent)
    : AbstractByteArrayModel(std::make_unique<PieceTableByteArrayModelPrivate>(this, data), parent)
{}

PieceTableByteArrayModel::PieceTableByteArrayModel(int size, Byte fillByte, QObject* parent)
    : AbstractByteArrayModel(std::make_unique<PieceTableByteArrayModelPrivate>(this, size, fillByte), parent)
{}

PieceTableByteArrayModel::~PieceTableByteArrayModel() = default;

Byte PieceTableByteArrayModel::byte(Address offset) const
{
    Q_D(const PieceTableByteArrayModel);

    return d->byte(offset);
}

Size PieceTableByteArrayModel::size() const
{
    Q_D(const PieceTableByteArrayModel);

    return d->size();
}

bool PieceTableByteArrayModel::isReadOnly() const
{
    Q_D(const PieceTableByteArrayModel);

    return d->isReadOnly();
}

bool PieceTableByteArrayModel::isModified() const
{
    Q_D(const PieceTableByteArrayModel);

    return d->isModified();
}

void PieceTableByteArrayModel::setReadOnly(bool readOnly)
{
    Q_D(PieceTableByteArrayModel);

    d->setReadOnly(readOnly);
}

void PieceTableByteArrayModel::setModified(bool modified)
{
    Q_D(PieceTableByteArrayModel);

    d->setModified(modified);
}

void PieceTableByteArrayModel::setData(const QByteArray& data)
{
    Q_D(PieceTableByteArrayModel);

    d->setData(data);
}

void PieceTableByteArrayModel::setByte(Address offset, Byte byte)
{
    Q_D(PieceTableByteArrayModel);

    d->setByte(offset, byte);
}

Size PieceTableByteArrayModel::insertBytes(Address offset, const Byte* insertData, int insertLength)
{
    Q_D(PieceTableByteArrayModel);

    return d->insertBytes(offset, insertData, insertLength);
}

Size PieceTableByteArrayModel::removeBytes(const AddressRange& removeRange)
{
    Q_D(PieceTableByteArrayModel);

    return d->removeBytes(removeRange);
}

Size PieceTableByteArrayModel::replaceBytes(const AddressRange& removeRange, const Byte* insertData, int insertLength)
{
    Q_D(PieceTableByteArrayModel);

    return d->replaceBytes(removeRange, insertData, insertLength);
}

bool PieceTableByteArrayModel::swapBytes(Address firstStart, const AddressRange& secondRange)
{
    Q_D(PieceTableByteArrayModel);

    return d->swapBytes(firstStart, secondRange);
}

Size PieceTableByteArrayModel::fill(Byte fillByte, Address offset, Size fillLength)
{
    Q_D(PieceTableByteArrayModel);

    return d->fill(fillByte, offset, fillLength);
}

// int PieceTableByteArrayModel::indexOf( const char *searchString, int length, int from ) const
// {
//     return d->indexOf( searchString, length, from );
// }
//
// int PieceTableByteArrayModel::lastIndexOf( const char *searchString, int length, int from ) const
// {
//     return d->lastIndexOf( searchString, length, from );
// }

int PieceTableByteArrayModel::versionIndex() const
{
    Q_D(const PieceTableByteArrayModel);

    return d->versionIndex();
}

int PieceTableByteArrayModel::versionCount() const
{
    Q_D(const PieceTableByteArrayModel);

    return d->versionCount();
}

QString PieceTableByteArrayModel::versionDescription(int versionIndex) const
{
    Q_D(const PieceTableByteArrayModel);

    return d->versionDescription(versionIndex);
}

void PieceTableByteArrayModel::revertToVersionByIndex(int versionIndex)
{
    Q_D(PieceTableByteArrayModel);

    d->revertToVersionByIndex(versionIndex);
}

void PieceTableByteArrayModel::addBookmarks(const QVector<Okteta::Bookmark>& bookmarks)
{
    Q_D(PieceTableByteArrayModel);

    d->addBookmarks(bookmarks);
}

void PieceTableByteArrayModel::removeBookmarks(const QVector<Okteta::Bookmark>& bookmarks)
{
    Q_D(PieceTableByteArrayModel);

    d->removeBookmarks(bookmarks);
}

void PieceTableByteArrayModel::removeAllBookmarks()
{
    Q_D(PieceTableByteArrayModel);

    d->removeAllBookmarks();
}

void PieceTableByteArrayModel::setBookmark(unsigned int index, const Okteta::Bookmark& bookmark)
{
    Q_D(PieceTableByteArrayModel);

    d->setBookmark(index, bookmark);
}

Okteta::BookmarksConstIterator PieceTableByteArrayModel::createBookmarksConstIterator() const
{
    Q_D(const PieceTableByteArrayModel);

    return d->createBookmarksConstIterator();
}

const Okteta::Bookmark& PieceTableByteArrayModel::bookmarkAt(unsigned int index) const
{
    Q_D(const PieceTableByteArrayModel);

    return d->bookmarkAt(index);
}

const Okteta::Bookmark& PieceTableByteArrayModel::bookmarkFor(Address offset) const
{
    Q_D(const PieceTableByteArrayModel);

    return d->bookmarkFor(offset);
}

bool PieceTableByteArrayModel::containsBookmarkFor(Address offset) const
{
    Q_D(const PieceTableByteArrayModel);

    return d->containsBookmarkFor(offset);
}

unsigned int PieceTableByteArrayModel::bookmarksCount() const
{
    Q_D(const PieceTableByteArrayModel);

    return d->bookmarksCount();
}

void PieceTableByteArrayModel::openGroupedChange(const QString& description)
{
    Q_D(PieceTableByteArrayModel);

    d->openGroupedChange(description);
}

void PieceTableByteArrayModel::cancelGroupedChange()
{
    Q_D(PieceTableByteArrayModel);

    d->cancelGroupedChange();
}

void PieceTableByteArrayModel::closeGroupedChange(const QString& description)
{
    Q_D(PieceTableByteArrayModel);

    d->closeGroupedChange(description);
}

QVector<ByteArrayChange> PieceTableByteArrayModel::changes(int firstVersionIndex, int lastVersionIndex) const
{
    Q_D(const PieceTableByteArrayModel);

    return d->changes(firstVersionIndex, lastVersionIndex);
}

QByteArray PieceTableByteArrayModel::initialData() const
{
    Q_D(const PieceTableByteArrayModel);

    return d->initialData();
}

void PieceTableByteArrayModel::doChanges(const QVector<Okteta::ByteArrayChange>& changes,
                                         int oldVersionIndex, int newVersionIndex)
{
    Q_D(PieceTableByteArrayModel);

    d->doChanges(changes, oldVersionIndex, newVersionIndex);
}

}

#include "moc_piecetablebytearraymodel.cpp"
