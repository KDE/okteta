/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "uniondatainformation.h"

QString UnionDataInformation::typeName() const
{
    return i18nc("data type in C/C++", "union");
}

int UnionDataInformation::size() const
{
    //since this is a union retur size of biggest element
    int size = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        size = qMax(size, mChildren[i]->size());
    }
    if (size % 8 != 0)
    {
        //biggest element is a bitfield -> add padding
        size = size + (8 - size % 8);
    }
    return size;
}

Okteta::Size UnionDataInformation::readData(Okteta::AbstractByteArrayModel* input,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining,
        quint8* bitOffset)
{
    Okteta::Size readBytes = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        //union -> size of biggest element
        //always start at beginning again
        readBytes = qMax(readBytes, mChildren[i]->readData(input, byteOrder,
                address, remaining, bitOffset));
    }
    if (readBytes % 8 != 0)
    {
        //biggest element is a bitfield -> add padding
        readBytes = readBytes + (8 - readBytes % 8);
    }
    return readBytes;
}

bool UnionDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining, quint8* bitOffset)
{
    if (this == inf)
    {
        kDebug() << "non-editable item";
        return true; //do nothing since this is not editable
    }
    for (int i = 0; i < mChildren.size(); i++)
    {
        if (mChildren[i]->setData(value, inf, out, byteOrder, address, remaining,
                bitOffset))
            return true; //found -> done job
    }
    return false;
}

void UnionDataInformation::addDataTypeToUnion(DataInformation* field)
{
    appendChild(field);
}

UnionDataInformation& UnionDataInformation::operator<<(DataInformation* field)
{
    if (field)
    {
        addDataTypeToUnion(field);
    }
    return *this;
}

UnionDataInformation::~UnionDataInformation()
{
}

UnionDataInformation::UnionDataInformation(QString name, int index,
        DataInformation* parent) :
    DataInformationWithChildren(name, index, parent)
{
}

UnionDataInformation::UnionDataInformation(const UnionDataInformation& d) :
    DataInformationWithChildren(d)
{
}
