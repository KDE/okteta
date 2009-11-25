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
#include "datainformationwithchildren.h"

void DataInformationWithChildren::appendChild(DataInformation* child)
{
    if (child != NULL)
    {
        child->setIndex(mChildren.size());
        child->setParent(this);
        mChildren.append(child);
        connect(child, SIGNAL(dataChanged()), this, SLOT(onChildDataChanged()));
    }
}

DataInformation* DataInformationWithChildren::childAt(unsigned int idx) const
{
    if (idx >= (unsigned) mChildren.size())
    {
        return NULL;
    }
    return mChildren[idx];
}

bool DataInformationWithChildren::setData(const QVariant& value,
        DataInformation* inf, Okteta::AbstractByteArrayModel *out,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining)
{
    if (this == inf)
        return true; //do nothing since this is not editable
    Okteta::Size readBytes = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        if (mChildren[i]->setData(value, inf, out, byteOrder, address + readBytes,
                remaining - readBytes))
            return true; //found -> done job
        readBytes += mChildren[i]->getSize() / 8;

    }
    return false;
}

DataInformationWithChildren::~DataInformationWithChildren()
{
    qDeleteAll(mChildren);
}

DataInformationWithChildren::DataInformationWithChildren(QString& name, int index,
        DataInformation* parent) :
    DataInformation(name, index, parent)
{
}

DataInformationWithChildren::DataInformationWithChildren(
        const DataInformationWithChildren& d) :
    DataInformation(d)
{
    foreach(DataInformation* dat,d.mChildren)
        {
            DataInformation * child = dat->clone();
            child->setParent(this);
            QObject::connect(child, SIGNAL(dataChanged()), this,
                    SLOT(onChildDataChanged()));
            mChildren.append(child);
        }
}

bool DataInformationWithChildren::isValid() const
{
    for (int i = 0; i < mChildren.size(); ++i)
    {
        if (!mChildren.at(i)->isValid())
            return false; //if one is invalid, whole structure is invalid (out of range)
    }
    return true;
}
QList<const DataInformation*> DataInformationWithChildren::findChildrenWithName(
        const QString& name, const DataInformation* const upTo) const
{
    QList<const DataInformation*> retList;
    if (parent())
    {
        DataInformationWithChildren* par =
                static_cast<DataInformationWithChildren*> (parent());
        if (par)
            retList.append(par->findChildrenWithName(name, this));
    }
    for (uint i = 0; i < childCount(); ++i)
    {
        DataInformation* data = childAt(i);
        if (data == upTo)
            break;
        if (data->getName() == name)
            retList.append(data);
        DataInformationWithChildren* dataWithChildren =
                dynamic_cast<DataInformationWithChildren*> (data);
        if (dataWithChildren)
            retList.append(dataWithChildren->findChildrenWithName(name, NULL));
    }
    return retList;
}

