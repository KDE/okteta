/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
        size = qMax(size, mChildren.at(i)->size());
    }
    return size;
}

qint64 UnionDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        ByteOrder byteOrder, Okteta::Address address, quint64 bitsRemaining,
        quint8* bitOffset)
{
    qint64 readBits = 0;
    quint8 originalBitOffset = *bitOffset;
    quint8 correctBitOffset = originalBitOffset;
    for (int i = 0; i < mChildren.size(); i++)
    {
        //bit offset always has to be reset to original value
        qint64 currentReadBits = mChildren[i]->readData(input, byteOrder, address,
                bitsRemaining, bitOffset);
        if (currentReadBits == -1)
            return -1;
        if (currentReadBits > readBits) {
            readBits = correctBitOffset;
            correctBitOffset = *bitOffset;
        }
        *bitOffset = originalBitOffset; // start at beginning
    }
    return readBits;
}

bool UnionDataInformation::setData(const QVariant& value,
        DataInformation* inf, Okteta::AbstractByteArrayModel *out,
        ByteOrder byteOrder, Okteta::Address address, quint64 bitsRemaining,
        quint8* bitOffset)
{
    if (this == inf)
        return true; //do nothing since this is not editable
    quint64 readBits = 0;
    uint readBytes = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        if (mChildren[i]->setData(value, inf, out, byteOrder, address + readBytes,
                bitsRemaining - readBits, bitOffset))
            return true; //found -> done job

        //useless since only one write anyway?
        readBits += mChildren[i]->size();
        readBytes = (readBits + *bitOffset) / 8;
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
