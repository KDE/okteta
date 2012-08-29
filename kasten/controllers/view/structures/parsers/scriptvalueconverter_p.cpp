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

#include <QScriptValueIterator>

using namespace ParserStrings;

namespace ScriptValueConverter
{

DataInformation* toDataInformation(const QScriptValue& value, const ParserInfo& oldInfo)
{
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

    //TODO internal type
    QString type = value.property(PROPERTY_INTERNAL_TYPE).toString().toLower(); //to lower just to be safe
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

    else if (type == TYPE_PRIMITIVE)
        returnVal = toPrimitive(value, info);

    if (returnVal)
    {
        CommonParsedData cpd(info);
        QString byteOrderStr = value.property(PROPERTY_BYTEORDER).toString();
        if (!byteOrderStr.isEmpty())
            cpd.endianess = ParserUtils::byteOrderFromString(byteOrderStr, info);
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
        apd.length = ParserUtils::intFromScriptValue(value);
    }

    QScriptValue childType = value.property(PROPERTY_TYPE);
    ParserInfo childInfo(info);
    childInfo.name = info.context() + QLatin1String(".<inner>");
    childInfo.parent = 0;
    apd.arrayType = toDataInformation(childType, childInfo);

    return DataInformationFactory::newArray(apd);
}

AbstractBitfieldDataInformation* toBitfield(const QScriptValue& value, const ParserInfo& info)
{
    BitfieldParsedData bpd(info);
    bpd.type = value.property(PROPERTY_TYPE).toString(); //TODO type
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
    //TODO fix this code so parent is correct!
    QScriptValue valueChildren = value.property(PROPERTY_CHILDREN);
    QVector<DataInformation*> fields = convertValues(valueChildren, info.logger);

    if (fields.isEmpty())
        info.info() << "No children were found for struct, this is could be a mistake.";

    StructureDataInformation* structData = new StructureDataInformation(info.name, fields);
    return structData;
}

UnionDataInformation* toUnion(const QScriptValue& value, const ParserInfo& info)
{
    //TODO fix this code so parent is correct!
    QScriptValue valueChildren = value.property(PROPERTY_CHILDREN);
    QVector<DataInformation*> fields = convertValues(valueChildren, info.logger);

    if (fields.isEmpty())
        info.info() << "No children were found for union, this is could be a mistake.";

    UnionDataInformation* unionData = new UnionDataInformation(info.name, fields);
    return unionData;
}

AbstractEnumDataInformation* toEnum(const QScriptValue& value, bool flags, const ParserInfo& info)
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
    //TODO check for bad parameters
    const QScriptValue terminatedBy = value.property(PROPERTY_TERMINATED_BY);
    const QScriptValue charCount = value.property(PROPERTY_MAX_CHAR_COUNT);
    const QScriptValue byteCount = value.property(PROPERTY_MAX_BYTE_COUNT);
    const QScriptValue encoding = value.property(PROPERTY_ENCODING);

    StringData::TerminationModes mode = StringData::None;
    if (terminatedBy.isNumber())
        mode |= StringData::Sequence;
    if (charCount.isNumber())
        mode |= StringData::CharCount;
    if (byteCount.isNumber())
        mode |= StringData::ByteCount;

    if ((mode & StringData::CharCount) && (mode & StringData::ByteCount))
        mode &= ~StringData::ByteCount; //when both exists charcount wins

    //TODO no more encoding.toString()
    StringDataInformation* data = new StringDataInformation(info.name, encoding.toString());
    //if mode is None, we assume zero terminated strings
    if (mode == StringData::None)
    {
        mode = StringData::Sequence;
        data->setTerminationCodePoint(0);
    }
    else if (mode & StringData::Sequence)
    {
        const uint term = terminatedBy.toUInt32();
        data->setTerminationCodePoint(term);
    }
    if (mode & StringData::CharCount)
    {
        const int count = charCount.toInt32();
        data->setMaxCharCount(qMax(count, 0));
    }
    if (mode & StringData::ByteCount)
    {
        const int count = byteCount.toInt32();
        data->setMaxByteCount(qMax(count, 0));
    }
    return data;
}

} //namespace ScriptValueConverter
