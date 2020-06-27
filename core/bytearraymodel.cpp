/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraymodel.hpp"

// lib
#include "bytearraymodel_p.hpp"
#include "arraychangemetricslist.hpp"

namespace Okteta {

ByteArrayModel::ByteArrayModel(Byte* data, int size, int rawSize, bool keepMemory, QObject* parent)
    : AbstractByteArrayModel(new ByteArrayModelPrivate(this, data, size, rawSize, keepMemory), parent)
{}

ByteArrayModel::ByteArrayModel(const Byte* data, int size, QObject* parent)
    : AbstractByteArrayModel(new ByteArrayModelPrivate(this, data, size), parent)
{}

ByteArrayModel::ByteArrayModel(int size, int maxSize, QObject* parent)
    : AbstractByteArrayModel(new ByteArrayModelPrivate(this, size, maxSize), parent)
{}

ByteArrayModel::~ByteArrayModel() = default;

Byte ByteArrayModel::byte(Address offset) const
{
    Q_D(const ByteArrayModel);

    return d->byte(offset);
}

Size ByteArrayModel::size() const
{
    Q_D(const ByteArrayModel);

    return d->size();
}

bool ByteArrayModel::isReadOnly() const
{
    Q_D(const ByteArrayModel);

    return d->isReadOnly();
}

bool ByteArrayModel::isModified() const
{
    Q_D(const ByteArrayModel);

    return d->isModified();
}

void ByteArrayModel::setReadOnly(bool readOnly)
{
    Q_D(ByteArrayModel);

    d->setReadOnly(readOnly);
}

void ByteArrayModel::setModified(bool modified)
{
    Q_D(ByteArrayModel);

    d->setModified(modified);
}

void ByteArrayModel::setMaxSize(int maxSize)
{
    Q_D(ByteArrayModel);

    d->setMaxSize(maxSize);
}

void ByteArrayModel::setKeepsMemory(bool keepsMemory)
{
    Q_D(ByteArrayModel);

    d->setKeepsMemory(keepsMemory);
}

void ByteArrayModel::setAutoDelete(bool autoDelete)
{
    Q_D(ByteArrayModel);

    d->setAutoDelete(autoDelete);
}

void ByteArrayModel::setData(Byte* data, int size, int rawSize, bool keepMemory)
{
    Q_D(ByteArrayModel);

    d->setData(data, size, rawSize, keepMemory);
}

Byte* ByteArrayModel::data() const
{
    Q_D(const ByteArrayModel);

    return d->data();
}

int ByteArrayModel::maxSize() const
{
    Q_D(const ByteArrayModel);

    return d->maxSize();
}

bool ByteArrayModel::keepsMemory() const
{
    Q_D(const ByteArrayModel);

    return d->keepsMemory();
}

bool ByteArrayModel::autoDelete() const
{
    Q_D(const ByteArrayModel);

    return d->autoDelete();
}

void ByteArrayModel::signalContentsChanged(int start, int end)
{
    const int length = end - start + 1;
    emit contentsChanged(ArrayChangeMetricsList::oneReplacement(start, length, length));
}

void ByteArrayModel::setByte(Address offset, Byte byte)
{
    Q_D(ByteArrayModel);

    d->setByte(offset, byte);
}

Size ByteArrayModel::insert(Address offset, const Byte* insertData, int insertLength)
{
    Q_D(ByteArrayModel);

    return d->insert(offset, insertData, insertLength);
}

Size ByteArrayModel::remove(const AddressRange& removeRange)
{
    Q_D(ByteArrayModel);

    return d->remove(removeRange);
}

Size ByteArrayModel::replace(const AddressRange& removeRange, const Byte* insertData, int insertLength)
{
    Q_D(ByteArrayModel);

    return d->replace(removeRange, insertData, insertLength);
}

bool ByteArrayModel::swap(Address firstStart, const AddressRange& secondRange)
{
    Q_D(ByteArrayModel);

    return d->swap(firstStart, secondRange);
}

Size ByteArrayModel::fill(Byte fillByte, Address offset, Size fillLength)
{
    Q_D(ByteArrayModel);

    return d->fill(fillByte, offset, fillLength);
}

void ByteArrayModel::addBookmarks(const QVector<Okteta::Bookmark>& bookmarks)
{
    Q_D(ByteArrayModel);

    d->addBookmarks(bookmarks);
}

void ByteArrayModel::removeBookmarks(const QVector<Okteta::Bookmark>& bookmarks)
{
    Q_D(ByteArrayModel);

    d->removeBookmarks(bookmarks);
}

void ByteArrayModel::removeAllBookmarks()
{
    Q_D(ByteArrayModel);

    d->removeAllBookmarks();
}

void ByteArrayModel::setBookmark(unsigned int index, const Okteta::Bookmark& bookmark)
{
    Q_D(ByteArrayModel);

    d->setBookmark(index, bookmark);
}

Okteta::BookmarksConstIterator ByteArrayModel::createBookmarksConstIterator() const
{
    Q_D(const ByteArrayModel);

    return d->createBookmarksConstIterator();
}

const Okteta::Bookmark& ByteArrayModel::bookmarkFor(int offset) const
{
    Q_D(const ByteArrayModel);

    return d->bookmarkFor(offset);
}

const Okteta::Bookmark& ByteArrayModel::bookmarkAt(unsigned int index) const
{
    Q_D(const ByteArrayModel);

    return d->bookmarkAt(index);
}

bool ByteArrayModel::containsBookmarkFor(int offset) const
{
    Q_D(const ByteArrayModel);

    return d->containsBookmarkFor(offset);
}

unsigned int ByteArrayModel::bookmarksCount() const
{
    Q_D(const ByteArrayModel);

    return d->bookmarksCount();
}

}
