/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef ALLPRIMITIVETYPES_H_
#define ALLPRIMITIVETYPES_H_

#include <QSysInfo>
#include <QtEndian>

#include <size.h>
#include <address.h>
#include <abstractbytearraymodel.h>

#include "datatypes/datainformationbase.h"

namespace Okteta
{
class AbstractByteArrayModel;
}

/** This union holds the value of one primitive datatype. Maximum size of a datatype is currently 64 bits.
 *  It has methods for reading and writing from @c Okteta::AbstractByteArrayModel */
union AllPrimitiveTypes
{
    qint64 longValue;
    quint64 ulongValue;
    qint32 intValue;
    quint32 uintValue;
    qint16 shortValue;
    quint16 ushortValue;
    qint8 byteValue;
    quint8 ubyteValue;
    float floatValue;
    double doubleValue;
    qint8 allBytes[8];
    inline AllPrimitiveTypes() :
        ulongValue(0)
    {
    }
    inline AllPrimitiveTypes(const AllPrimitiveTypes& a)
        : ulongValue(a.ulongValue)
    {
    }
    inline AllPrimitiveTypes(quint64 val) :
        ulongValue(val)
    {
    }
    inline AllPrimitiveTypes(qint64 val) :
        longValue(val)
    {
    }
    //set all to zero first with smaller data types
    inline AllPrimitiveTypes(qint32 val) :
        longValue(val < 0 ? -1 : 0)
    {
        intValue = val;
    }
    inline AllPrimitiveTypes(quint32 val) :
        ulongValue(0)
    {
        uintValue = val;

    }
    inline AllPrimitiveTypes(qint16 val) :
        longValue(val < 0 ? -1 : 0)
    {
        shortValue = val;

    }
    inline AllPrimitiveTypes(quint16 val) :
        ulongValue(0)
    {
        ushortValue = val;

    }
    inline AllPrimitiveTypes(qint8 val) :
        longValue(val < 0 ? -1 : 0)
    {
        byteValue = val;
    }
    inline AllPrimitiveTypes(quint8 val) :
        ulongValue(0)
    {
        ubyteValue = val;

    }
    inline AllPrimitiveTypes(float val) :
        ulongValue(0)
    {
        floatValue = val;
    }
    inline AllPrimitiveTypes(double val) :
        doubleValue(val)
    {
    }
    inline bool operator!=(AllPrimitiveTypes other) const
    {
        return ulongValue != other.ulongValue;
    }
    inline bool operator==(AllPrimitiveTypes other) const
    {
        return ulongValue == other.ulongValue;
    }
    inline bool operator<(AllPrimitiveTypes other) const
    {
        return this->ulongValue < other.ulongValue;
    }
    /** Writes given number of bits to @p out.
     *  If the value of this union is not equal to @p newValue it is set to @p newValue.
     *  @p bitOffset is changed in this method so it does not have to be handled later.
     *  There is no need to write an optimised version of this method for reading complete bytes
     *  since this is already handled internally.
     *
     *  On failure value of this union is set to @c 0.
     *
     *  @param bitCount the number of bits to read
     *  @param newValue the new value of this union
     *  @param out the byte array the value is read from
     *  @param byteOrder the byteOrder used for reading values
     *  @param address the address in @p out
     *  @param remaining number of bytes remaining in @p input
     *  @param bitOffset the bit to start at in the first byte
     *  @return @c true on success, @c false otherwise
     */
    bool writeBits(quint8 bitCount, AllPrimitiveTypes newValue,
            Okteta::AbstractByteArrayModel* out, QSysInfo::Endian byteOrder,
            Okteta::Address address, BitCount64 bitsRemaining, quint8* const bitOffset);
    /** Reads given number of bits from @p input and sets value of this union to
     *  the new value.
     *  @p bitOffset is changed in this method so it does not have to be handled later.
     *  There is no need to write an optimised version of this method for reading complete bytes
     *  since this is already handled internally.
     *
     *  On failure value of this union is set to @c 0.
     *
     *  @param bitCount the number of bits to read
     *  @param input the byte array the value is read from
     *  @param byteOrder the byteOrder used for reading values
     *  @param address the address in @p input
     *  @param bitsRemaining number of bytes remaining in @p input
     *  @param bitOffset the bit to start at in the first byte
     *  @return @c true on success, @c false otherwise
     */
    bool readBits(quint8 bitCount, const Okteta::AbstractByteArrayModel* input,
            QSysInfo::Endian byteOrder, Okteta::Address address, BitCount64 bitsRemaining,
            quint8* const bitOffset);

    template<typename T> T value() const;
    /**
     * Read data of type @p T from the model. Range checking must have been performed before
     * @param input the input to read from
     * @param address the starting address
     * @param endianess the endianess to use when reading
     * @param bitOffset the number of bits into the first byte (different depending on endianess)
     * @return the read value
     */
    //TODO bool* ok parameter for when reading from model can cause errors (or exceptions sometime?)
    template<typename T> static T readValue(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            QSysInfo::Endian endianess, quint8 bitOffset);
    //TODO add writeValue

private:
    template<int size> static typename QIntegerForSize<size>::Unsigned readValuePrivate(
            const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            QSysInfo::Endian endianess, quint8 bitOffset);
    template<int size> static typename QIntegerForSize<size>::Unsigned readRawBytes(
            const Okteta::AbstractByteArrayModel* input, Okteta::Address address);

    void readDataLittleEndian(quint8 bitCount, const Okteta::AbstractByteArrayModel* input,
            Okteta::Address address, quint8 bo);
    void writeDataLittleEndian(quint8 bitCount, AllPrimitiveTypes newValue,
            Okteta::AbstractByteArrayModel *out, Okteta::Address address, quint8 bo) const;

    void readDataBigEndian(quint8 bitCount, const Okteta::AbstractByteArrayModel* input,
            Okteta::Address address, quint8 bo);
    void writeDataBigEndian(quint8 bitCount, AllPrimitiveTypes newValue,
            Okteta::AbstractByteArrayModel *out, Okteta::Address address, quint8 bo) const;

    //optimised methods for reading/writing full bytes
    void readFullBytes(quint8 byteCount, const Okteta::AbstractByteArrayModel* input,
            QSysInfo::Endian byteOrder, Okteta::Address address);
    void writeFullBytes(quint8 byteCount, AllPrimitiveTypes newValue,
            Okteta::AbstractByteArrayModel* out, QSysInfo::Endian byteOrder,
            Okteta::Address address);
};

template<> inline quint8 AllPrimitiveTypes::value<quint8>() const { return ubyteValue; }
template<> inline quint16 AllPrimitiveTypes::value<quint16>() const { return ushortValue; }
template<> inline quint32 AllPrimitiveTypes::value<quint32>() const { return uintValue; }
template<> inline quint64 AllPrimitiveTypes::value<quint64>() const { return ulongValue; }
template<> inline qint8 AllPrimitiveTypes::value<qint8>() const { return byteValue; }
template<> inline qint16 AllPrimitiveTypes::value<qint16>() const { return shortValue; }
template<> inline qint32 AllPrimitiveTypes::value<qint32>() const { return intValue; }
template<> inline qint64 AllPrimitiveTypes::value<qint64>() const { return longValue; }
template<> inline float AllPrimitiveTypes::value<float>() const { return floatValue; }
template<> inline double AllPrimitiveTypes::value<double>() const { return doubleValue; }


template<typename T>
inline T AllPrimitiveTypes::readValue(const Okteta::AbstractByteArrayModel* input,
        Okteta::Address address, QSysInfo::Endian endianess, quint8 bitOffset)
{
    //check for out of bounds
    Q_ASSERT(BitCount64(input->size() - address) * 8 - bitOffset >= sizeof(T) * 8);
    Q_ASSERT(bitOffset < 8);
    //this union exists to force unsigned shifts
    union {
        T value;
        typename QIntegerForSizeof<T>::Unsigned unsignedValue;
    } u;
    u.unsignedValue = readValuePrivate<sizeof(T)>(input, address, endianess, bitOffset);
    return u.value;
}

template<int size>
inline typename QIntegerForSize<size>::Unsigned AllPrimitiveTypes::readValuePrivate(
        const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        QSysInfo::Endian endianess, quint8 bitOffset)
{
    typename QIntegerForSize<size>::Unsigned unsignedValue = readRawBytes<size>(input, address);
    if (endianess != QSysInfo::ByteOrder)
    {
        //swap the byte order if machine endianess does not match requested endianess
        unsignedValue = qbswap(unsignedValue);
    }
    if (Q_UNLIKELY(bitOffset != 0))
    {
        quint8 lastByte = input->byte(address + size);
        //handle the remaining bits
        if (endianess == QSysInfo::BigEndian)
        {
            //the coming bits are the least significant, and range from bit (8-bitOffset)..7
            unsignedValue <<= bitOffset;
            lastByte >>= 8 - bitOffset; //unsigned shift
            Q_ASSERT((unsignedValue & lastByte) == 0); //must not overlap
            unsignedValue |= lastByte;
        }
        else
        {
            //the coming bits are the most significant bits and range from 0..bitOffset
            unsignedValue >>= bitOffset;
            //promote lastByte to unsigned T and mask off the interesting bits
            typename QIntegerForSize<size>::Unsigned tmp = lastByte & ((1u << bitOffset) - 1);
            tmp <<= (size * 8) - bitOffset;
            unsignedValue |= tmp;
        }
    }
    return unsignedValue;
}

template<int size>
inline typename QIntegerForSize<size>::Unsigned AllPrimitiveTypes::readRawBytes(
        const Okteta::AbstractByteArrayModel* input, Okteta::Address address)
{
    union {
        typename QIntegerForSize<size>::Unsigned value;
        Okteta::Byte bytes[size];
    } buf;
    Okteta::Size read = input->copyTo(buf.bytes, address, size);
    Q_ASSERT(read == size);
    return buf.value;
}

//specialize it for the case where we only need to read one byte
template<>
inline quint8 AllPrimitiveTypes::readRawBytes<1>(
        const Okteta::AbstractByteArrayModel* input, Okteta::Address address)
{
    return input->byte(address);
}

#endif /* ALLPRIMITIVETYPES_H_ */
