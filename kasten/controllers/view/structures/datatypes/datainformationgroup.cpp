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
#include "datainformationgroup.h"
#include <KLocale>

DataInformationGroup::DataInformationGroup(QString name, DataInformation* parent) :
    DataInformationWithChildren(name, -1, parent)
{
}

DataInformationGroup::DataInformationGroup(const DataInformationGroup& d) :
    DataInformationWithChildren(d)
{
}

DataInformationGroup::~DataInformationGroup()
{
}

Okteta::Size DataInformationGroup::readData(Okteta::AbstractByteArrayModel* input,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining)
{
    Okteta::Size readBytes = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        //group -> return size of biggest element
        //always start at beginning again
        readBytes = qMax(readBytes, mChildren[i]->readData(input, byteOrder, address,
                remaining));
    }
    return readBytes;
}

QVariant DataInformationGroup::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == 0)
            return i18n("Structure definitions loaded from %1", getName());
        else if (column == 1)
            return QVariant(); //empty QString
        else
            return QVariant();
    }
    else
        return QVariant();
}

QString DataInformationGroup::getTypeName() const {
    return i18n("group");
}

bool DataInformationGroup::setData(const QVariant& value, DataInformation* inf,
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
