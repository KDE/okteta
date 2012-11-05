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
#include "datainformationfactory.h"

#include "../datatypes/primitive/bitfield/boolbitfielddatainformation.h"
#include "../datatypes/primitive/bitfield/unsignedbitfielddatainformation.h"
#include "../datatypes/primitive/bitfield/signedbitfielddatainformation.h"
#include "../datatypes/primitivefactory.h"

#include "../script/scriptlogger.h"
#include <QScriptEngine>

AbstractBitfieldDataInformation* DataInformationFactory::newBitfield(const BitfieldParsedData& pd)
{
    if (!pd.width.isValid)
    {
        if (pd.width.string.isEmpty())
            pd.error() << "Bitfield is missing width.";
        else
            pd.error() << "Width of bitfield is not a valid number: " << pd.width.string;
        return 0;
    }
    if (pd.width.value <= 0 || pd.width.value > 64)
    {
        pd.error() << "Width of bitfield is not a value from 1-64:" << pd.width.value;
        return 0;
    }
    AbstractBitfieldDataInformation* bitf = 0;
    const QString type = pd.type.toLower();
    if (type.isEmpty())
    {
        pd.info() << "No bitfield type specified, defaulting to unsigned.";
        bitf = new UnsignedBitfieldDataInformation(pd.name, pd.width.value, pd.parent);
    }
    else if (type == QLatin1String("bool"))
        bitf = new BoolBitfieldDataInformation(pd.name, pd.width.value, pd.parent);
    else if (type == QLatin1String("unsigned"))
        bitf = new UnsignedBitfieldDataInformation(pd.name, pd.width.value, pd.parent);
    else if (type == QLatin1String("signed"))
        bitf = new SignedBitfieldDataInformation(pd.name, pd.width.value, pd.parent);
    else
    {
        pd.error() << "invalid bitfield type attribute given:" << type;
        return 0;
    }
    return bitf;
}

PrimitiveDataInformation* DataInformationFactory::newPrimitive(const PrimitiveParsedData& pd)
{
    if (pd.type.isEmpty())
    {
        pd.error() << "Type of primitive not specified, cannot create it!";
        return 0;
    }
    LoggerWithContext lwc(pd.logger, pd.context());
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(pd.type, lwc);
    if (primitiveType == Type_Invalid || primitiveType == Type_Bitfield)
    {
        pd.error() << "Unrecognized primitive type: " << pd.type;
        return 0;
    }
    return PrimitiveFactory::newInstance(pd.name, primitiveType, lwc, pd.parent);
}

namespace
{
template<class T>
T* newEnumOrFlags(const EnumParsedData& pd)
{
    LoggerWithContext lwc(pd.logger, pd.context() + QLatin1String(" (type)"));
    const PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(pd.type, lwc);
    if (primitiveType == Type_Invalid || primitiveType == Type_Bitfield)
    {
        pd.error() << "Unrecognized enum type: " << pd.type;
        return 0;
    }
    if (primitiveType == Type_Float || primitiveType == Type_Double)
    {
        pd.error() << "Floating-point enums are not allowed since they make little sense.";
        return 0;
    }
    EnumDefinition::Ptr definition = pd.enumDef;
    if (!definition)
    {
        QMap<AllPrimitiveTypes, QString> enumValues =
                EnumDefinition::parseEnumValues(pd.enumValuesObject, lwc, primitiveType);
        if (enumValues.isEmpty())
        {
            pd.error() << "No enum values specified!";
            return 0;
        }
        definition = EnumDefinition::Ptr(new EnumDefinition(enumValues, pd.enumName, primitiveType));
    }
    if (definition->type() != primitiveType)
    {
        pd.error().nospace() << "Enum type (" << definition->type() << ") and value type (" << primitiveType
                << ") do not match!";
        return 0;
    }
    PrimitiveDataInformation* primData = PrimitiveFactory::newInstance(pd.name, primitiveType, lwc);
    //TODO allow bitfields?
    if (!primData)
    {
        pd.error() << "Could not create a value object for this enum!";
        return 0;
    }
    return new T(pd.name, primData, definition, pd.parent);
}

template<class T>
T* newStructOrUnion(const StructOrUnionParsedData& supd)
{
    T* structOrUnion = new T(supd.name, QVector<DataInformation*>(), supd.parent);
    supd.children->setParent(structOrUnion);
    while (supd.children->hasNext())
    {
        DataInformation* data = supd.children->next();
        if (data)
            structOrUnion->appendChild(data, false);
        else
            return 0; //error message should be logged already
    }
    if (structOrUnion->childCount() == 0)
        supd.info() << "No children were found, this is probably a mistake.";
    return structOrUnion;
}

}

EnumDataInformation* DataInformationFactory::newEnum(const EnumParsedData& pd)
{
    return newEnumOrFlags<EnumDataInformation>(pd);
}

FlagDataInformation* DataInformationFactory::newFlags(const EnumParsedData& pd)
{
    return newEnumOrFlags<FlagDataInformation>(pd);
}

ArrayDataInformation* DataInformationFactory::newArray(const ArrayParsedData& pd)
{
    if (!pd.arrayType)
    {
        pd.error() << "Failed to parse array type!";
        return 0;
    }
    int initialLength = 0;
    if (!pd.length.isValid && !pd.lengthFunction.isValid())
    {
        pd.error() << "Neither fixed length nor length function specified, cannot create array.";
        return 0;
    }
    if (pd.length.isValid)
    {
        if (pd.length.value < 0)
        {
            pd.error() << "Cannot create array with negative length:" << pd.length.value;
            return 0;
        }
        initialLength = pd.length.value;
    }
    if (pd.lengthFunction.isValid())
    {
        if (!pd.lengthFunction.isFunction())
        {
            pd.error() << "Length function is not a function:" << pd.lengthFunction.toString();
            return 0;
        }
    }
    return new ArrayDataInformation(pd.name, initialLength, pd.arrayType, pd.parent, pd.lengthFunction);
}

StringDataInformation* DataInformationFactory::newString(const StringParsedData& pd)
{
    if (pd.maxByteCount.isValid && pd.maxCharCount.isValid)
    {
        pd.error() << "Both maxCharCount and maxByteCount are set, only one is allowed.";
        return 0;
    }
    StringDataInformation::StringType encoding = ParserUtils::toStringEncoding(pd.encoding,
            LoggerWithContext(pd.logger, pd.context()));
    if (encoding == StringDataInformation::InvalidEncoding)
    {
        pd.error() << "Bad string encoding given:" << pd.encoding;
        return 0;
    }
    StringDataInformation* data = new StringDataInformation(pd.name, encoding, pd.parent);
    bool modeSet = false;
    if (pd.termination.isValid)
    {
        data->setTerminationCodePoint(pd.termination.value);
        modeSet = true;
    }
    if (pd.maxByteCount.isValid)
    {
        data->setMaxByteCount(pd.maxByteCount.value);
        modeSet = true;
    }
    if (pd.maxCharCount.isValid)
    {
        data->setMaxCharCount(pd.maxCharCount.value);
        modeSet = true;
    }
    //if mode is None, we assume zero terminated strings
    if (!modeSet)
    {
        pd.info() << "No string termination mode set, assuming null terminated strings.";
        data->setTerminationCodePoint(0);
        Q_ASSERT(data->terminationMode() == StringData::Sequence);
    }
    return data;
}

UnionDataInformation* DataInformationFactory::newUnion(const StructOrUnionParsedData& pd)
{
    return newStructOrUnion<UnionDataInformation>(pd);
}

StructureDataInformation* DataInformationFactory::newStruct(const StructOrUnionParsedData& pd)
{
    return newStructOrUnion<StructureDataInformation>(pd);
}

bool DataInformationFactory::commonInitialization(DataInformation* data, const CommonParsedData& pd)
{
    data->setByteOrder(pd.endianess);

    if (data->name().isEmpty())
    {
        pd.warn() << "Name is empty!";
    }
    if (pd.updateFunc.isValid())
    {
        if (!pd.updateFunc.isFunction())
        {
            pd.error() << "Update function is not a function: " << pd.updateFunc.toString();
            return false;
        }
        else
            data->setUpdateFunc(pd.updateFunc);
    }
    if (pd.validationFunc.isValid())
    {
        if (!pd.validationFunc.isFunction())
        {
            pd.error() << "Validation function is not a function: " << pd.validationFunc.toString();
            return false;
        }
        else
            data->setValidationFunc(pd.validationFunc);
    }
    return true;

}

PointerDataInformation* DataInformationFactory::newPointer(const PointerParsedData& pd)
{
    if (!pd.pointerTarget)
    {
        pd.error() << "Missing pointer target";
        return 0;
    }
    if (!pd.valueType)
    {
        pd.error() << "Missing pointer type";
        return 0;
    }
    if (!pd.valueType->isPrimitive())
    {
        pd.error() << "Bad pointer type, only unsigned integers are allowed";
        return 0;
    }
    PrimitiveDataInformation* primValue = pd.valueType->asPrimitive();
    if (!(primValue->type() == Type_UInt8 || primValue->type() == Type_UInt16
            || primValue->type() == Type_UInt32 || primValue->type() == Type_UInt64))
    {
        pd.error() << "Bad pointer type, only unsigned integers are allowed"; //TODO offsets (signed int + bitfields)
        return 0;
    }
    return new PointerDataInformation(pd.name, pd.pointerTarget, primValue, pd.parent);
}

TaggedUnionDataInformation* DataInformationFactory::newTaggedUnion(const TaggedUnionParsedData& pd)
{
    QScopedPointer<TaggedUnionDataInformation> tagged(new TaggedUnionDataInformation(pd.name, pd.parent));
    pd.children->setParent(tagged.data());
    while (pd.children->hasNext())
    {
        DataInformation* data = pd.children->next();
        if (data)
            tagged->appendChild(data, false);
        else
            return 0; //error message should be logged already
    }
    if (tagged->childCount() == 0)
    {
        pd.error() << "No children in tagged union. At least one is required so that tag can be evaluated.";
        return 0;
    }
    //verify alternatives
    bool alternativesValid = true;
    QVector<TaggedUnionDataInformation::FieldInfo> altInfo;
    for(int i = 0; i < pd.alternatives.size(); ++i)
    {
        const TaggedUnionParsedData::Alternatives& fi = pd.alternatives.at(i);
        if (!fi.selectIf.isFunction())
        {
            ParsedNumber<quint64> number = ParserUtils::uint64FromScriptValue(fi.selectIf);
            if (!number.isValid)
            {
                pd.error() << "Alternative number" << i << "is not valid. SelectIf is neither function nor number!";
                alternativesValid = false;
            }
            //number is valid -> there must be exactly one field
            if (tagged->childCount() != 1)
            {
                pd.error() << "Alternative number" << i << "is not valid. SelectIf is number,"
                        " but there is not exactly one child!";
                alternativesValid = false;
            }
        }
        QVector<DataInformation*> children;
        while (fi.fields->hasNext())
        {
            DataInformation* next = fi.fields->next();
            if (next)
                children.append(next);
            else
            {
                pd.error() << "Alternative number" << i << "has an invalid field!";
                alternativesValid = false;
            }
        }
        altInfo.append(TaggedUnionDataInformation::FieldInfo(fi.name, fi.selectIf, children));

    }
    if (!alternativesValid)
    {
        for (int i = 0; i < altInfo.size(); ++i)
            qDeleteAll(altInfo.at(i).fields);
        return 0;
    }
    tagged->setAlternatives(altInfo, false);

    pd.defaultFields->setParent(tagged.data());
    while (pd.defaultFields->hasNext())
    {
        DataInformation* data = pd.defaultFields->next();
        if (data)
            tagged->appendDefaultField(data, false);
        else
            return 0; //error message should be logged already
    }
    return tagged.take();
}
