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
#include "staticlengtharraydatainformation.h"

QString UnionDataInformation::getTypeName() const
{
    return i18nc("data type in C/C++", "union");
}

int UnionDataInformation::getSize() const
{
    //since this is a union retur size of biggest element
    int size = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        size = qMax(size, mChildren[i]->getSize());
    }
    return size;
}

Okteta::Size UnionDataInformation::readData(Okteta::AbstractByteArrayModel* input,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining)
{
    Okteta::Size readBytes = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        //union -> size of biggest element
        //always start at beginning again
        readBytes = qMax(readBytes, mChildren[i]->readData(input, byteOrder,
                address, remaining));
    }
    return readBytes;
}

bool UnionDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining)
{
    if (this == inf)
    {
        kDebug() << "not editable item";
        return true; //do nothing since this is not editable
    }
    for (int i = 0; i < mChildren.size(); i++)
    {
        if (mChildren[i]->setData(value, inf, out, byteOrder, address, remaining))
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

QVariant DataInformationWithChildren::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == 0)
        {
            if (dynamic_cast<StaticLengthArrayDataInformation*> (parent()))
            {
                return QString("[%1]").arg(mIndex);
            }
            return getName();
        }
        else if (column == 1)
            return getTypeName();
        else if (column == 2)
            return getValueString(); //empty QString
        else
            return QVariant();
    }
    else if (role == Qt::ToolTipRole)
    {
        return i18np("Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)",
                "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)",
                childCount(), getName(), getValueString(), getTypeName(),
                getSizeString());
    }
    else
        return QVariant();
}
