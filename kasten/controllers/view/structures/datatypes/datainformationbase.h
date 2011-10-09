/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef DATAINFORMATIONBASE_H
#define DATAINFORMATIONBASE_H

#include "structviewpreferences.h"

class DataInformationBase;
class DataInformation;
class DummyDataInformation;
class UnionDataInformation;
class StructureDataInformation;
class AbstractBitfieldDataInformation;
class EnumDataInformation;
class PrimitiveDataInformation;
class ArrayDataInformation;
class TopLevelDataInformation;

typedef Kasten::StructViewPreferences::EnumByteOrder::type ByteOrder;
typedef Kasten::StructViewPreferences::EnumByteOrder ByteOrderEnumClass;
/** To indicate that it is a number of bits not bytes */
typedef quint32 BitCount32;
typedef quint64 BitCount64;

class DataInformationBase
{
public:
    DataInformationBase();
    virtual ~DataInformationBase();
    virtual bool isTopLevel() const = 0;
    TopLevelDataInformation* asTopLevel();
    DataInformation* asDataInformation();
    virtual bool isArray() const;
    ArrayDataInformation* asArray();
    virtual bool isPrimitive() const;
    PrimitiveDataInformation* asPrimitive();
    virtual bool isEnum() const;
    EnumDataInformation* asEnum();
    virtual bool isBitfield() const;
    AbstractBitfieldDataInformation* asBitfield();
    virtual bool isStruct() const;
    StructureDataInformation* asStruct();
    virtual bool isUnion() const;
    UnionDataInformation* asUnion();
    virtual bool isDummy() const;
    DummyDataInformation* asDummy();
};

//TODO we have to use reinterpret_cast<> since I don't know how to forward declare inheritance
//and including other headers would be overkill

inline ArrayDataInformation* DataInformationBase::asArray()
{
    Q_ASSERT(isArray());
    return reinterpret_cast<ArrayDataInformation*>(this);
}

inline AbstractBitfieldDataInformation* DataInformationBase::asBitfield()
{
    Q_ASSERT(isBitfield());
    return reinterpret_cast<AbstractBitfieldDataInformation*>(this);
}

inline DummyDataInformation* DataInformationBase::asDummy()
{
    Q_ASSERT(isDummy());
    return reinterpret_cast<DummyDataInformation*>(this);
}

inline EnumDataInformation* DataInformationBase::asEnum()
{
    Q_ASSERT(isEnum());
    return reinterpret_cast<EnumDataInformation*>(this);
}

inline PrimitiveDataInformation* DataInformationBase::asPrimitive()
{
    Q_ASSERT(isPrimitive());
    return reinterpret_cast<PrimitiveDataInformation*>(this);
}

inline StructureDataInformation* DataInformationBase::asStruct()
{
    Q_ASSERT(isStruct());
    return reinterpret_cast<StructureDataInformation*>(this);
}

inline TopLevelDataInformation* DataInformationBase::asTopLevel()
{
    Q_ASSERT(isTopLevel());
    return reinterpret_cast<TopLevelDataInformation*>(this);
}

inline DataInformation* DataInformationBase::asDataInformation()
{
    Q_ASSERT(!isTopLevel());
    return reinterpret_cast<DataInformation*>(this);
}

inline UnionDataInformation* DataInformationBase::asUnion()
{
    Q_ASSERT(isUnion());
    return reinterpret_cast<UnionDataInformation*>(this);
}

#endif // DATAINFORMATIONBASE_H
