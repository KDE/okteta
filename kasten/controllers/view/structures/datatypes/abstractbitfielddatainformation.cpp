/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "abstractbitfielddatainformation.h"

//FIXME this code really needs unit tests!
bool AbstractBitfieldDataInformation::setData(const QVariant& value,
        DataInformation* inf, Okteta::AbstractByteArrayModel *out,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining,
        quint8* bitOffset)
{
    if (this != inf)
    {
        //make sure bitOffset is always incremented
        *bitOffset = (*bitOffset + width()) % 8;
        return false;
    }
    AllPrimitiveTypes oldVal(mValue);
    AllPrimitiveTypes newVal(value.toULongLong());
    mIsValid = mValue.writeBits(width(), newVal, out, byteOrder, address, remaining,
            bitOffset);
    if (oldVal != mValue)
        emit dataChanged();
    mIsValid = true;
    return true;
}

Okteta::Size AbstractBitfieldDataInformation::readData(
        Okteta::AbstractByteArrayModel* input, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining, quint8* bitOffset)
{
    if ((unsigned) (remaining * 8) - *bitOffset < width())
    {
        mIsValid = false;
        mValue = 0;
        return true;
    }
    int usedBits = *bitOffset + width();
    AllPrimitiveTypes oldVal(mValue);
    mIsValid = mValue.readBits(width(),input,byteOrder,address,remaining,bitOffset);
    if (oldVal != mValue.ulongValue)
    {
        emit dataChanged();
    }
    return usedBits / 8; //TODO the return value needs updating in method signature
}

QString AbstractBitfieldDataInformation::sizeString() const
{
    return i18np("%1 bit", "%1 bits", width());
}
