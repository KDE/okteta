/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddata.hpp"

namespace Okteta {

static inline void copyInvertedBytes(Byte* data, const Byte* sourceData, const int length)
{
    const int last = length - 1;
    for (int i = 0, j = last; i < length; ++i, --j) {
        data[i] = sourceData[j];
    }
}

PODData::PODData() = default;

const Byte* PODData::originalData()     const { return mCurrentOriginalData; }
const Byte* PODData::byteOrderSetData() const { return mCurrentEndiannessSetData; }
QSysInfo::Endian PODData::byteOrder()          const { return mByteOrder; }
int PODData::size()                     const { return mCurrentSize; }

Byte* PODData::rawData()    { return mOriginalAligned64Bit.mBytes; }

void PODData::setByteOrder(QSysInfo::Endian byteOrder)
{
    if (mByteOrder == byteOrder) {
        return;
    }

    mByteOrder = byteOrder;

    // swap data
    if (mCurrentOriginalData) {
        copyInvertedBytes(mByteOrderSetAligned64Bit.mBytes, mOriginalAligned64Bit.mBytes, Size);
    } else {
        mByteOrderSetAligned64Bit = mOriginalAligned64Bit;
    }
}

unsigned long PODData::bitValue(int noOfBitsToRead) const
{
    constexpr int BitsPerByte = 8;
    constexpr unsigned char BitMask[9] = {
        0, 1 << 7, 3 << 6, 7 << 5, 15 << 4, 31 << 3, 63 << 2, 127 << 1, 255
    };

    unsigned long result = 0;

    if (noOfBitsToRead < 1) {
        noOfBitsToRead = 1;
    }

    // TODO: the cursor currently does not go into the byte
    int noOfUsedBits = 0;// 7 - state.cell;

    const bool isReverse = (mByteOrder != QSysInfo::ByteOrder);
    const Byte* data = mByteOrderSetAligned64Bit.mBytes;
    if (isReverse) {
        data += 7;
    }

    while (noOfBitsToRead > 0) {
        Byte byte = (*data << noOfUsedBits);

        const int noOfNextBits =
            ((BitsPerByte - noOfUsedBits) >= noOfBitsToRead) ? noOfBitsToRead : (BitsPerByte - noOfUsedBits);

        byte &= BitMask[noOfNextBits];

        byte >>= BitsPerByte - noOfNextBits;

        result = (result << noOfNextBits) | byte;

        noOfBitsToRead -= noOfNextBits;
        noOfUsedBits += noOfNextBits;

        if (noOfUsedBits >= BitsPerByte) {
            noOfUsedBits = 0;
            if (isReverse) {
                --data;
            } else {
                ++data;
            }
        }
    }

    return result;
}

bool PODData::updateRawData(int size)
{
    const Byte* oldCurrentOriginalData = mCurrentOriginalData;

    if (size > 0) {
        if (mByteOrder != QSysInfo::ByteOrder) {
            copyInvertedBytes(mByteOrderSetAligned64Bit.mBytes, mOriginalAligned64Bit.mBytes, Size);
        } else {
            mByteOrderSetAligned64Bit = mOriginalAligned64Bit;
        }

        mCurrentOriginalData = mOriginalAligned64Bit.mBytes;
        mCurrentEndiannessSetData = mByteOrderSetAligned64Bit.mBytes;
        mCurrentSize = size;
    } else {
        mCurrentOriginalData = nullptr;
        mCurrentEndiannessSetData = nullptr;
        mCurrentSize = 0;
    }

    return (mCurrentOriginalData || oldCurrentOriginalData);
}

void PODData::getPointers(const void** P8Bit, const void** P16Bit, const void** P32Bit, const void** P64Bit) const
{
    constexpr int MachineOffsets[4] = { 0, 0, 0, 0 };
    constexpr int ReversedOffsets[4] = { 7, 6, 4, 0 };

    const int* offsets = (mByteOrder == QSysInfo::ByteOrder) ? MachineOffsets : ReversedOffsets;
    const Byte* data = mByteOrderSetAligned64Bit.mBytes;

    *P8Bit =  (mCurrentSize >= 1) ? data + offsets[0] : nullptr;
    *P16Bit = (mCurrentSize >= 2) ? data + offsets[1] : nullptr;
    *P32Bit = (mCurrentSize >= 4) ? data + offsets[2] : nullptr;
    *P64Bit = (mCurrentSize >= 8) ? data + offsets[3] : nullptr;
}

const void* PODData::pointer(int byteCount) const
{
    if (byteCount > mCurrentSize) {
        byteCount = 0;
    }

    const int offset = (mByteOrder == QSysInfo::ByteOrder) ? 0 : 8 - byteCount;
    const Byte* data = mByteOrderSetAligned64Bit.mBytes;

    return (byteCount > 0) ? data + offset : nullptr;
}

}
