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
    if (this != inf)
        return false;
    if ((unsigned) (remaining * 8) - *bitOffset < width())
    {
        mIsValid = false;
        mValue = 0;
        *bitOffset = 0;
        return true;
    }
    Q_ASSERT(*bitOffset < 8);
    AllPrimitiveTypes val = value.toULongLong();
    if (val.ulongValue != mValue.ulongValue)
        emit dataChanged();
    mValue = val;
    //set model data:
    kError() << "not implemented yet";
#if 0
    if (width() < (unsigned) (8 - *bitOffset))
    {
        quint8 readValue;
        if (byteOrder == ByteOrderEnumClass::LittleEndian)
        {
            //fits completely
            quint8 lowerMask = (1 << *bitOffset) - 1;
            quint8 higherMask = ~((1 << (*bitOffset + width())) - 1);
            quint8 completeMask = lowerMask | higherMask;
            //completeMask maskes the value -> negate it to clear all the bytes
            readValue = out->byte(address);
            readValue &= ~completeMask;
            //fill in our bytes
            readValue |= val.ubyteValue << *bitOffset;
        }
        else
        {
            //big endian and fits completely
            quint8 lowerMask = (1 << (8 - (width() + *bitOffset))) - 1;
            quint8 higherMask = 0xff << (8 - *bitOffset);
            quint8 completeMask = lowerMask | higherMask;
            //completeMask maskes the value -> negate it to clear all the bytes
            readValue = out->byte(address);
            readValue &= ~completeMask;
            //fill in our bytes
            readValue |= val.ubyteValue << (8 - (*bitOffset + width()));
        }
        out->setByte(address, readValue);
    }
    else
    {
        for (uint i = 0; i < width(); i += 8)
        {
            if (byteOrder == ByteOrderEnumClass::LittleEndian)
            {
                if (*bitOffset != 0)
                {
                    if (byteOrder == ByteOrderEnumClass::LittleEndian)
                    {
                        int remainingBits = width() - i;
                        Q_ASSERT(*bitOffset < 8);
                        quint8 thisByte = out->byte(address);
                        int remainingBitsInThisByte = 8 - *bitOffset;
                        if (remainingBits < remainingBitsInThisByte)
                        {
                            //leave the top values:
                            int theMask = (1 << remainingBitsInThisByte) - 1;
                            quint8 topMask = ~((1 << (8 - (remainingBitsInThisByte
                                                            - remainingBits))) - 1);
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
                        // big endian
                        int remainingBits = width() - i;
                        Q_ASSERT(*bitOffset < 8);
                        quint8 thisByte = out->byte(address);
                        int remainingBitsInThisByte = 8 - *bitOffset;
                        //clear the lowest bits
                        if (remainingBits < remainingBitsInThisByte)
                        {
                            quint8 theMask = 0xff << remainingBitsInThisByte;
                            quint8 otherMask = ((1 << remainingBits) - 1)
                            << (remainingBitsInThisByte - remainingBits);//leave the lowest bits
                            theMask |= otherMask;
                            thisByte &= theMask;
                        }
                        else
                        thisByte &= ((unsigned) 0xff << remainingBitsInThisByte); //clear lowest bits in currentValue
                        quint64 mask = (1 << remainingBitsInThisByte) - 1;
                        mask <<= i; //shift mask to match the current byte
                        quint64 maskedVal = val.ulongValue & mask;
                        maskedVal >>= i; //move back to lowest byte
                        quint8 bitfieldValuesOfCurrentByte = maskedVal >> *bitOffset; //and shift to correct position
                        thisByte |= bitfieldValuesOfCurrentByte;
                        out->setByte(address + (i / 8), thisByte);
                    }
                }
                else
                {
                    //no bit offset
                    quint8 thisByte = (val.ulongValue & mask()) >> i;
                    out->setByte(address + (i / 8), thisByte);
                }
            }
        }
    }
#endif
    *bitOffset = (*bitOffset + width()) % 8;
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
    Q_ASSERT(*bitOffset < 8);
    AllPrimitiveTypes val;
    if (byteOrder == ByteOrderEnumClass::LittleEndian)
    {
        val = readDataLittleEndian(input, address, bitOffset);
    }
    else if (byteOrder == ByteOrderEnumClass::BigEndian)
    {
        val = readDataBigEndian(input, address, bitOffset);
    }
    else
    {
        kError() << "invalid byte order specified.";
        val = 0;
    }
    if (val.ulongValue != mValue.ulongValue)
    {
        emit dataChanged();
        mValue = val;
    }
    int usedBits = *bitOffset + width();
    *bitOffset = (*bitOffset + width()) % 8;
    mIsValid = true;
    return usedBits / 8;
}

AllPrimitiveTypes AbstractBitfieldDataInformation::readDataLittleEndian(
        Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        quint8* bitOffset)
{
    //maybe compiler can't optimise all that dereferencing away, also saves typing a few chars
    quint8 bo = *bitOffset;
    AllPrimitiveTypes val(0);
    if (width() <= (unsigned) (8 - bo))
    {
        //fits completely
        quint8 lowerMask = 0xff << bo; //all lower bits are 0
        quint8 higherMask = 0xff >> (8 - (bo + width())); // all higher bits are 0
        quint8 completeMask = lowerMask & higherMask; //region in the middle
        quint8 readByte = input->byte(address);
        quint8 maskedByte = readByte & completeMask;
        val.ubyteValue = maskedByte >> bo;
    }
    else
    {
        quint8 firstByteMask = 0xff << bo;
        quint8 firstByte = input->byte(address);
        quint8 firstByteMasked = firstByte & firstByteMask;
        val.ubyteValue = firstByteMasked >> bo;
        //if spans more than this one byte continue
        for (uint i = 8; i < width() + bo; i += 8)
        {
            quint8 readVal = input->byte(address + (i / 8));
            if (width() + bo <= i + 8)
            {
                //this is last byte needed, possibly cut off top values
                quint8 missingBits = (width() + bo) % 8;
                quint8 mask = (1 << missingBits) - 1;
                readVal &= mask; //mask the top few bits
            }
            //otherwise we need full byte -> nothing to do
            quint64 shiftedVal = readVal << i;
            val.ulongValue |= shiftedVal >> bo; //move to correct byte
        }
    }
    return val;
}

AllPrimitiveTypes AbstractBitfieldDataInformation::readDataBigEndian(
        Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        quint8* bitOffset)
{
    //maybe compiler can't optimise all that dereferencing away, also saves typing a few chars
    quint8 bo = *bitOffset;
    AllPrimitiveTypes val(0);
    if (width() <= (unsigned) (8 - bo))
    {
        quint8 lowerMask = 0xff << (8 - (bo + width()));
        quint8 higherMask = 0xff >> bo;
        quint8 completeMask = lowerMask & higherMask;
        //completeMask maskes the value -> negate it to clear all the bytes
        quint8 readByte = input->byte(address);
        quint8 maskedByte = readByte & completeMask;
        val.ubyteValue = maskedByte >> (8 - (bo + width()));
    }
    else
    {
        quint8 firstByteMask = 0xff >> bo;
        quint8 firstByte = input->byte(address);
        quint8 firstByteMasked = firstByte & firstByteMask;
        quint64 firstByteShifted = firstByteMasked << (bo + width() - 8);
        val.ulongValue = firstByteShifted;
        //if spans more than this one byte continue
        for (uint i = 8; i < width() + bo; i += 8)
        {
            quint8 readVal = input->byte(address + (i / 8));
            if (width() + bo <= i + 8)
            {
                //this is last byte needed, possibly cut off lower values
                quint8 missingBits = (bo + width()) % 8;
                quint8 mask = 0xff << (8 - missingBits);
                quint8 maskedVal = readVal & mask; //cut off lower bits
                quint8 shiftedVal = maskedVal >> (8 - missingBits);
                val.ubyteValue |= shiftedVal;
            }
            else
            {
                //otherwise we need full byte -> nothing to do
                quint64 shiftedVal = readVal << ((bo + width()) - (8 + i)); //move to correct byte
                val.ulongValue |= shiftedVal;
            }
        }
    }
    return val;
}

void AbstractBitfieldDataInformation::setDataLittleEndian(const QVariant &value,
        DataInformation* inf, Okteta::AbstractByteArrayModel *out,
        Okteta::Address address, quint8* bitOffset)
{
    //FIXME stub

}

void AbstractBitfieldDataInformation::setDataBigEndian(const QVariant &value,
        DataInformation* inf, Okteta::AbstractByteArrayModel *out,
        Okteta::Address address, quint8* bitOffset)
{
    //FIXME stub

}

QString AbstractBitfieldDataInformation::getSizeString() const
{
    return i18np("%1 bit", "%1 bits", width());
}
