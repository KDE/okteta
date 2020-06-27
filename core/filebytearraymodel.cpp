/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filebytearraymodel.hpp"

// lib
#include "filebytearraymodel_p.hpp"
// Std
#include <cstdlib>

namespace Okteta {

FileByteArrayModel::FileByteArrayModel(int pageNumber, int pageSize, QObject* parent)
    : AbstractByteArrayModel(new FileByteArrayModelPrivate(this, pageNumber, pageSize), parent)
{
}

FileByteArrayModel::~FileByteArrayModel() = default;

Size FileByteArrayModel::size() const
{
    Q_D(const FileByteArrayModel);

    return d->size();
}

bool FileByteArrayModel::isReadOnly() const
{
    Q_D(const FileByteArrayModel);

    return d->isReadOnly();
}

bool FileByteArrayModel::isModified() const
{
    return false;
}

bool FileByteArrayModel::isOpen() const
{
    Q_D(const FileByteArrayModel);

    return d->isOpen();
}

void FileByteArrayModel::setReadOnly(bool readOnly)
{
    Q_D(FileByteArrayModel);

    d->setReadOnly(readOnly);
}

void FileByteArrayModel::setModified(bool)  {}

void FileByteArrayModel::setByte(Address, Byte)  {}

Byte FileByteArrayModel::byte(Address offset) const
{
    Q_D(const FileByteArrayModel);

    return d->byte(offset);
}

Size FileByteArrayModel::insert(Address /*Pos*/, const Byte*, int /*Length*/) {  return 0; }
Size FileByteArrayModel::remove(const AddressRange& /*Section*/) {  return 0; }
Size FileByteArrayModel::replace(const AddressRange& /*Section*/, const Byte*, int /*Length*/) {  return 0; }
Size FileByteArrayModel::fill(Byte /*FillChar*/, Address /*Pos*/, Size /*Length*/) {  return 0; }
bool FileByteArrayModel::swap(Address /*DestPos*/, const AddressRange& /*SourceSection*/) { return false; }

bool FileByteArrayModel::open(const QString& fileName)
{
    Q_D(FileByteArrayModel);

    return d->open(fileName);
}

bool FileByteArrayModel::close()
{
    Q_D(FileByteArrayModel);

    return d->close(); 
}

}
