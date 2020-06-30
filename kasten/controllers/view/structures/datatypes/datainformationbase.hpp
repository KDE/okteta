/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DATAINFORMATIONBASE_HPP
#define KASTEN_DATAINFORMATIONBASE_HPP

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
class DataInformationWithChildren;
class DataInformationWithDummyChildren;

#include <QtGlobal>

using BitCount32 = quint32;
using BitCount64 = quint64;

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
    virtual bool isDataInformationWithChildren() const;
    DataInformationWithChildren* asDataInformationWithChildren();
    const DataInformationWithChildren* asDataInformationWithChildren() const;
    virtual bool isDataInformationWithDummyChildren() const;
    DataInformationWithDummyChildren* asDataInformationWithDummyChildren();
    const DataInformationWithDummyChildren* asDataInformationWithDummyChildren() const;
    virtual bool isDummy() const;
    DummyDataInformation* asDummy();
    const DummyDataInformation* asDummy() const;
};

#define CAST_FUNCS_2(typename, name) inline typename* DataInformationBase::as##name() {\
        return is##name() ? reinterpret_cast<typename*>(this) : nullptr; }\
    inline const typename* DataInformationBase::as##name() const {\
        return is##name() ? reinterpret_cast<const typename*>(this) : nullptr; }

#define CAST_FUNCS(type) CAST_FUNCS_2(type##DataInformation, type)
// we have to use reinterpret_cast<> since it does not seem to be possible to declare inheritance
// and including other headers would be overkill

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
CAST_FUNCS_2(DataInformationWithChildren, DataInformationWithChildren)
CAST_FUNCS_2(DataInformationWithDummyChildren, DataInformationWithDummyChildren)

// this is not handled by the macro
inline DataInformation* DataInformationBase::asDataInformation()
{
    return isTopLevel() ? nullptr : reinterpret_cast<DataInformation*>(this);
}

inline const DataInformation* DataInformationBase::asDataInformation() const
{
    return isTopLevel() ? nullptr : reinterpret_cast<const DataInformation*>(this);
}

#undef CAST_FUNCS_2
#undef CAST_FUNCS

#endif // KASTEN_DATAINFORMATIONBASE_HPP
