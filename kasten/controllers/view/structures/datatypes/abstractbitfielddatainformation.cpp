/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
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
bool AbstractBitfieldDataInformation::setData(const QVariant &value,
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

    if (byteOrder == ByteOrderEnumClass::LittleEndian)
    {
        setDataLittleEndian(val, out, address, bitOffset);
    }
    else if (byteOrder == ByteOrderEnumClass::BigEndian)
    {
        setDataBigEndian(val, out, address, bitOffset);
    }
    else
    {
        kWarning() << "invalid byte order";
    }
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
        const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
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
        const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
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

void AbstractBitfieldDataInformation::setDataLittleEndian(
        const AllPrimitiveTypes val, Okteta::AbstractByteArrayModel *out,
        Okteta::Address address, quint8* bitOffset)
{
    //maybe compiler can't optimise all that dereferencing away, also saves typing a few chars
    quint8 bo = *bitOffset;
    if (width() <= (unsigned) (8 - bo))
    {
        //fits completely
        quint8 lowerMask = (1 << bo) - 1; //all lower bits are 1
        quint8 higherMask = 0xff << (bo + width()); // all higher bits are 1
        quint8 completeMask = lowerMask | higherMask; //region in the middle is 0
        quint8 readByte = out->byte(address);
        quint8 maskedByte = readByte & completeMask;
        quint8 addedVal = val.ubyteValue << bo;
        quint8 newVal = maskedByte | addedVal;
        out->setByte(address, newVal);
    }
    else
    {
        quint8 firstByteMask = (1 << bo) - 1;
        quint8 firstByte = out->byte(address);
        quint8 firstByteMasked = firstByte & firstByteMask;
        quint8 firstAddedVal = (val.ubyteValue << bo);
        quint8 firstByteWithValAdded = firstByteMasked | firstAddedVal;
        out->setByte(address, firstByteWithValAdded);
        //if spans more than this one byte continue
        for (uint i = 8; i < width() + bo; i += 8)
        {
            quint8 readVal = out->byte(address + (i / 8));
            quint8 currentByte = val.ulongValue >> (i - bo);
            if (width() + bo <= i + 8)
            {
                //this is last byte needed, possibly cut off bottom
                quint8 missingBits = (width() + bo) % 8;
                quint8 mask = 0xff << missingBits;
                readVal &= mask; //remove the bottom values
                quint8 newValue = currentByte;
                readVal |= newValue;
                out->setByte(address + (i / 8), readVal);
            }
            else
            {
                //otherwise we need full byte -> nothing to do
                out->setByte(address + (i / 8), currentByte);
            }
        }
    }
}

void AbstractBitfieldDataInformation::setDataBigEndian(const AllPrimitiveTypes val,
        Okteta::AbstractByteArrayModel *out, Okteta::Address address,
        quint8* bitOffset)
{
    //maybe compiler can't optimise all that dereferencing away, also saves typing a few chars
    quint8 bo = *bitOffset;
    if (width() <= (unsigned) (8 - bo))
    {
        //fits completely
        quint8 lowerMask = 0xff >> (bo + width()); //all lower bits are 1
        quint8 higherMask = 0xff << (8 - bo); // all higher bits are 1
        quint8 completeMask = lowerMask | higherMask; //region in the middle is 0
        quint8 readByte = out->byte(address);
        quint8 maskedByte = readByte & completeMask;
        quint8 addedVal = val.ubyteValue << (8 - bo - 1); //move to missing area
        maskedByte |= addedVal;
        out->setByte(address, maskedByte);
    }
    else
    {
        quint8 missingBits = (width() + bo) % 8;
        quint8 lastAddress = address + ((bo + width()) / 8) - (missingBits > 0 ? 0 : 1);
        quint8 lastByte = out->byte(lastAddress);
        quint8 lastByteMask = (1 << missingBits) - 1;
        quint8 lastByteMasked = lastByte & lastByteMask; //remove the top values
        quint8 lastByteAddedVal = val.ubyteValue << (8 - missingBits);
        quint8 lastByteWithValAdded = lastByteMasked | lastByteAddedVal;
        out->setByte(lastAddress, lastByteWithValAdded);
        for (int currAddress = lastAddress - 1; currAddress >= address; currAddress--)
        {
            quint8 currentByte = out->byte(currAddress);
            if (currAddress == address)
            {
                //last byte to read
                quint8 firstByteMask = 0xff << (8 - bo);
                quint8 firstByteMasked = currentByte & firstByteMask;
                quint8 highestByte = val.ulongValue >> (bo + width() - 8);
                quint8 firstByteWithValAdded = firstByteMasked | highestByte;
                out->setByte(address, firstByteWithValAdded);
            }
            else
            {
                int bytesNotToShift = 1 + (lastAddress - address) - (lastAddress - currAddress);
                quint8 thisByteShifted = val.ulongValue >> (bo + width() - (8
                        * bytesNotToShift));
                out->setByte(currAddress, thisByteShifted);
            }
        }
    }
}

QString AbstractBitfieldDataInformation::sizeString() const
{
    return i18np("%1 bit", "%1 bits", width());
}
