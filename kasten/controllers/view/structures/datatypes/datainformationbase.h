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

class EnumDataInformation;
class StringDataInformation;
class DataInformationBase;
class DataInformation;
class DummyDataInformation;
class UnionDataInformation;
class TaggedUnionDataInformation;
class StructureDataInformation;
class AbstractBitfieldDataInformation;
class PrimitiveDataInformation;
class ArrayDataInformation;
class TopLevelDataInformation;
class PointerDataInformation;

#include <QtGlobal>

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
    const TopLevelDataInformation* asTopLevel() const;
    const DataInformation* asDataInformation() const;
    virtual bool isArray() const;
    ArrayDataInformation* asArray();
    const ArrayDataInformation* asArray() const;
    virtual bool isPrimitive() const;
    PrimitiveDataInformation* asPrimitive();
    const PrimitiveDataInformation* asPrimitive() const;
    virtual bool isEnum() const;
    EnumDataInformation* asEnum();
    const EnumDataInformation* asEnum() const;
    virtual bool isBitfield() const;
    AbstractBitfieldDataInformation* asBitfield();
    const AbstractBitfieldDataInformation* asBitfield() const;
    virtual bool isStruct() const;
    StructureDataInformation* asStruct();
    const StructureDataInformation* asStruct() const;
    virtual bool isUnion() const;
    UnionDataInformation* asUnion();
    const UnionDataInformation* asUnion() const;
    virtual bool isString() const;
    StringDataInformation* asString();
    const StringDataInformation* asString() const;
    virtual bool isPointer() const;
    PointerDataInformation* asPointer();
    const PointerDataInformation* asPointer() const;
    virtual bool isTaggedUnion() const;
    TaggedUnionDataInformation* asTaggedUnion();
    const TaggedUnionDataInformation* asTaggedUnion() const;
    virtual bool isDummy() const;
    DummyDataInformation* asDummy();
    const DummyDataInformation* asDummy() const;
};

#define CAST_FUNCS_2(typename, name) inline typename* DataInformationBase::as##name() {\
        return is##name() ? reinterpret_cast<typename*>(this) : 0; }\
    inline const typename* DataInformationBase::as##name() const {\
        return is##name() ? reinterpret_cast<const typename*>(this) : 0; }

#define CAST_FUNCS(type) CAST_FUNCS_2(type##DataInformation, type)
//we have to use reinterpret_cast<> since it does not seem to be possible to declare inheritance
//and including other headers would be overkill

CAST_FUNCS(Array)
CAST_FUNCS(Union)
CAST_FUNCS_2(StructureDataInformation, Struct)
CAST_FUNCS(Enum)
CAST_FUNCS(Primitive)
CAST_FUNCS(Dummy)
CAST_FUNCS_2(AbstractBitfieldDataInformation, Bitfield)
CAST_FUNCS(String)
CAST_FUNCS(Pointer)
CAST_FUNCS(TaggedUnion)

//this is not handled by the macro
inline DataInformation* DataInformationBase::asDataInformation()
{
    return isTopLevel() ? 0 : reinterpret_cast<DataInformation*>(this);
}

inline const DataInformation* DataInformationBase::asDataInformation() const
{
    return isTopLevel() ? 0 : reinterpret_cast<const DataInformation*>(this);
}

#undef CAST_FUNCS_2
#undef CAST_FUNCS

#endif // DATAINFORMATIONBASE_H
