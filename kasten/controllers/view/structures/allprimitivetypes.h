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

#include <QtGlobal>
#include <QByteArray>
#include <KLocale>

#include <size.h>
#include <address.h>

#include "structviewpreferences.h"
#include "datatypes/datainformationbase.h"

namespace Okteta
{
class AbstractByteArrayModel;
}
typedef Kasten2::StructViewPreferences::EnumByteOrder::type ByteOrder;
typedef Kasten2::StructViewPreferences::EnumByteOrder ByteOrderEnumClass;

#define compile_time_assert(e) extern char (*ct_assert(void)) [sizeof(char[1 - 2*!(e)])]

enum PrimitiveDataType
{
//!!! DO NOT CHANGE ORDER OF ITEMS !!!
    Type_NotPrimitive = -1,
    Type_START = 0,
    Type_Bool8 = 0,
    Type_Int8 = 1,
    Type_UInt8 = 2,
    Type_Char = 3,
    Type_Bool16 = 4,
    Type_Int16 = 5,
    Type_UInt16 = 6,
    Type_Bool32 = 7,
    Type_Int32 = 8,
    Type_UInt32 = 9,
    Type_Bool64 = 10,
    Type_Int64 = 11,
    Type_UInt64 = 12,
    Type_Float = 13,
    Type_Double = 14,
    Type_Bitfield = 15,
    Type_END = Type_Bitfield
};

struct PrimitiveType {
    static const QLatin1String typeNames[Type_END + 1];
    static const char* longTypeNames[Type_END + 1];
};

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
    bool writeBits(const quint8 bitCount, const AllPrimitiveTypes newValue,
            Okteta::AbstractByteArrayModel* out, const ByteOrder byteOrder,
            const Okteta::Address address, const BitCount64 bitsRemaining,
            quint8* const bitOffset);
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
    bool readBits(const quint8 bitCount,
            const Okteta::AbstractByteArrayModel* input, const ByteOrder byteOrder,
            const Okteta::Address address, const BitCount64 bitsRemaining,
            quint8* const bitOffset);

    template<typename T> T value() const;

private:
    void readDataLittleEndian(const quint8 bitCount,
            const Okteta::AbstractByteArrayModel* input,
            const Okteta::Address address, const quint8 bo);
    void writeDataLittleEndian(const quint8 bitCount,
            const AllPrimitiveTypes newValue, Okteta::AbstractByteArrayModel *out,
            const Okteta::Address address, const quint8 bo) const;

    void readDataBigEndian(const quint8 bitCount,
            const Okteta::AbstractByteArrayModel* input,
            const Okteta::Address address, const quint8 bo);
    void writeDataBigEndian(const quint8 bitCount, const AllPrimitiveTypes newValue,
            Okteta::AbstractByteArrayModel *out, const Okteta::Address address,
            const quint8 bo) const;

    //optimised methods for reading/writing full bytes
    void readFullBytes(const quint8 byteCount,
            const Okteta::AbstractByteArrayModel* input, const ByteOrder byteOrder,
            const Okteta::Address address);
    void writeFullBytes(const quint8 byteCount, const AllPrimitiveTypes newValue,
            Okteta::AbstractByteArrayModel* out, const ByteOrder byteOrder,
            const Okteta::Address address);
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


#endif /* ALLPRIMITIVETYPES_H_ */
