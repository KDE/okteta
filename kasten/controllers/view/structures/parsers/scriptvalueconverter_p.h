/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef SCRIPTVALUECONVERTER_P_H_
#define SCRIPTVALUECONVERTER_P_H_

#include "../parsers/parserutils.h"

#include <QScriptValue>
#include <QScriptValueIterator>

namespace ScriptValueConverter
{
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

class ScriptValueChildrenParser : public ChildrenParser {
public:
    ScriptValueChildrenParser(const ParserInfo& info, const QScriptValue& children);
    virtual ~ScriptValueChildrenParser();
    virtual DataInformation* next();
    virtual bool hasNext();
    virtual void setParent(DataInformation* newParent);
private:
    QScriptValue mValue;
    QScriptValueIterator mIter;
    ParserInfo mInfo;
};

}

#endif //SCRIPTVALUECONVERTER_P_H_

