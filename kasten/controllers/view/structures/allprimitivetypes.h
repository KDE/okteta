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
#ifndef ALLPRIMITIVETYPES_H_
#define ALLPRIMITIVETYPES_H_

#include <QtGlobal>
#include <QByteArray>

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
    Type_Int32,
    Type_UInt32,
    Type_Bool32,
    Type_Int64,
    Type_UInt64,
    Type_Bool64,
    Type_Float,
    Type_Double
};
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
    bool operator!=(AllPrimitiveTypes other)
    {
        return ulongValue != other.ulongValue;
    }
    inline QByteArray toQByteArray() const
    {
        return QByteArray((const char*) allBytes, 8);
    }
    inline bool operator<(AllPrimitiveTypes other) const
    {
        return this->ulongValue < other.ulongValue;
    }
};
#endif /* ALLPRIMITIVETYPES_H_ */
