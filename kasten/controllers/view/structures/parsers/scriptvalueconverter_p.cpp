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

#include "scriptvalueconverter.h"
#include "scriptvalueconverter_p.h"

#include "datainformationfactory.h"
#include "parserutils.h"
#include "../datatypes/uniondatainformation.h"
#include "../datatypes/structuredatainformation.h"
#include "../datatypes/strings/stringdata.h"
#include "../datatypes/strings/stringdatainformation.h"
#include "../script/scriptlogger.h"

using namespace ParserStrings;

namespace ScriptValueConverter
{

DataInformation* toDataInformation(const QScriptValue& value, const ParserInfo& oldInfo)
{
    if (!value.isValid())
    {
        oldInfo.error() << "invalid value passed!";
        return 0;
    }
    ParserInfo info(oldInfo);
    QString nameOverride = value.property(PROPERTY_NAME).toString();
    if (!nameOverride.isEmpty())
        info.name = nameOverride;

    //check function array and date, since they are objects too
    if (value.isRegExp())
    {
        //apparently regexp is a function
        info.error() << "Cannot convert a RegExp object to DataInformation!";
        return 0;
    }
    if (value.isFunction())
    {
        info.error() << "Cannot convert a Function object to DataInformation!";
        return 0;
    }
    if (value.isArray())
    {
        info.error() << "Cannot convert a Array object to DataInformation!";
        return 0;
    }
    if (value.isDate())
    {
        info.error() << "Cannot convert a Date object to DataInformation!";
        return 0;
    }
    if (value.isError())
    {
        info.error() << "Cannot convert a Error object to DataInformation!";
        return 0;
    }
    //variant and qobject are also object types, however they cannot appear from user code, no need to check

    //if it is a string, we convert to primitive type, if not it has to be an object
    if (value.isString())
        return toPrimitive(value, info); //a type string is also okay

    if (!value.isObject())
    {
        if (value.isBool())
            info.error() << "Cannot convert Boolean to DataInformation!";
        else if (value.isNull())
            info.error() << "Cannot convert null to DataInformation!";
        else if (value.isUndefined())
            info.error() << "Cannot convert undefined to DataInformation!";
        else if (value.isNumber())
            info.error() << "Cannot convert Number to DataInformation!";
        else
            info.error() << "Cannot convert object of unknown type to DataInformation!";

        return 0; //no point trying to convert
    }

    QString type = value.property(PROPERTY_INTERNAL_TYPE).toString();
    if (type.isEmpty())
    {
        info.error() << "Cannot convert object since type of object could not be determined!";
        return 0;
    }
    DataInformation* returnVal = 0;

    if (type == TYPE_ARRAY)
        returnVal = toArray(value, info);

    else if (type == TYPE_STRUCT)
        returnVal = toStruct(value, info);

    else if (type == TYPE_UNION)
        returnVal = toUnion(value, info);

    else if (type == TYPE_BITFIELD)
        returnVal = toBitfield(value, info);

    else if (type == TYPE_ENUM)
        returnVal = toEnum(value, false, info);

    else if (type == TYPE_FLAGS)
        returnVal = toEnum(value, true, info);

    else if (type == TYPE_STRING)
        returnVal = toString(value, info);

    else if (type == TYPE_POINTER)
        returnVal = toPointer(value, info);

    else if (type == TYPE_TAGGED_UNION)
        returnVal = toTaggedUnion(value, info);

    else if (type == TYPE_PRIMITIVE)
        returnVal = toPrimitive(value, info);

    else
        info.error() << "Unkown type:" << type;

    if (returnVal)
    {
        CommonParsedData cpd(info);
        QString byteOrderStr = value.property(PROPERTY_BYTEORDER).toString();
        if (!byteOrderStr.isEmpty())
            cpd.endianess = ParserUtils::byteOrderFromString(byteOrderStr,
                    LoggerWithContext(info.logger, info.context()));
        cpd.updateFunc = value.property(PROPERTY_UPDATE_FUNC);
        cpd.validationFunc = value.property(PROPERTY_VALIDATION_FUNC);
        if (!DataInformationFactory::commonInitialization(returnVal, cpd))
        {
            delete returnVal; //error message has already been logged
            return 0;
        }
    }
    return returnVal;
}

ArrayDataInformation* toArray(const QScriptValue& value, const ParserInfo& info)
{
    ArrayParsedData apd(info);
    QScriptValue lengthProp = value.property(PROPERTY_LENGTH);
    if (!lengthProp.isValid())
    {
        info.error() << "No array length specified!";
        return 0;
    }

    if (lengthProp.isFunction())
    {
        apd.lengthFunction = lengthProp;
    }
    else
    {
        apd.length = ParserUtils::intFromScriptValue(lengthProp);
    }

    QScriptValue childType = value.property(PROPERTY_TYPE);
    ParserInfo childInfo(info);
    DummyDataInformation dummy(info.parent, info.name);
    childInfo.parent = &dummy;
    apd.arrayType = toDataInformation(childType, childInfo);

    return DataInformationFactory::newArray(apd);
}

AbstractBitfieldDataInformation* toBitfield(const QScriptValue& value, const ParserInfo& info)
{
    BitfieldParsedData bpd(info);
    bpd.type = value.property(PROPERTY_TYPE).toString();
    bpd.width = ParserUtils::intFromScriptValue(value.property(PROPERTY_WIDTH));
    return DataInformationFactory::newBitfield(bpd);
}

PrimitiveDataInformation* toPrimitive(const QScriptValue& value, const ParserInfo& info)
{
    PrimitiveParsedData ppd(info);
    ppd.type = value.isString() ? value.toString() : value.property(PROPERTY_TYPE).toString();
    return DataInformationFactory::newPrimitive(ppd);
}

StructureDataInformation* toStruct(const QScriptValue& value, const ParserInfo& info)
{
    StructOrUnionParsedData supd(info);
    supd.children.reset(new ScriptValueChildrenParser(info, value.property(PROPERTY_CHILDREN)));
    return DataInformationFactory::newStruct(supd);
}

UnionDataInformation* toUnion(const QScriptValue& value, const ParserInfo& info)
{
    StructOrUnionParsedData supd(info);
    supd.children.reset(new ScriptValueChildrenParser(info, value.property(PROPERTY_CHILDREN)));
    return DataInformationFactory::newUnion(supd);}

PointerDataInformation* toPointer(const QScriptValue& value, const ParserInfo& info)
{
    PointerParsedData ppd(info);

    ParserInfo childInfo(info);
    DummyDataInformation dummy(info.parent, info.name);
    childInfo.parent = &dummy;
    childInfo.name = NAME_POINTER_TARGET;
    ppd.pointerTarget = toDataInformation(value.property(PROPERTY_TARGET), childInfo);
    childInfo.name = NAME_POINTER_VALUE_TYPE;
    ppd.valueType = toDataInformation(value.property(PROPERTY_TYPE), childInfo);

    return DataInformationFactory::newPointer(ppd);
}

EnumDataInformation* toEnum(const QScriptValue& value, bool flags, const ParserInfo& info)
{
    EnumParsedData epd(info);
    QScriptValue enumType = value.property(PROPERTY_TYPE);
    if (enumType.isString())
        epd.type = enumType.toString();
    else if (enumType.isObject())
        epd.type = enumType.property(PROPERTY_TYPE).toString();
    //else it stays empty
    epd.enumName = value.property(PROPERTY_ENUM_NAME).toString();
    epd.enumValuesObject = value.property(PROPERTY_ENUM_VALUES);

    if (flags)
        return DataInformationFactory::newFlags(epd);
    else
        return DataInformationFactory::newEnum(epd);

}

StringDataInformation* toString(const QScriptValue& value, const ParserInfo& info)
{
    StringParsedData spd(info);
    spd.encoding = value.property(PROPERTY_ENCODING).toString();
    spd.termination = ParserUtils::uintFromScriptValue(value.property(PROPERTY_TERMINATED_BY));
    spd.maxByteCount = ParserUtils::uintFromScriptValue(value.property(PROPERTY_MAX_BYTE_COUNT));
    spd.maxCharCount = ParserUtils::uintFromScriptValue(value.property(PROPERTY_MAX_CHAR_COUNT));
    return DataInformationFactory::newString(spd);
}

TaggedUnionDataInformation* toTaggedUnion(const QScriptValue& value, const ParserInfo& info)
{
    TaggedUnionParsedData tpd(info);
    QScriptValue alternatives = value.property(PROPERTY_ALTERNATIVES);
    if (!alternatives.isArray())
    {
        info.error() << "Alternatives must be an array!";
        return 0;
    }
    int length = alternatives.property(PROPERTY_LENGTH).toInt32();
    for (int i = 0; i < length; ++i)
    {
        TaggedUnionParsedData::Alternatives alt;
        QScriptValue current = alternatives.property(i);
        alt.name = current.property(PROPERTY_STRUCT_NAME).toString();
        alt.selectIf = current.property(PROPERTY_SELECT_IF);
        alt.fields = QSharedPointer<ChildrenParser>(
                new ScriptValueChildrenParser(info, current.property(PROPERTY_CHILDREN)));
        tpd.alternatives.append(alt);
    }
    tpd.children.reset(new ScriptValueChildrenParser(info, value.property(PROPERTY_CHILDREN)));
    tpd.defaultFields.reset(new ScriptValueChildrenParser(info, value.property(PROPERTY_DEFAULT_CHILDREN)));
    return DataInformationFactory::newTaggedUnion(tpd);
}

} //namespace ScriptValueConverter

ScriptValueConverter::ScriptValueChildrenParser::ScriptValueChildrenParser(const ParserInfo& info,
        const QScriptValue& children) : mValue(children), mIter(children), mInfo(info)
{
}

ScriptValueConverter::ScriptValueChildrenParser::~ScriptValueChildrenParser()
{
}

DataInformation* ScriptValueConverter::ScriptValueChildrenParser::next()
{
    Q_ASSERT(hasNext());
    mIter.next();
    if (mValue.isArray() && mIter.name() == QLatin1String("length"))
        mIter.next(); //skip length property
    mInfo.name = mIter.name();
    return toDataInformation(mIter.value(), mInfo);
}

bool ScriptValueConverter::ScriptValueChildrenParser::hasNext()
{
    if (!mIter.hasNext())
        return false;
    if (mValue.isArray())
    {
        //check if next element is length property
        mIter.next();
        if (mIter.name() != QLatin1String("length"))
        {
            mIter.previous(); //go back and return true
            return true;
        }
        else
        {
            return mIter.hasNext(); //skipped length
        }
    }
    else
    {
        return true;
    }
}

void ScriptValueConverter::ScriptValueChildrenParser::setParent(DataInformation* newParent)
{
    mInfo.parent = newParent;
}
