/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DATAINFORMATIONFACTORY_HPP
#define KASTEN_DATAINFORMATIONFACTORY_HPP

// lib
#include "parserutils.hpp"
#include <bitfield/abstractbitfielddatainformation.hpp>
#include <primitivedatainformation.hpp>
#include <enumdefinition.hpp>
#include <flagdatainformation.hpp>
#include <enumdatainformation.hpp>
#include <pointerdatainformation.hpp>
#include <arraydatainformation.hpp>
#include <stringdatainformation.hpp>
#include <taggeduniondatainformation.hpp>
#include <uniondatainformation.hpp>
#include <structuredatainformation.hpp>
#include <scriptlogger.hpp>
// Qt
#include <QScriptValue>
#include <QSharedPointer>
// Std
#include <memory>

struct CommonParsedData : public ParserInfo
{
    inline explicit CommonParsedData(const ParserInfo& i)
        : ParserInfo(i)
    {}
    CommonParsedData(const CommonParsedData&) = delete;

    ~CommonParsedData() = default;

    CommonParsedData& operator=(const CommonParsedData&) = delete;

    QScriptValue updateFunc;
    QScriptValue validationFunc;
    QScriptValue toStringFunc;
    QString customTypeName;
    DataInformation::DataInformationEndianness endianness = DataInformation::DataInformationEndianness::EndiannessInherit;
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
    {}
    ArrayParsedData(const ArrayParsedData&) = delete;

    ~ArrayParsedData() = default;

    ArrayParsedData& operator=(const ArrayParsedData&) = delete;

    QScriptValue length;
    std::unique_ptr<DataInformation> arrayType;
};

struct PointerParsedData : public ParserInfo
{
    inline explicit PointerParsedData(const ParserInfo& i)
        : ParserInfo(i)
    {}
    PointerParsedData(const PointerParsedData&) = delete;

    ~PointerParsedData() = default;

    PointerParsedData& operator=(const PointerParsedData&) = delete;

    std::unique_ptr<DataInformation> valueType;
    std::unique_ptr<DataInformation> pointerTarget;
    qint64           pointerScale = 1;
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

    std::unique_ptr<ChildrenParser> children;
    std::vector<Alternatives> alternatives;
    std::unique_ptr<ChildrenParser> defaultFields;
};

struct StructOrUnionParsedData : public ParserInfo
{
    inline explicit StructOrUnionParsedData(const ParserInfo& i) : ParserInfo(i) {}
    StructOrUnionParsedData(const StructOrUnionParsedData&) = delete;

    ~StructOrUnionParsedData() = default;

    StructOrUnionParsedData& operator=(const StructOrUnionParsedData&) = delete;

    std::unique_ptr<ChildrenParser> children;
};

namespace DataInformationFactory {
std::unique_ptr<AbstractBitfieldDataInformation> newBitfield(const BitfieldParsedData& pd);
std::unique_ptr<PrimitiveDataInformation> newPrimitive(const PrimitiveParsedData& pd);
std::unique_ptr<EnumDataInformation> newEnum(const EnumParsedData& pd);
std::unique_ptr<FlagDataInformation> newFlags(const EnumParsedData& pd);
std::unique_ptr<ArrayDataInformation> newArray(ArrayParsedData& pd);
std::unique_ptr<StringDataInformation> newString(const StringParsedData& pd);
std::unique_ptr<PointerDataInformation> newPointer(PointerParsedData& pd);
std::unique_ptr<UnionDataInformation> newUnion(const StructOrUnionParsedData& pd);
std::unique_ptr<StructureDataInformation> newStruct(const StructOrUnionParsedData& pd);
std::unique_ptr<TaggedUnionDataInformation> newTaggedUnion(const TaggedUnionParsedData& pd);

bool commonInitialization(DataInformation* data, const CommonParsedData& pd);
}

#endif /* KASTEN_DATAINFORMATIONFACTORY_HPP */
