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
#include "abstractbitfielddatainformation.h"
bool AbstractBitfieldDataInformation::setData(const QVariant &value,
        DataInformation* inf, Okteta::AbstractByteArrayModel *out,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining,
        quint8* bitOffset)
{
    Q_UNUSED(byteOrder)
    if (this != inf)
        return false;
    if ((unsigned) (remaining * 8) - *bitOffset < width())
    {
        mIsValid = false;
        mValue = 0;
        *bitOffset = 0;
        return true;
    }
    AllPrimitiveTypes val = value.toULongLong();
    if (val.ulongValue != mValue.ulongValue)
        emit dataChanged();
    mValue = val;
    //set model data:
    for (uint i = 0; i < width(); i += 8)
    {
        if (*bitOffset != 0)
        {
            int remainingBits = width() - i;
            Q_ASSERT(*bitOffset < 8);
            quint8 thisByte = out->byte(address);
            int remainingBitsInThisByte = 8 - *bitOffset;
            if (remainingBits < remainingBitsInThisByte)
            {
                //leave the top values:
                int theMask = (1 << remainingBitsInThisByte) - 1;
                quint8 topMask = ~(((1 << remainingBitsInThisByte) - 1) | (((1
                        << remainingBits) - 1) << *bitOffset));
                theMask |= topMask;
                thisByte &= theMask;
            }
            else
                thisByte &= (1 << *bitOffset) - 1; //clear top bits in currentValue
            quint64 mask = (1 << remainingBitsInThisByte) - 1;
            mask <<= i; //shift mask to match the current byte
            quint64 maskedVal = val.ulongValue & mask;
            maskedVal >>= i; //move back to lowest byte
            quint8 bitfieldValuesOfCurrentByte = maskedVal << *bitOffset; //and shift to correct position
            thisByte |= bitfieldValuesOfCurrentByte;
            out->setByte(address + (i / 8), thisByte);
        }
        else
        {
            quint8 thisByte = (val.ulongValue & mask()) >> i;
            out->setByte(address + (i / 8), thisByte);
        }
    }
    *bitOffset = (*bitOffset + width()) % 8;
    mIsValid = true;
    return true;
}
Okteta::Size AbstractBitfieldDataInformation::readData(
        Okteta::AbstractByteArrayModel* input, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining, quint8* bitOffset)
{
    Q_UNUSED(byteOrder)
    if ((unsigned) (remaining * 8) - *bitOffset < width())
    {
        mIsValid = false;
        mValue = 0;
        return true;
    }
    AllPrimitiveTypes val = 0;
    //set model data:
    for (uint i = 0; i < width(); i += 8)
    {
        Q_ASSERT(*bitOffset < 8);
        quint8 thisByte = input->byte(address);
        thisByte >>= *bitOffset; //get the useful bytes
        int remainingBits = width() - i;
        if (remainingBits < 8 - *bitOffset)
        {
            int theMask = (1 << remainingBits) - 1;
            thisByte &= theMask;
        }
        val.ulongValue |= thisByte << i;
    }
    if (val.ulongValue != mValue.ulongValue)
        emit dataChanged();
    mValue = val;
    int usedBits = *bitOffset + width();
    *bitOffset = (*bitOffset + width()) % 8;
    mIsValid = true;
    return usedBits / 8;
}

QString AbstractBitfieldDataInformation::getSizeString() const
{
    return i18np("%1 bit", "%1 bits", width());
}
