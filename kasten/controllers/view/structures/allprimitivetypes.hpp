/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ALLPRIMITIVETYPES_HPP
#define KASTEN_ALLPRIMITIVETYPES_HPP

#include <QSysInfo>
#include <QtEndian>

#include <size.hpp>
#include <Okteta/Address>
#include <Okteta/AbstractByteArrayModel>

#include "datatypes/datainformationbase.hpp"

namespace Okteta {
class AbstractByteArrayModel;
}

#ifdef Q_CC_GNU
#define PACKED_STRUCT __attribute__((packed, aligned(8)))
#else
#define PACKED_STRUCT
#endif

/** Ensures that when used in a union the uint8 value will be equal to the lowest bits of the uint32 value
 * This means we need to add padding equal to 8-sizeof(T) before the value in the big endian case.
 * On little endian padding gets added at the end (not strictly necessary)
 */
template <typename T, int padCount>
struct EndianIndependentBase
{
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    char padding[padCount];
#endif
    T value;
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    char padding[padCount];
#endif
} PACKED_STRUCT;
template <typename T>
struct EndianIndependentBase<T, 0>
{
    T value;
};

template <typename T>
struct EndianIndependent : public EndianIndependentBase<T
        , 8 - sizeof(T)>
{
};

/** This union holds the value of one primitive datatype. Maximum size of a datatype is currently 64 bits.
 *  It has methods for reading and writing from @c Okteta::AbstractByteArrayModel */
union AllPrimitiveTypes
{
private:
    EndianIndependent<qint8> _byte;
    EndianIndependent<quint8> _ubyte;
    EndianIndependent<qint16> _short;
    EndianIndependent<quint16> _ushort;
    EndianIndependent<qint32> _int;
    EndianIndependent<quint32> _uint;
    EndianIndependent<qint64> _long;
    EndianIndependent<quint64> _ulong;
    EndianIndependent<float> _float;
    EndianIndependent<double> _double;

public:
    qint8 allBytes[8];
    inline AllPrimitiveTypes() { _ulong.value = 0; }
    inline AllPrimitiveTypes(const AllPrimitiveTypes &a) { _ulong.value = a._ulong.value; }
    inline AllPrimitiveTypes(quint64 val) { _ulong.value = val; }
    inline AllPrimitiveTypes(qint64 val) { _long.value = val; }
    // set all to zero first with smaller data types
    inline AllPrimitiveTypes(qint32 val) { _long.value = (val < 0 ? -1 : 0); _int.value = val; }
    inline AllPrimitiveTypes(quint32 val) { _ulong.value = 0; _uint.value = val; }
    inline AllPrimitiveTypes(qint16 val) { _long.value = (val < 0 ? -1 : 0);  _short.value = val; }
    inline AllPrimitiveTypes(quint16 val) { _ulong.value = 0; _ushort.value = val; }
    inline AllPrimitiveTypes(qint8 val) { _long.value = (val < 0 ? -1 : 0); _byte.value = val; }
    inline AllPrimitiveTypes(quint8 val) { _ulong.value = 0; _ubyte.value = val; }
    inline AllPrimitiveTypes(float val) { _ulong.value = 0; _float.value = val; }
    inline AllPrimitiveTypes(double val) { _double.value = val; }
    inline ~AllPrimitiveTypes() = default;

    inline AllPrimitiveTypes& operator=(const AllPrimitiveTypes &a)
    { _ulong.value = a._ulong.value; return *this; }
    inline bool operator!=(AllPrimitiveTypes other) const
    {
        return _ulong.value != other._ulong.value;
    }
    inline bool operator==(AllPrimitiveTypes other) const
    {
        return _ulong.value == other._ulong.value;
    }
    /** Not useful, but needed so we can store this in a QMap */
    inline bool operator<(AllPrimitiveTypes other) const
    {
        return _ulong.value < other._ulong.value;
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
     *  @param bitsRemaining remaining number of bits remaining in @p input
     *  @param bitOffset the bit to start at in the first byte
     *  @return @c true on success, @c false otherwise
     */
    bool writeBits(quint8 bitCount, AllPrimitiveTypes newValue,
                   Okteta::AbstractByteArrayModel* out, QSysInfo::Endian byteOrder,
                   Okteta::Address address, BitCount64 bitsRemaining, quint8 * const bitOffset);
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
                  quint8 * const bitOffset);

    template <typename T> T value() const;
    /**
     * Read data of type @p T from the model. Range checking must have been performed before
     * @param input the input to read from
     * @param address the starting address
     * @param endianess the endianess to use when reading
     * @param bitOffset the number of bits into the first byte (different depending on endianess)
     * @return the read value
     */
    // TODO bool* ok parameter for when reading from model can cause errors (or exceptions sometime?)
    template <typename T> static T readValue(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        QSysInfo::Endian endianess, quint8 bitOffset);
    // TODO add writeValue

private:
    template <int size> static typename QIntegerForSize<size>::Unsigned readValuePrivate(
        const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        QSysInfo::Endian endianess, quint8 bitOffset);
    template <int size> static typename QIntegerForSize<size>::Unsigned readRawBytes(
        const Okteta::AbstractByteArrayModel* input, Okteta::Address address);

    void readDataLittleEndian(quint8 bitCount, const Okteta::AbstractByteArrayModel* input,
                              Okteta::Address address, quint8 bo);
    void writeDataLittleEndian(quint8 bitCount, AllPrimitiveTypes newValue,
                               Okteta::AbstractByteArrayModel* out, Okteta::Address address, quint8 bo) const;

    void readDataBigEndian(quint8 bitCount, const Okteta::AbstractByteArrayModel* input,
                           Okteta::Address address, quint8 bo);
    void writeDataBigEndian(quint8 bitCount, AllPrimitiveTypes newValue,
                            Okteta::AbstractByteArrayModel* out, Okteta::Address address, quint8 bo) const;

    // optimised methods for reading/writing full bytes
    void readFullBytes(quint8 byteCount, const Okteta::AbstractByteArrayModel* input,
                       QSysInfo::Endian byteOrder, Okteta::Address address);
    void writeFullBytes(quint8 byteCount, AllPrimitiveTypes newValue,
                        Okteta::AbstractByteArrayModel* out, QSysInfo::Endian byteOrder,
                        Okteta::Address address);
};

template <> inline quint8 AllPrimitiveTypes::value<quint8>() const { return _ubyte.value; }
template <> inline quint16 AllPrimitiveTypes::value<quint16>() const { return _ushort.value; }
template <> inline quint32 AllPrimitiveTypes::value<quint32>() const { return _uint.value; }
template <> inline quint64 AllPrimitiveTypes::value<quint64>() const { return _ulong.value; }
template <> inline qint8 AllPrimitiveTypes::value<qint8>() const { return _byte.value; }
template <> inline qint16 AllPrimitiveTypes::value<qint16>() const { return _short.value; }
template <> inline qint32 AllPrimitiveTypes::value<qint32>() const { return _int.value; }
template <> inline qint64 AllPrimitiveTypes::value<qint64>() const { return _long.value; }
template <> inline float AllPrimitiveTypes::value<float>() const { return _float.value; }
template <> inline double AllPrimitiveTypes::value<double>() const { return _double.value; }

template <typename T>
inline T AllPrimitiveTypes::readValue(const Okteta::AbstractByteArrayModel* input,
                                      Okteta::Address address, QSysInfo::Endian endianess, quint8 bitOffset)
{
    // check for out of bounds
    Q_ASSERT(BitCount64(input->size() - address) * 8 - bitOffset >= sizeof(T) * 8);
    Q_ASSERT(bitOffset < 8);
    // this union exists to force unsigned shifts
    union {
        T value;
        typename QIntegerForSizeof<T>::Unsigned unsignedValue;
    } u;
    u.unsignedValue = readValuePrivate<sizeof(T)>(input, address, endianess, bitOffset);
    return u.value;
}

template <int size>
inline typename QIntegerForSize<size>::Unsigned AllPrimitiveTypes::readValuePrivate(
    const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
    QSysInfo::Endian endianess, quint8 bitOffset)
{
    typename QIntegerForSize<size>::Unsigned unsignedValue = readRawBytes<size>(input, address);
    if (endianess != QSysInfo::ByteOrder) {
        // swap the byte order if machine endianess does not match requested endianess
        unsignedValue = qbswap(unsignedValue);
    }
    if (Q_UNLIKELY(bitOffset != 0)) {
        quint8 lastByte = input->byte(address + size);
        // handle the remaining bits
        if (endianess == QSysInfo::BigEndian) {
            // the coming bits are the least significant, and range from bit (8-bitOffset)..7
            unsignedValue <<= bitOffset;
            lastByte >>= 8 - bitOffset; // unsigned shift
            Q_ASSERT((unsignedValue & lastByte) == 0); // must not overlap
            unsignedValue |= lastByte;
        } else {
            // the coming bits are the most significant bits and range from 0..bitOffset
            unsignedValue >>= bitOffset;
            // promote lastByte to unsigned T and mask off the interesting bits
            typename QIntegerForSize<size>::Unsigned tmp = lastByte & ((1u << bitOffset) - 1);
            tmp <<= (size * 8) - bitOffset;
            unsignedValue |= tmp;
        }
    }
    return unsignedValue;
}

template <int size>
inline typename QIntegerForSize<size>::Unsigned AllPrimitiveTypes::readRawBytes(
    const Okteta::AbstractByteArrayModel* input, Okteta::Address address)
{
    union {
        typename QIntegerForSize<size>::Unsigned value;
        Okteta::Byte bytes[size];
    } buf;
    Okteta::Size read = input->copyTo(buf.bytes, address, size);
    Q_ASSERT(read == size);
    Q_UNUSED(read)
    return buf.value;
}

// specialize it for the case where we only need to read one byte
template <>
inline quint8 AllPrimitiveTypes::readRawBytes<1>(
    const Okteta::AbstractByteArrayModel* input, Okteta::Address address)
{
    return input->byte(address);
}

#endif /* KASTEN_ALLPRIMITIVETYPES_HPP */
