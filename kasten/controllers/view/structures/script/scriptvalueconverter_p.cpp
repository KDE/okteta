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

#include "../parsers/abstractstructureparser.h"

#include "scriptutils.h"
#include "scriptlogger.h"

#include <QScriptValueIterator>

namespace ScriptValueConverter
{

DataInformation* toDataInformation(const QScriptValue& value, const QString& passedName, ScriptLogger* logger)
{
    const QString name = passedName.isEmpty() ? i18n("<no name specified>") : passedName;
    DataInformation* returnVal;
    if (value.isString())
        return toPrimitive(value, name, logger); //a type string is also okay

    if (!value.isObject())
        return 0; //no point trying to convert

    QString type = value.property(QLatin1String("type")).toString().toLower(); //to lower just to be safe

    if (type == QLatin1String("array"))
        returnVal = toArray(value, name, logger);

    else if (type == QLatin1String("struct"))
        returnVal = toStruct(value, name, logger);

    else if (type == QLatin1String("union"))
        returnVal = toUnion(value, name, logger);

    else if (type == QLatin1String("bitfield"))
        returnVal = toBitfield(value, name, logger);

    else if (type == QLatin1String("enum"))
        returnVal = toEnum(value, name, false, logger);

    else if (type == QLatin1String("flags"))
        returnVal = toEnum(value, name, true, logger);

    else if (type == QLatin1String("string"))
        returnVal = toString(value, name, logger);

    //now it can only be a primitive type or something invalid
    else
        returnVal = toPrimitive(value, name, logger);

    if (returnVal)
    {
        //successfully parsed -> add the validate and update functions to the additional data
        QScriptValue updateFunc = value.property(QLatin1String("updateFunc"));
        QScriptValue validationFunc = value.property(QLatin1String("validationFunc"));
        QScriptValue byteOrder = value.property(QLatin1String("byteOrder"));
        returnVal->setByteOrder(AbstractStructureParser::byteOrderFromString(byteOrder.toString()));
        AdditionalData* aData = 0;
        if (updateFunc.isFunction())
        {
            aData = new AdditionalData();
            aData->setUpdateFunction(updateFunc);
        }
        if (validationFunc.isFunction())
        {
            if (!aData)
                aData = new AdditionalData();
            aData->setValidationFunction(validationFunc);
        }
        if (aData)
            returnVal->setAdditionalData(aData);
    }
    return returnVal;
}

ArrayDataInformation * toArray(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    //we can safely assume that type == "array"
    int length = value.property(QLatin1String("length")).toInt32();
    if (length < 0)
    {
        logger->error(QLatin1String("array length is less than 0: ") + QString::number(length), value);
        return 0;
    }
    QScriptValue childType = value.property(QLatin1String("childType"));
    DataInformation* arrayType = toDataInformation(childType, QString(), logger);
    if (!arrayType)
    {
        logger->error(QLatin1String("Failed to convert array type to valid object!"), childType);
        return 0;
    }
    return new ArrayDataInformation(name, length, arrayType);
}

AbstractBitfieldDataInformation* toBitfield(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    //we can safely assume that type == "bitfield"
    int width = value.property(QLatin1String("width")).toInt32();
    if (width < 0 || width > 64)
    {
        logger->error(QLatin1String("bitfield width is outside of range 1-64: ") + QString::number(width), value);
        return 0;
    }
    QString bitfieldType = value.property(QLatin1String("bitfieldType")).toString();
    if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(name, width);

    else if (bitfieldType.toLower() == QLatin1String("signed"))
        return new SignedBitfieldDataInformation(name, width);

    else if (bitfieldType.toLower() == QLatin1String("unsigned"))
        return new UnsignedBitfieldDataInformation(name, width);

    logger->error(QLatin1String("invalid bitfield type specified: '") + bitfieldType
            + QLatin1String("'. Only 'bool', 'signed' or 'unsigned' are valid."));
    return 0;
}

PrimitiveDataInformation* toPrimitive(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    QString typeString = value.isString() ? value.toString() : value.property(QLatin1String("type")).toString();
    if (typeString.isEmpty()) {
        logger->error(QLatin1String("Invalid object passed, cannot convert it to primitive: ")
                + ScriptUtils::qScriptValueToString(value));
        return 0;
    }
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(typeString);
    if (primitiveType == Type_NotPrimitive || primitiveType == Type_Bitfield)
    {
        logger->error(QLatin1String("unrecognised primitive type: '") + typeString
                + QLatin1Char('\''));
        return 0;
    }
    return PrimitiveFactory::newInstance(name, primitiveType);
}

StructureDataInformation* toStruct(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    QScriptValue valueChildren = value.property(QLatin1String("children"));
    QVector<DataInformation*> fields = convertValues(valueChildren, logger);

    if (fields.isEmpty())
        logger->info(QLatin1String("No children were found for struct, this is probably a mistake."), valueChildren);

    StructureDataInformation* structData = new StructureDataInformation(name);
    structData->setChildren(fields);
    return structData;
}

UnionDataInformation* toUnion(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    QScriptValue valueChildren = value.property(QLatin1String("children"));
    QVector<DataInformation*> fields = convertValues(valueChildren, logger);

    if (fields.isEmpty())
        logger->info(QLatin1String("No children were found for union, this is probably a mistake."), valueChildren);

    UnionDataInformation* unionData = new UnionDataInformation(name);
    unionData->setChildren(fields);
    return unionData;
}

AbstractEnumDataInformation* toEnum(const QScriptValue& value, const QString& name, bool flags, ScriptLogger* logger)
{
    //TODO also allow bitfields
    QScriptValue typeProperty = value.property(QLatin1String("enumType"));
    QString typeString = typeProperty.toString();
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(typeString);
    if (primitiveType == Type_NotPrimitive)
    {
        logger->warn(QLatin1String("Could not convert variable to primitve type identifier!"), typeProperty);
        return 0;
    }
    QScriptValue enumValuesObj = value.property(QLatin1String("enumValues"));
    if (!enumValuesObj.isValid() || !enumValuesObj.isObject())
    {
        logger->warn(QLatin1String("Could not parse enumerators since variable of type "
                "object was expected!"), enumValuesObj);
        return 0;
    }
    QMap<AllPrimitiveTypes, QString> enumValues = AbstractEnumDataInformation::parseEnumValues(enumValuesObj, primitiveType);

    QString enumName = value.property(QLatin1String("enumName")).toString();
    EnumDefinition::Ptr def(new EnumDefinition(enumValues, enumName, primitiveType));
    PrimitiveDataInformation* primData = PrimitiveFactory::newInstance(name, primitiveType);
    if (!primData)
        return NULL;

    if (flags)
        return new FlagDataInformation(name, primData, def);
    else
        return new EnumDataInformation(name, primData, def);
}

StringDataInformation* toString(const QScriptValue& value, const QString& name, ScriptLogger* logger)
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

    StringDataInformation* data = new StringDataInformation(name, encoding.toString());
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
