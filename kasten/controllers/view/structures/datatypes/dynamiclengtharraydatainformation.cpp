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

#include "dynamiclengtharraydatainformation.h"
#include "primitivedatainformation.h"

void DynamicLengthArrayDataInformation::resizeChildren()
{
    //kDebug() << "old childcount: " << childCount();
    int l = qMax(0, calculateLength());
    unsigned int len = (unsigned) (l);
    setArrayLength(len);
}

qint64 DynamicLengthArrayDataInformation::readData(
        Okteta::AbstractByteArrayModel *input, ByteOrder byteOrder,
        Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    resizeChildren();
    uint readBytes = 0;
    quint64 readBits = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        qint64 currentReadBits = mChildren[i]->readData(input, byteOrder, address
                + readBytes, bitsRemaining - readBits, bitOffset);
        if (currentReadBits < 0)
        {
            mWasAbleToRead = false;
            return -1;
        }
        readBits += currentReadBits;
        readBytes = (readBits + *bitOffset) / 8;
    }
    mWasAbleToRead = true;
    return readBits;
}
DynamicLengthArrayDataInformation::DynamicLengthArrayDataInformation(QString name,
        const QString& lengthStr, const DataInformation& children, int index,
        DataInformation* parent) :
    AbstractArrayDataInformation(name, children, 0, index, parent), mLengthString(
            lengthStr)
{
    resizeChildren();
}

DynamicLengthArrayDataInformation::DynamicLengthArrayDataInformation(
        const DynamicLengthArrayDataInformation& d) :
    AbstractArrayDataInformation(d), mLengthString(d.mLengthString)
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
        kDebug()
            << "referenced size field not found";
        return 0;
    }
    for (int i = 0; i < refs.length(); ++i)
    {
        const DataInformation* data = refs.at(i);
        const PrimitiveDataInformation* prim =
                dynamic_cast<const PrimitiveDataInformation*> (data);
        if (prim)
        {
            if (!prim->wasAbleToRead())
            {
                kDebug()
                    << "primitive type is not valid";
                continue;
            }
            else
            {
                return prim->value().ulongValue;
            }
        }
    }
    return 0;
}

