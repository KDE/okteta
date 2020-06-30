/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTVALUECONVERTER_P_HPP
#define KASTEN_SCRIPTVALUECONVERTER_P_HPP

#include "../parsers/parserutils.hpp"

#include <QScriptValue>
#include <QScriptValueIterator>

namespace ScriptValueConverter {

DataInformation* toDataInformation(const QScriptValue& value, const ParserInfo& info);
ArrayDataInformation* toArray(const QScriptValue& value, const ParserInfo& info);
AbstractBitfieldDataInformation* toBitfield(const QScriptValue& value, const ParserInfo& info);
PrimitiveDataInformation* toPrimitive(const QScriptValue& value, const ParserInfo& info);
StructureDataInformation* toStruct(const QScriptValue& value, const ParserInfo& info);
UnionDataInformation* toUnion(const QScriptValue& value, const ParserInfo& info);
StringDataInformation* toString(const QScriptValue& value, const ParserInfo& info);
PointerDataInformation* toPointer(const QScriptValue& value, const ParserInfo& info);
EnumDataInformation* toEnum(const QScriptValue& value, bool isFlags, const ParserInfo& info);
TaggedUnionDataInformation* toTaggedUnion(const QScriptValue& value, const ParserInfo& info);

class ScriptValueChildrenParser : public ChildrenParser
{
public:
    ScriptValueChildrenParser(const ParserInfo& info, const QScriptValue& children);
    ~ScriptValueChildrenParser() override;
    DataInformation* next() override;
    bool hasNext() override;
    void setParent(DataInformation* newParent) override;

private:
    QScriptValue mValue;
    QScriptValueIterator mIter;
    ParserInfo mInfo;
};

}

#endif // KASTEN_SCRIPTVALUECONVERTER_P_HPP
