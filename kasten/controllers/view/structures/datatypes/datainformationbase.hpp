/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DATAINFORMATIONBASE_HPP
#define KASTEN_DATAINFORMATIONBASE_HPP

// Qt
#include <QtGlobal>

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

using BitCount32 = quint32;
using BitCount64 = quint64;

class DataInformationBase
{
public:
    DataInformationBase();
    DataInformationBase(const DataInformationBase&) = delete;
    DataInformationBase(DataInformationBase&&) = delete;

    virtual ~DataInformationBase();

public:
    DataInformationBase& operator=(const DataInformationBase&) = delete;
    DataInformationBase& operator=(DataInformationBase&&) = delete;

public: // API to implement
    [[nodiscard]]
    virtual bool isTopLevel() const = 0;
    [[nodiscard]]
    virtual bool isArray() const;
    [[nodiscard]]
    virtual bool isPrimitive() const;
    [[nodiscard]]
    virtual bool isEnum() const;
    [[nodiscard]]
    virtual bool isBitfield() const;
    [[nodiscard]]
    virtual bool isStruct() const;
    [[nodiscard]]
    virtual bool isUnion() const;
    [[nodiscard]]
    virtual bool isString() const;
    [[nodiscard]]
    virtual bool isPointer() const;
    [[nodiscard]]
    virtual bool isTaggedUnion() const;
    [[nodiscard]]
    virtual bool isDataInformationWithChildren() const;
    [[nodiscard]]
    virtual bool isDataInformationWithDummyChildren() const;
    [[nodiscard]]
    virtual bool isDummy() const;

public:
    [[nodiscard]]
    TopLevelDataInformation* asTopLevel();
    [[nodiscard]]
    DataInformation* asDataInformation();
    [[nodiscard]]
    const TopLevelDataInformation* asTopLevel() const;
    [[nodiscard]]
    const DataInformation* asDataInformation() const;
    [[nodiscard]]
    ArrayDataInformation* asArray();
    [[nodiscard]]
    const ArrayDataInformation* asArray() const;
    [[nodiscard]]
    PrimitiveDataInformation* asPrimitive();
    [[nodiscard]]
    const PrimitiveDataInformation* asPrimitive() const;
    [[nodiscard]]
    EnumDataInformation* asEnum();
    [[nodiscard]]
    const EnumDataInformation* asEnum() const;
    [[nodiscard]]
    AbstractBitfieldDataInformation* asBitfield();
    [[nodiscard]]
    const AbstractBitfieldDataInformation* asBitfield() const;
    [[nodiscard]]
    StructureDataInformation* asStruct();
    [[nodiscard]]
    const StructureDataInformation* asStruct() const;
    [[nodiscard]]
    UnionDataInformation* asUnion();
    [[nodiscard]]
    const UnionDataInformation* asUnion() const;
    [[nodiscard]]
    StringDataInformation* asString();
    [[nodiscard]]
    const StringDataInformation* asString() const;
    [[nodiscard]]
    PointerDataInformation* asPointer();
    [[nodiscard]]
    const PointerDataInformation* asPointer() const;
    [[nodiscard]]
    TaggedUnionDataInformation* asTaggedUnion();
    [[nodiscard]]
    const TaggedUnionDataInformation* asTaggedUnion() const;
    [[nodiscard]]
    DataInformationWithChildren* asDataInformationWithChildren();
    [[nodiscard]]
    const DataInformationWithChildren* asDataInformationWithChildren() const;
    [[nodiscard]]
    DataInformationWithDummyChildren* asDataInformationWithDummyChildren();
    [[nodiscard]]
    const DataInformationWithDummyChildren* asDataInformationWithDummyChildren() const;
    [[nodiscard]]
    DummyDataInformation* asDummy();
    [[nodiscard]]
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
