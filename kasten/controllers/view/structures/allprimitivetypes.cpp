/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "allprimitivetypes.hpp"

#include <Okteta/AbstractByteArrayModel>

Q_STATIC_ASSERT(sizeof(double) == 8);
Q_STATIC_ASSERT(sizeof(float) == 4);
Q_STATIC_ASSERT(sizeof(AllPrimitiveTypes) == 8);

// FIXME this code really needs unit tests!
// TODO optimised methods for *bitOffset == 0 && bitCount % 8 == 0

bool AllPrimitiveTypes::writeBits(quint8 bitCount, AllPrimitiveTypes newValue,
                                  Okteta::AbstractByteArrayModel* out, QSysInfo::Endian byteOrder, Okteta::Address address,
                                  BitCount64 bitsRemaining, quint8* const bitOffset)
{
    Q_ASSERT(*bitOffset < 8);
    Q_ASSERT(bitCount <= 64);
    if (bitsRemaining < bitCount) {
        _ulong.value = 0;
        *bitOffset = 0;
        return false;
    }
    // set if not
    if (_ulong.value != newValue._ulong.value) {
        _ulong.value = newValue._ulong.value;
    }

    if (bitCount % 8 == 0 && *bitOffset == 0) {
        // only writing full bytes
        writeFullBytes(bitCount / 8, newValue, out, byteOrder, address);
    } else {
        if (byteOrder == QSysInfo::LittleEndian) {
            writeDataLittleEndian(bitCount, newValue, out, address, *bitOffset);
        } else if (byteOrder == QSysInfo::BigEndian) {
            writeDataBigEndian(bitCount, newValue, out, address, *bitOffset);
        } else {
            Q_ASSERT(false);
            return false;
        }
        *bitOffset = (*bitOffset + bitCount) % 8;
    }
    return true;
}

bool AllPrimitiveTypes::readBits(quint8 bitCount, const Okteta::AbstractByteArrayModel* input,
                                 QSysInfo::Endian byteOrder, Okteta::Address address, BitCount64 bitsRemaining,
                                 quint8* const bitOffset)
{
    Q_ASSERT(bitCount <= 64);
    Q_ASSERT(*bitOffset < 8);
    if (bitsRemaining < bitCount) {
        _ulong.value = 0;
        *bitOffset = 0;
        return false;
    }
    // set to zero before reading
    _ulong.value = 0;
    if (bitCount % 8 == 0 && *bitOffset == 0) {
        // only reading full bytes
        readFullBytes(bitCount / 8, input, byteOrder, address);
    } else {
        if (byteOrder == QSysInfo::LittleEndian) {
            readDataLittleEndian(bitCount, input, address, *bitOffset);
        } else if (byteOrder == QSysInfo::BigEndian) {
            readDataBigEndian(bitCount, input, address, *bitOffset);
        } else {
            Q_ASSERT(false);
            return false;
        }
        *bitOffset = (*bitOffset + bitCount) % 8;
    }
    return true;
}

void AllPrimitiveTypes::readDataLittleEndian(quint8 bitCount,
                                             const Okteta::AbstractByteArrayModel* input, Okteta::Address address, quint8 bo)
{
    if (bitCount <= (unsigned) (8 - bo)) {
        // fits completely
        const quint8 lowerMask = 0xff << bo; // all lower bits are 0
        const quint8 higherMask = 0xff >> (8 - (bo + bitCount)); // all higher bits are 0
        const quint8 completeMask = lowerMask & higherMask; // region in the middle
        const quint8 readByte = input->byte(address);
        const quint8 maskedByte = readByte & completeMask;
        _ubyte.value = maskedByte >> bo;
    } else {
        const quint8 firstByteMask = 0xff << bo;
        const quint8 firstByte = input->byte(address);
        const quint8 firstByteMasked = firstByte & firstByteMask;
        _ubyte.value = firstByteMasked >> bo;
        // if spans more than this one byte continue
        for (uint i = 8; i < bitCount + bo; i += 8) {
            quint8 readVal = input->byte(address + (i / 8));
            if (bitCount + bo < i + 8) {
                // this is last byte needed, possibly cut off top values
                const quint8 missingBits = (bitCount + bo) % 8;
                const quint8 mask = (1 << missingBits) - 1;
                readVal &= mask; // mask the top few bits
            }
            // otherwise we need full byte -> nothing to do
            // needs cast since otherwise compiler decides to use 32 bit int and top 32 bits get lost
            const quint64 shiftedVal = (quint64) readVal << i;
            _ulong.value |= shiftedVal >> bo; // move to correct byte
        }
    }
}

void AllPrimitiveTypes::readDataBigEndian(quint8 bitCount,
                                          const Okteta::AbstractByteArrayModel* input, Okteta::Address address, quint8 bo)
{
    if (bitCount <= (unsigned) (8 - bo)) {
        const quint8 lowerMask = 0xff << (8 - (bo + bitCount));
        const quint8 higherMask = 0xff >> bo;
        const quint8 completeMask = lowerMask & higherMask;
        // completeMask maskes the value -> negate it to clear all the bytes
        const quint8 readByte = input->byte(address);
        const quint8 maskedByte = readByte & completeMask;
        _ubyte.value = maskedByte >> (8 - (bo + bitCount));
    } else {
        const quint8 firstByteMask = 0xff >> bo;
        const quint8 firstByte = input->byte(address);
        // needs quint64 since otherwise compiler decides to use 32 bit int when shifting and top 32 bits get lost
        const quint64 firstByteMasked = firstByte & firstByteMask;
        const quint64 firstByteShifted = firstByteMasked << (bo + bitCount - 8);
        _ulong.value = firstByteShifted;
        // if spans more than this one byte continue
        for (uint i = 8; i < bitCount + bo; i += 8) {
            quint8 readVal = input->byte(address + (i / 8));
            if (bitCount + bo < i + 8) {
                // this is last byte needed, possibly cut off lower values
                const quint8 missingBits = (bo + bitCount) % 8;
                const quint8 mask = 0xff << (8 - missingBits);
                const quint8 maskedVal = readVal & mask; // cut off lower bits
                const quint8 shiftedVal = maskedVal >> (8 - missingBits);
                _ulong.value |= shiftedVal;
            } else {
                // otherwise we need full byte -> nothing to do
                // needs cast since otherwise compiler decides to use 32 bit int and top 32 bits get lost
                const quint64 shiftedVal = (quint64) readVal << ((bo + bitCount)
                                                                   - (8 + i)); // move to correct byte
                _ulong.value |= shiftedVal;
            }
        }
    }
}

void AllPrimitiveTypes::writeDataLittleEndian(quint8 bitCount,
                                              AllPrimitiveTypes newValue, Okteta::AbstractByteArrayModel* out,
                                              Okteta::Address address, quint8 bo) const
{
    if (bitCount <= (unsigned) (8 - bo)) {
        // fits completely
        const quint8 lowerMask = (1 << bo) - 1; // all lower bits are 1
        const quint8 higherMask = 0xff << (bo + bitCount); // all higher bits are 1
        const quint8 completeMask = lowerMask | higherMask; // region in the middle is 0
        const quint8 readByte = out->byte(address);
        const quint8 maskedByte = readByte & completeMask;
        const quint8 addedVal = newValue._ubyte.value << bo;
        const quint8 newVal = maskedByte | addedVal;
        out->setByte(address, newVal);
    } else {
        const quint8 firstByteMask = (1 << bo) - 1;
        const quint8 firstByte = out->byte(address);
        const quint8 firstByteMasked = firstByte & firstByteMask;
        const quint8 firstAddedVal = (newValue._ubyte.value << bo);
        const quint8 firstByteWithValAdded = firstByteMasked | firstAddedVal;
        out->setByte(address, firstByteWithValAdded);
        // if spans more than this one byte continue
        for (uint i = 8; i < bitCount + bo; i += 8) {
            const quint8 currentByte = newValue._ulong.value >> (i - bo);
            if (bitCount + bo < i + 8) {
                const quint8 readVal = out->byte(address + (i / 8));
                // this is last byte needed, possibly cut off bottom
                const quint8 missingBits = (bitCount + bo) % 8;
                const quint8 mask = 0xff << missingBits;
                const quint8 readValMasked = readVal & mask; // remove the bottom values
                const quint8 resultingVal = readValMasked | currentByte;
                out->setByte(address + (i / 8), resultingVal);
            } else {
                // otherwise we need full byte -> nothing to do
                out->setByte(address + (i / 8), currentByte);
            }
        }
    }
}

void AllPrimitiveTypes::writeDataBigEndian(quint8 bitCount,
                                           AllPrimitiveTypes newValue, Okteta::AbstractByteArrayModel* out,
                                           Okteta::Address address, quint8 bo) const
{
    if (bitCount <= (unsigned) (8 - bo)) {
        // fits completely
        const quint8 lowerMask = 0xff >> (bo + bitCount); // all lower bits are 1
        const quint8 higherMask = 0xff << (8 - bo); // all higher bits are 1
        const quint8 completeMask = lowerMask | higherMask; // region in the middle is 0
        const quint8 readByte = out->byte(address);
        const quint8 maskedByte = readByte & completeMask;
        const quint8 addedVal = newValue._ubyte.value << (8 - bo - 1); // move to missing area
        const quint8 maskedByteWithValueAdded = maskedByte | addedVal;
        out->setByte(address, maskedByteWithValueAdded);
    } else {
        quint8 missingBits = (bitCount + bo) % 8;
        missingBits = (missingBits == 0 ? 8 : missingBits);
        const quint8 lastAddress = address + ((bo + bitCount) / 8) - (missingBits
                                                                      > 0 ? 0 : 1);
        const quint8 lastByte = out->byte(lastAddress);
        const quint8 lastByteMask = (1 << missingBits) - 1;
        const quint8 lastByteMasked = lastByte & lastByteMask; // remove the top values
        const quint8 lastByteAddedVal = newValue._ubyte.value << (8 - missingBits);
        const quint8 lastByteWithValAdded = lastByteMasked | lastByteAddedVal;
        out->setByte(lastAddress, lastByteWithValAdded);
        for (int currAddress = lastAddress - 1; currAddress >= address; currAddress--) {
            const quint8 currentByte = out->byte(currAddress);
            if (currAddress == address) {
                // last byte to read
                const quint8 firstByteMask = 0xff << (8 - bo);
                const quint8 firstByteMasked = currentByte & firstByteMask;
                const quint8 highestByte = newValue._ulong.value
                                           >> (bo + bitCount - 8);
                const quint8 firstByteWithValAdded = firstByteMasked | highestByte;
                out->setByte(address, firstByteWithValAdded);
            } else {
                const int bytesNotToShift = 1 + (lastAddress - address)
                                            - (lastAddress - currAddress);
                const quint8 thisByteShifted = newValue._ulong.value >> (bo + bitCount
                                                                         - (8 * bytesNotToShift));
                out->setByte(currAddress, thisByteShifted);
            }
        }
    }
}

void AllPrimitiveTypes::readFullBytes(quint8 byteCount, const Okteta::AbstractByteArrayModel* input,
                                      QSysInfo::Endian byteOrder, Okteta::Address address)
{
    Q_ASSERT(byteCount <= 8);
    // always use unsigned value
    for (int i = 0; i < byteCount; i++) {
        int index = (byteOrder == QSysInfo::LittleEndian) ? i : ((byteCount - 1) - i);
        Okteta::Byte readByte = input->byte(address + i);
        allBytes[index] = readByte;
    }
}

void AllPrimitiveTypes::writeFullBytes(quint8 byteCount, AllPrimitiveTypes newValue,
                                       Okteta::AbstractByteArrayModel* out, QSysInfo::Endian byteOrder, Okteta::Address address)
{
    Q_ASSERT(byteCount <= 8);
    for (int i = 0; i < byteCount; ++i) {
        int index = (byteOrder == QSysInfo::LittleEndian) ? i : ((byteCount - 1) - i);
        out->setByte(address + i, newValue.allBytes[index]);
    }
}
