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
        return toPrimitive(value, name); //a type string is also okay

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
        //TODO allow int values too
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
    Q_UNUSED(logger)
    //we can safely assume that type == "array"
    int length = value.property(QLatin1String("length")).toInt32();
    if (length < 0)
    {
        ScriptUtils::object()->logScriptError(QLatin1String("array length < 0 -> return NULL"));
        return NULL;
    }
    QScriptValue childType = value.property(QLatin1String("childType"));
    DataInformation* arrayType = toDataInformation(childType,
            childType.property(QLatin1String("name")).toString());
    if (!arrayType)
    {
        ScriptUtils::object()->logScriptError(QLatin1String("could not parse array type -> return NULL"));
        return 0;
    }
    return new ArrayDataInformation(name, length, arrayType);
}

AbstractBitfieldDataInformation* toBitfield(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    //we can safely assume that type == "bitfield"
    int width = value.property(QLatin1String("width")).toInt32();
    if (width < 0)
    {
        ScriptUtils::object()->logScriptError(
                QLatin1String("bitfield has a length of < 0 -> return NULL"));
        return NULL;
    }
    QString bitfieldType = value.property(QLatin1String("bitfieldType")).toString();
    if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(name, width);

    else if (bitfieldType.toLower() == QLatin1String("signed"))
        return new SignedBitfieldDataInformation(name, width);

    else if (bitfieldType.toLower() == QLatin1String("unsigned"))
        return new UnsignedBitfieldDataInformation(name, width);

    ScriptUtils::object()->logScriptError(QLatin1String("invalid bitfield type specified:")
            + bitfieldType);
    return NULL;
}

PrimitiveDataInformation* toPrimitive(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    Q_UNUSED(logger)
    QString typeString = value.isString() ? value.toString() : value.property(QLatin1String("type")).toString();
    if (typeString.isEmpty()) {
        kDebug() << "empty type string passed (or type property not set) -> cannot convert to primitive";
        return NULL;
    }
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(typeString);
    if (primitiveType == Type_NotPrimitive || primitiveType == Type_Bitfield)
    {
        ScriptUtils::object()->logScriptError(QLatin1String("unrecognised type: '") + typeString
                + QLatin1String("' -> return NULL"));
        kDebug() << "unrecognised type: '" << typeString;
        return NULL;
    }
    return PrimitiveFactory::newInstance(name, primitiveType);
}

StructureDataInformation* toStruct(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    QVector<DataInformation*> fields;

    QScriptValue valueChildren = value.property(QLatin1String("children"));
    QScriptValueIterator it(valueChildren);
    while (it.hasNext())
    {
        it.next();
        QScriptValue val = it.value();
        if (!val.isObject())
            continue;
        DataInformation* data = toDataInformation(val, it.name());
        if (data)
            fields.append(data);
    }
    if (fields.size() == 0)
    {
        if (logger)
            logger->info(QLatin1String("No children were found for struct, this is probably a mistake."));
    }

    StructureDataInformation* structure = new StructureDataInformation(name);

    foreach(DataInformation* data, fields)
        {
            structure->addDataTypeToStruct(data);
        }
    return structure;
}

UnionDataInformation* toUnion(const QScriptValue& value, const QString& name, ScriptLogger* logger)
{
    Q_UNUSED(logger)
    QVector<DataInformation*> fields;

    QScriptValue valueChildren = value.property(QLatin1String("children"));
    QScriptValueIterator it(valueChildren);
    while (it.hasNext())
    {
        it.next();
        QScriptValue val = it.value();
        if (!val.isObject())
            continue;
        DataInformation* data = toDataInformation(val, it.name());
        if (data)
            fields.append(data);
    }
    if (fields.size() == 0)
    {
        ScriptUtils::object()->logScriptError(QLatin1String("no children were found in element '")
                + name + QLatin1String("' -> return NULL"));
        return NULL;
    }

    UnionDataInformation* unionInf = new UnionDataInformation(name);

    foreach(DataInformation* data, fields)
        {
            unionInf->addDataTypeToUnion(data);
        }
    return unionInf;
}

AbstractEnumDataInformation* toEnum(const QScriptValue& value, const QString& name, bool flags, ScriptLogger* logger)
{
    Q_UNUSED(logger)
    QString typeString = value.property(QLatin1String("enumType")).toString();
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(typeString);
    if (primitiveType == Type_NotPrimitive)
    {
        ScriptUtils::object()->logScriptError(QLatin1String("unrecognised enum type: '")
                + typeString + QLatin1String("' -> return NULL"));
        return NULL;
    }

    QScriptValue enumValuesObj = value.property(QLatin1String("enumValues"));
    if (!enumValuesObj.isValid() || !enumValuesObj.isObject()) {
        ScriptUtils::object()->logScriptError(QLatin1String("Could not parse enumValues as a number of enum values"));
        ScriptUtils::dumpQScriptValue(value, __FILE__, __LINE__);
        return NULL;
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
    Q_UNUSED(logger)
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
