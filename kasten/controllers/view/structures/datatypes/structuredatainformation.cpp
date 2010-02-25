/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
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
#include "structuredatainformation.h"
#include "abstractbitfielddatainformation.h"
QString StructureDataInformation::typeName() const
{
    return i18nc("data type in C/C++", "struct");
}

int StructureDataInformation::size() const
{
    int size = 0;
    bool lastChildWasBitfield = false;
    for (int i = 0; i < mChildren.size(); i++)
    {
        const DataInformation* data = mChildren.at(i);

        bool isBitfield =
            dynamic_cast<const AbstractBitfieldDataInformation*> (data) != 0;
        if (isBitfield)
        {
            lastChildWasBitfield = true;
        }
        else if (lastChildWasBitfield)
        {
            uint padding = 8 - (size % 8);
            size += padding;
            //this element is not a bitfield -> add padding;
            lastChildWasBitfield = false;
        }
        size += data->size();
    }
    if (lastChildWasBitfield)
    {
        //add padding at end of structure (align to next byte)
        uint padding = 8 - (size % 8);
        size += padding;
    }
    return size;
}

Okteta::Size StructureDataInformation::readData(
        Okteta::AbstractByteArrayModel* input, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining, quint8* bitOffset)
{
    Okteta::Size readBytes = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        readBytes += mChildren[i]->readData(input, byteOrder, address + readBytes,
                remaining - readBytes, bitOffset);
    }
    if (readBytes % 8 != 0)
    {
        //last element is a bitfield -> add padding
        readBytes = readBytes + (8 - readBytes % 8);
    }
    return readBytes;
}

void StructureDataInformation::addDataTypeToStruct(DataInformation* field)
{
    appendChild(field);
}

StructureDataInformation& StructureDataInformation::operator<<(
        DataInformation* field)
{
    if (field)
    {
        addDataTypeToStruct(field);
    }
    return *this;
}

StructureDataInformation::~StructureDataInformation()
{
}

StructureDataInformation::StructureDataInformation(QString name, int index,
        DataInformation* parent) :
    DataInformationWithChildren(name, index, parent)
{
}

StructureDataInformation::StructureDataInformation(const StructureDataInformation& d) :
    DataInformationWithChildren(d)
{
}

Okteta::Size StructureDataInformation::offset(unsigned int index) const
{
    if (index >= childCount())
        return 0;
    Okteta::Size offset = 0;
    //sum size of elements up to index
    for (unsigned int i = 0; i < index; ++i)
    {
        offset += childAt(i)->size() / 8;
    }
    return offset;
}

