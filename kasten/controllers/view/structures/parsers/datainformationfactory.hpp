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
    QScriptValue updateFunc;
    QScriptValue validationFunc;
    QScriptValue toStringFunc;
    QString customTypeName;
    DataInformation::DataInformationEndianess endianess;

private:
    Q_DISABLE_COPY(CommonParsedData)
};

struct BitfieldParsedData : public ParserInfo
{
    inline explicit BitfieldParsedData(const ParserInfo& i) : ParserInfo(i) {}
    QString type;
    ParsedNumber<int> width;

private:
    Q_DISABLE_COPY(BitfieldParsedData)
};

struct PrimitiveParsedData : public ParserInfo
{
    inline explicit PrimitiveParsedData(const ParserInfo& i) : ParserInfo(i) {}
    QString type;

private:
    Q_DISABLE_COPY(PrimitiveParsedData)
};

struct EnumParsedData : public ParserInfo
{
    inline explicit EnumParsedData(const ParserInfo& i) : ParserInfo(i) {}
    QString type;
    QString enumName;
    EnumDefinition::Ptr enumDef; // TODO QMap<QString, QScriptValue> instead
    /** only used if enumDef is null, to allow sharing (only possible in OSD) */
    QScriptValue enumValuesObject;

private:
    Q_DISABLE_COPY(EnumParsedData)
};

struct StringParsedData : public ParserInfo
{
    inline explicit StringParsedData(const ParserInfo& i) : ParserInfo(i) {}
    QString encoding;
    ParsedNumber<quint32> termination;
    ParsedNumber<quint32> maxCharCount;
    ParsedNumber<quint32> maxByteCount;

private:
    Q_DISABLE_COPY(StringParsedData)
};

struct ArrayParsedData : public ParserInfo
{
    inline explicit ArrayParsedData(const ParserInfo& i)
        : ParserInfo(i)
        , arrayType(nullptr)
    {}
    QScriptValue length;
    DataInformation* arrayType;

private:
    Q_DISABLE_COPY(ArrayParsedData)
};

struct PointerParsedData : public ParserInfo
{
    inline explicit PointerParsedData(const ParserInfo& i)
        : ParserInfo(i)
        , valueType(nullptr)
        , pointerTarget(nullptr)
        , pointerScale(1)
    {}
    DataInformation* valueType;
    DataInformation* pointerTarget;
    qint64           pointerScale;
    QScriptValue     interpretFunc;

private:
    Q_DISABLE_COPY(PointerParsedData)
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
    QScopedPointer<ChildrenParser> children;
    QVector<Alternatives> alternatives;
    QScopedPointer<ChildrenParser> defaultFields;

private:
    Q_DISABLE_COPY(TaggedUnionParsedData)
};

struct StructOrUnionParsedData : public ParserInfo
{
    inline explicit StructOrUnionParsedData(const ParserInfo& i) : ParserInfo(i) {}
    QScopedPointer<ChildrenParser> children;

private:
    Q_DISABLE_COPY(StructOrUnionParsedData)
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
