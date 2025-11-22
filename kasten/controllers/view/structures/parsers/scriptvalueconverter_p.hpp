/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTVALUECONVERTER_P_HPP
#define KASTEN_SCRIPTVALUECONVERTER_P_HPP

// lib
#include "parserutils.hpp"
// Qt
#include <QScriptValue>
#include <QScriptValueIterator>
// Std
#include <memory>

namespace ScriptValueConverter {

[[nodiscard]]
std::unique_ptr<DataInformation> toDataInformation(const QScriptValue& value, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<ArrayDataInformation> toArray(const QScriptValue& value, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<AbstractBitfieldDataInformation> toBitfield(const QScriptValue& value, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<PrimitiveDataInformation> toPrimitive(const QScriptValue& value, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<StructureDataInformation> toStruct(const QScriptValue& value, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<UnionDataInformation> toUnion(const QScriptValue& value, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<StringDataInformation> toString(const QScriptValue& value, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<PointerDataInformation> toPointer(const QScriptValue& value, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<EnumDataInformation> toEnum(const QScriptValue& value, bool isFlags, const ParserInfo& info);
[[nodiscard]]
std::unique_ptr<TaggedUnionDataInformation> toTaggedUnion(const QScriptValue& value, const ParserInfo& info);

class ScriptValueChildrenParser : public ChildrenParser
{
public:
    ScriptValueChildrenParser(const ParserInfo& info, const QScriptValue& children);
    ~ScriptValueChildrenParser() override;

public: // ChildrenParser API
    [[nodiscard]]
    std::unique_ptr<DataInformation> next() override;
    [[nodiscard]]
    bool hasNext() override;
    void setParent(DataInformation* newParent) override;

private:
    QScriptValue mValue;
    QScriptValueIterator mIter;
    ParserInfo mInfo;
};

}

#endif // KASTEN_SCRIPTVALUECONVERTER_P_HPP
