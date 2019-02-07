/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003,2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
    Q_D(const FileByteArrayModel);

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
