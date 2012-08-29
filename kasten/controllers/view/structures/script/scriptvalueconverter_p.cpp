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

#include "../datatypes/array/arraydatainformation.h"
#include "../datatypes/uniondatainformation.h"
#include "../datatypes/structuredatainformation.h"
#include "../datatypes/primitive/enumdatainformation.h"
#include "../datatypes/primitive/flagdatainformation.h"
#include "../datatypes/primitive/enumdefinition.h"
#include "../datatypes/strings/stringdata.h"
#include "../datatypes/strings/stringdatainformation.h"
#include "../datatypes/primitive/bitfield/boolbitfielddatainformation.h"
#include "../datatypes/primitive/bitfield/unsignedbitfielddatainformation.h"
#include "../datatypes/primitive/bitfield/signedbitfielddatainformation.h"
#include "../datatypes/primitivefactory.h"
#include "../datatypes/additionaldata.h"

#include "scriptlogger.h"

#include <QScriptValueIterator>

namespace ScriptValueConverter
{

DataInformation* toDataInformation(const QScriptValue& value, const ParserInfo& info)
{
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

    DataInformation* returnVal;
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

    QString type = value.property(QLatin1String("type")).toString().toLower(); //to lower just to be safe
    if (type.isEmpty())
    {
        info.error() << "Cannot convert object since type of object could not be determined!";
        return 0;
    }
    if (type == QLatin1String("array"))
        returnVal = toArray(value, info);

    else if (type == QLatin1String("struct"))
        returnVal = toStruct(value, info);

    else if (type == QLatin1String("union"))
        returnVal = toUnion(value, info);

    else if (type == QLatin1String("bitfield"))
        returnVal = toBitfield(value, info);

    else if (type == QLatin1String("enum"))
        returnVal = toEnum(value, false, info);

    else if (type == QLatin1String("flags"))
        returnVal = toEnum(value, true, info);

    else if (type == QLatin1String("string"))
        returnVal = toString(value, info);

    //now it can only be a primitive type or something invalid
    else
        returnVal = toPrimitive(value, info);

    if (returnVal)
    {
        //successfully parsed -> add the validate and update functions to the additional data
        QScriptValue updateFunc = value.property(QLatin1String("updateFunc"));
        QScriptValue validationFunc = value.property(QLatin1String("validationFunc"));
        QScriptValue byteOrder = value.property(QLatin1String("byteOrder"));
        if (byteOrder.isValid())
            returnVal->setByteOrder(AbstractStructureParser::byteOrderFromString(byteOrder.toString(), info.logger));

        if (updateFunc.isFunction())
            returnVal->setUpdateFunc(updateFunc);

        if (validationFunc.isFunction())
            returnVal->setValidationFunc(updateFunc);
    }
    return returnVal;
}

ArrayDataInformation* toArray(const QScriptValue& value, const ParserInfo& info)
{
    //we can safely assume that type == "array"
    int length = value.property(QLatin1String("length")).toInt32();
    if (length < 0)
    {
        info.error() << "array length is less than 0:" << length;
        return 0;
    }
    QScriptValue childType = value.property(QLatin1String("childType"));
    DataInformation* arrayType = toDataInformation(childType, info);
    if (!arrayType)
    {
        info.error() << "Failed to convert array type to valid object!";
        return 0;
    }
    return new ArrayDataInformation(info.name, length, arrayType);
}

AbstractBitfieldDataInformation* toBitfield(const QScriptValue& value, const ParserInfo& info)
{
    //we can safely assume that type == "bitfield"
    int width = value.property(QLatin1String("width")).toInt32();
    if (width < 0 || width > 64)
    {
        info.error() <<  "bitfield width is outside of range 1-64:" << width;
        return 0;
    }
    QString bitfieldType = value.property(QLatin1String("bitfieldType")).toString();
    if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(info.name, width);

    else if (bitfieldType.toLower() == QLatin1String("signed"))
        return new SignedBitfieldDataInformation(info.name, width);

    else if (bitfieldType.toLower() == QLatin1String("unsigned"))
        return new UnsignedBitfieldDataInformation(info.name, width);

    info.error().nospace() << "invalid bitfield type specified: '" << bitfieldType
            << "'. Only 'bool', 'signed' or 'unsigned' are valid.";
    return 0;
}

PrimitiveDataInformation* toPrimitive(const QScriptValue& value, const ParserInfo& info)
{
    QString typeString =
            value.isString() ? value.toString() : value.property(QLatin1String("type")).toString();
    if (typeString.isEmpty())
    {
        info.error() << "Object is neither string nor object with type property"
                ", cannot convert it to primitive";
        return 0;
    }
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(typeString);
    if (primitiveType == Type_Invalid || primitiveType == Type_Bitfield)
    {
        info.error() << "unrecognised primitive type: " << typeString;
        return 0;
    }
    return PrimitiveFactory::newInstance(info.name, primitiveType);
}

StructureDataInformation* toStruct(const QScriptValue& value, const ParserInfo& info)
{
    //TODO fix this code so parent is correct!
    QScriptValue valueChildren = value.property(QLatin1String("children"));
    QVector<DataInformation*> fields = convertValues(valueChildren, info.logger);

    if (fields.isEmpty())
        info.info() << "No children were found for struct, this is could be a mistake.";

    StructureDataInformation* structData = new StructureDataInformation(info.name, fields);
    return structData;
}

UnionDataInformation* toUnion(const QScriptValue& value, const ParserInfo& info)
{
    //TODO fix this code so parent is correct!
    QScriptValue valueChildren = value.property(QLatin1String("children"));
    QVector<DataInformation*> fields = convertValues(valueChildren, info.logger);

    if (fields.isEmpty())
        info.info() << "No children were found for union, this is could be a mistake.";


    UnionDataInformation* unionData = new UnionDataInformation(info.name, fields);
    return unionData;
}

AbstractEnumDataInformation* toEnum(const QScriptValue& value, bool flags, const ParserInfo& info)
{
    //TODO also allow bitfields
    QString typeString = value.property(QLatin1String("enumType")).toString();
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(typeString);
    if (primitiveType == Type_Invalid)
    {
        info.error() << "unrecognised primitive type: " << typeString;
        return 0;
    }
    QScriptValue enumValuesObj = value.property(QLatin1String("enumValues"));
    if (!enumValuesObj.isValid() || !enumValuesObj.isObject())
    {
        info.error() << "Could not parse enumerators since variable with key-value"
                "structure was expected";
        return 0;
    }
    QMap<AllPrimitiveTypes, QString> enumValues =
            AbstractEnumDataInformation::parseEnumValues(enumValuesObj, info.logger, primitiveType);

    QString enumName = value.property(QLatin1String("enumName")).toString();
    EnumDefinition::Ptr def(new EnumDefinition(enumValues, enumName, primitiveType));
    PrimitiveDataInformation* primData = PrimitiveFactory::newInstance(info.name, primitiveType);
    if (!primData)
        return NULL;

    if (flags)
        return new FlagDataInformation(info.name, primData, def);
    else
        return new EnumDataInformation(info.name, primData, def);
}

StringDataInformation* toString(const QScriptValue& value, const ParserInfo& info)
{
    //TODO check for bad parameters
    const QScriptValue terminatedBy = value.property(QLatin1String("terminatedBy"));
    const QScriptValue charCount = value.property(QLatin1String("maxCharCount"));
    const QScriptValue byteCount = value.property(QLatin1String("maxByteCount"));
    const QScriptValue encoding = value.property(QLatin1String("encoding"));

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
