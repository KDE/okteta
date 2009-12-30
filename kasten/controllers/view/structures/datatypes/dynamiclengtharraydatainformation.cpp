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

#include "dynamiclengtharraydatainformation.h"
#include "primitivedatainformation.h"

void DynamicLengthArrayDataInformation::resizeChildren()
{
    //kDebug() << "old childcount: " << childCount();
    int l = qMax(0, calculateLength());
    unsigned int len = (unsigned) (l);
    //kDebug() << "len: " << len;
    if (len > childCount())
    {
        for (uint i = childCount(); i < len; ++i)
        {
            DataInformation* arrayElem = mChildType->clone();
            QObject::connect(arrayElem, SIGNAL(dataChanged()), this,
                    SLOT(onChildDataChanged()));
            appendChild(arrayElem);
        }
    }
    else if (len < mChildren.length()) //TODO maybe keep some cached
    {
        for (int i = len; i != mChildren.length();)
        {
            delete mChildren.takeAt(i);
        }
    }
    //kDebug() << "new childcount: " << childCount();
}

Okteta::Size DynamicLengthArrayDataInformation::readData(
        Okteta::AbstractByteArrayModel* input, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining)
{
    Okteta::Size readBytes = 0;
    resizeChildren();
    for (unsigned int i = 0; i < childCount(); i++)
    {
        readBytes += childAt(i)->readData(input, byteOrder, address + readBytes,
                remaining - readBytes);
    }
    return readBytes;
}

DynamicLengthArrayDataInformation::DynamicLengthArrayDataInformation(QString name,
        const QString& lengthStr, const DataInformation& children, int index,
        DataInformation* parent) :
    AbstractArrayDataInformation(name, index, parent), mLengthString(lengthStr),
            mChildType(children.clone())
{
    resizeChildren();
}

DynamicLengthArrayDataInformation::DynamicLengthArrayDataInformation(
        const DynamicLengthArrayDataInformation& d) :
    AbstractArrayDataInformation(d), mLengthString(d.mLengthString), mChildType(
            d.mChildType->clone())
{
}

DynamicLengthArrayDataInformation::~DynamicLengthArrayDataInformation()
{
}
int DynamicLengthArrayDataInformation::calculateLength()
{
    if (!parent())
    {
        kWarning() << "dynamic length array without parent ->"
            " length will always be 0";
        return 0;
    }
    QList<const DataInformation*> refs = findChildrenWithName(mLengthString, this);
    if (refs.length() == 0)
    {
        kDebug() << "referenced size field not found";
        return 0;
    }
    for (int i = 0; i < refs.length(); ++i)
    {
        const DataInformation* data = refs.at(i);
        const PrimitiveDataInformation* prim =
                dynamic_cast<const PrimitiveDataInformation*> (data);
        if (prim)
        {
            if (!prim->isValid())
            {
                kDebug() << "primitive type is not valid";
                continue;
            }
            else
            {
                return prim->value().longValue;
            }
        }
    }
    return 0;
}

