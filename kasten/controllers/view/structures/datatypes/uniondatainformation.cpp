/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
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
#include "topleveldatainformation.h"

QString UnionDataInformation::typeName() const
{
    return i18nc("data type in C/C++, then name", "union %1", name());
}

BitCount32 UnionDataInformation::size() const
{
    //since this is a union return size of biggest element
    BitCount32 size = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        size = qMax(size, mChildren.at(i)->size());
    }
    return size;
}

qint64 UnionDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    //first of all update the structure:
    topLevelDataInformation()->updateElement(this);

    qint64 readBits = 0;
    quint8 originalBitOffset = *bitOffset;
    quint8 correctBitOffset = originalBitOffset;
    for (int i = 0; i < mChildren.size(); i++)
    {
        //bit offset always has to be reset to original value
        qint64 currentReadBits = mChildren[i]->readData(input, address, bitsRemaining, bitOffset);
        if (currentReadBits == -1)
        {
            //could not read one element -> whole structure could not be read
            mWasAbleToRead = false;
            return -1;
        }
        if (currentReadBits > readBits)
        {
            readBits = correctBitOffset;
            correctBitOffset = *bitOffset;
        }
        *bitOffset = originalBitOffset; // start at beginning
    }
    mWasAbleToRead = false;
    return readBits;
}

void UnionDataInformation::addDataTypeToUnion(DataInformation* field)
{
    appendChild(field);
}

UnionDataInformation::~UnionDataInformation()
{
}

UnionDataInformation::UnionDataInformation(QString name, DataInformation* parent) :
    DataInformationWithChildren(name, parent)
{
}

UnionDataInformation::UnionDataInformation(const UnionDataInformation& d) :
    DataInformationWithChildren(d)
{
}
