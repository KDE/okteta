/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DATAINFORMATIONFACTORY_HPP
#define KASTEN_DATAINFORMATIONFACTORY_HPP

#include <QScriptValue>
#include <QSharedPointer>

#include "../datatypes/primitive/bitfield/abstractbitfielddatainformation.hpp"
#include "../datatypes/primitive/primitivedatainformation.hpp"
#include "../datatypes/primitive/enumdefinition.hpp"
#include "../datatypes/primitive/flagdatainformation.hpp"
#include "../datatypes/primitive/enumdatainformation.hpp"
#include "../datatypes/primitive/pointerdatainformation.hpp"
#include "../datatypes/array/arraydatainformation.hpp"
#include "../datatypes/strings/stringdatainformation.hpp"
#include "../datatypes/taggeduniondatainformation.hpp"
#include "../datatypes/uniondatainformation.hpp"
#include "../datatypes/structuredatainformation.hpp"

#include "../script/scriptlogger.hpp"
#include "parserutils.hpp"

struct CommonParsedData : public ParserInfo
{
    inline explicit CommonParsedData(const ParserInfo& i)
        : ParserInfo(i)
        , endianess(DataInformation::DataInformationEndianess::EndianessInherit)
    {}
    CommonParsedData(const CommonParsedData&) = delete;

    ~CommonParsedData() = default;

    CommonParsedData& operator=(const CommonParsedData&) = delete;

    QScriptValue updateFunc;
    QScriptValue validationFunc;
    QScriptValue toStringFunc;
    QString customTypeName;
    DataInformation::DataInformationEndianess endianess;
};

struct BitfieldParsedData : public ParserInfo
{
    inline explicit BitfieldParsedData(const ParserInfo& i) : ParserInfo(i) {}
    BitfieldParsedData(const BitfieldParsedData&) = delete;

    ~BitfieldParsedData() = default;

    BitfieldParsedData& operator=(const BitfieldParsedData&) = delete;

    QString type;
    ParsedNumber<int> width;
};

struct PrimitiveParsedData : public ParserInfo
{
    inline explicit PrimitiveParsedData(const ParserInfo& i) : ParserInfo(i) {}
    PrimitiveParsedData(const PrimitiveParsedData&) = delete;

    ~PrimitiveParsedData() = default;

    PrimitiveParsedData& operator=(const PrimitiveParsedData&) = delete;

    QString type;
};

struct EnumParsedData : public ParserInfo
{
    inline explicit EnumParsedData(const ParserInfo& i) : ParserInfo(i) {}
    EnumParsedData(const EnumParsedData&) = delete;

    ~EnumParsedData() = default;

    EnumParsedData& operator=(const EnumParsedData&) = delete;

    QString type;
    QString enumName;
    EnumDefinition::Ptr enumDef; // TODO QMap<QString, QScriptValue> instead
    /** only used if enumDef is null, to allow sharing (only possible in OSD) */
    QScriptValue enumValuesObject;
};

struct StringParsedData : public ParserInfo
{
    inline explicit StringParsedData(const ParserInfo& i) : ParserInfo(i) {}
    StringParsedData(const StringParsedData&) = delete;

    ~StringParsedData() = default;

    StringParsedData& operator=(const StringParsedData&) = delete;

    QString encoding;
    ParsedNumber<quint32> termination;
    ParsedNumber<quint32> maxCharCount;
    ParsedNumber<quint32> maxByteCount;
};

struct ArrayParsedData : public ParserInfo
{
    inline explicit ArrayParsedData(const ParserInfo& i)
        : ParserInfo(i)
        , arrayType(nullptr)
    {}
    ArrayParsedData(const ArrayParsedData&) = delete;

    ~ArrayParsedData() = default;

    ArrayParsedData& operator=(const ArrayParsedData&) = delete;

    QScriptValue length;
    DataInformation* arrayType;
};

struct PointerParsedData : public ParserInfo
{
    inline explicit PointerParsedData(const ParserInfo& i)
        : ParserInfo(i)
        , valueType(nullptr)
        , pointerTarget(nullptr)
        , pointerScale(1)
    {}
    PointerParsedData(const PointerParsedData&) = delete;

    ~PointerParsedData() = default;

    PointerParsedData& operator=(const PointerParsedData&) = delete;

    DataInformation* valueType;
    DataInformation* pointerTarget;
    qint64           pointerScale;
    QScriptValue     interpretFunc;
};

struct TaggedUnionParsedData : public ParserInfo
{
    struct Alternatives
    {
        QString name;
        QScriptValue selectIf;
        QSharedPointer<ChildrenParser> fields;
    };
    inline explicit TaggedUnionParsedData(const ParserInfo& i) : ParserInfo(i) {}
    TaggedUnionParsedData(const TaggedUnionParsedData&) = delete;

    ~TaggedUnionParsedData() = default;

    TaggedUnionParsedData& operator=(const TaggedUnionParsedData&) = delete;

    QScopedPointer<ChildrenParser> children;
    QVector<Alternatives> alternatives;
    QScopedPointer<ChildrenParser> defaultFields;
};

struct StructOrUnionParsedData : public ParserInfo
{
    inline explicit StructOrUnionParsedData(const ParserInfo& i) : ParserInfo(i) {}
    StructOrUnionParsedData(const StructOrUnionParsedData&) = delete;

    ~StructOrUnionParsedData() = default;

    StructOrUnionParsedData& operator=(const StructOrUnionParsedData&) = delete;

    QScopedPointer<ChildrenParser> children;
};

namespace DataInformationFactory {
AbstractBitfieldDataInformation* newBitfield(const BitfieldParsedData& pd);
PrimitiveDataInformation* newPrimitive(const PrimitiveParsedData& pd);
EnumDataInformation* newEnum(const EnumParsedData& pd);
FlagDataInformation* newFlags(const EnumParsedData& pd);
ArrayDataInformation* newArray(const ArrayParsedData& pd);
StringDataInformation* newString(const StringParsedData& pd);
PointerDataInformation* newPointer(const PointerParsedData& pd);
UnionDataInformation* newUnion(const StructOrUnionParsedData& pd);
StructureDataInformation* newStruct(const StructOrUnionParsedData& pd);
TaggedUnionDataInformation* newTaggedUnion(const TaggedUnionParsedData& pd);

bool commonInitialization(DataInformation* data, const CommonParsedData& pd);
}

#endif /* KASTEN_DATAINFORMATIONFACTORY_HPP */
