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

#include <size.h>
#include <address.h>

#include "structviewpreferences.h"

namespace Okteta
{
class AbstractByteArrayModel;
}
typedef Kasten::StructViewPreferences::EnumByteOrder::type ByteOrder;
typedef Kasten::StructViewPreferences::EnumByteOrder ByteOrderEnumClass;

//TODO remove sometime
enum PrimitiveDataType
{
    Type_NotPrimitive = -1,
    Type_Bool8 = 0,
    Type_Int8,
    Type_UInt8,
    Type_Char,
    Type_Bool16,
    Type_Int16,
    Type_UInt16,
    Type_Bool32,
    Type_Int32,
    Type_UInt32,
    Type_Bool64,
    Type_Int64,
    Type_UInt64,
    Type_Float,
    Type_Double,
    Type_Bitfield
};

const QLatin1String primitiveTypeNames[] = {
    QLatin1String("bool8"),
    QLatin1String("int8"),
    QLatin1String("uint8"),
    QLatin1String("char"),
    QLatin1String("bool16"),
    QLatin1String("int16"),
    QLatin1String("uint16"),
    QLatin1String("bool32"),
    QLatin1String("int32"),
    QLatin1String("uint32"),
    QLatin1String("bool64"),
    QLatin1String("int64"),
    QLatin1String("uint64"),
    QLatin1String("float"),
    QLatin1String("double"),
    QLatin1String("bitfield"),
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
        ulongValue(0)
    {
        intValue = val;
    }
    inline AllPrimitiveTypes(quint32 val) :
        ulongValue(0)
    {
        uintValue = val;

    }
    inline AllPrimitiveTypes(qint16 val) :
        ulongValue(0)
    {
        shortValue = val;

    }
    inline AllPrimitiveTypes(quint16 val) :
        ulongValue(0)
    {
        ushortValue = val;

    }
    inline AllPrimitiveTypes(qint8 val) :
        ulongValue(0)
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
    inline AllPrimitiveTypes(QByteArray& array) :
        ulongValue(0)
    {
        for (uint i = 0; i < sizeof(AllPrimitiveTypes); i++)
        {
            allBytes[i] = array.at(i);
        }
    }
    inline bool operator!=(AllPrimitiveTypes other) const
    {
        return ulongValue != other.ulongValue;
    }
    inline bool operator==(AllPrimitiveTypes other) const
    {
        return ulongValue == other.ulongValue;
    }
    inline QByteArray toQByteArray() const
    {
        return QByteArray((const char*) allBytes, 8);
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
            const Okteta::Address address, const quint64 bitsRemaining,
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
            const Okteta::Address address, const quint64 bitsRemaining,
            quint8* const bitOffset);

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
#endif /* ALLPRIMITIVETYPES_H_ */
