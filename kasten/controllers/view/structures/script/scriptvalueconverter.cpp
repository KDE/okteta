/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include <KDebug>
#include <QtScript/QScriptValueList>
#include <QtScript/QScriptValueIterator>
#include <QtCore/QScopedPointer>

#include "../datatypes/staticlengtharraydatainformation.h"
#include "../datatypes/uniondatainformation.h"
#include "../datatypes/structuredatainformation.h"
#include "../datatypes/enumdatainformation.h"
#include "../datatypes/enumdefinition.h"
#include "../datatypes/strings/stringdata.h"
#include "../datatypes/strings/stringdatainformation.h"
#include "../datatypes/primitive/bitfield/boolbitfielddatainformation.h"
#include "../datatypes/primitive/bitfield/unsignedbitfielddatainformation.h"
#include "../datatypes/primitive/bitfield/signedbitfielddatainformation.h"
#include "../datatypes/primitivefactory.h"
#include "../datatypes/additionaldata.h"
#include "../parsers/abstractstructureparser.h"
#include "scriptutils.h"

ScriptValueConverter::ScriptValueConverter(QScriptValue& value, QString name) :
    mValue(value), mName(name)
{
}

ScriptValueConverter::~ScriptValueConverter()
{
}

DataInformation* ScriptValueConverter::convert()
{
    QScriptValue evaluatedVal;
    if (mValue.isFunction())
    {
        QScriptValue thisObj = mValue.engine()->newObject();
        QScriptValueList args;
        evaluatedVal = mValue.call(thisObj, args);
    }
    else if (mValue.isObject()) //this must be checked second since any function is also an object
        evaluatedVal = mValue;
    else
    {
        ScriptUtils::object()->logScriptError("value is neither function nor object");
        return NULL;
    }
        //FIXME evaluate all -> make this return QLIst<DataInformation*>
    //    ScriptUtils::object()->dumpQScriptValue(evaluatedVal,__FILE__,__LINE__);
    return toDataInformation(evaluatedVal, mName); //could be NULL
}

DataInformation* ScriptValueConverter::toDataInformation(QScriptValue value, QString name) const
{
    DataInformation* returnVal;
    if (!value.isObject())
        return NULL; //no point trying to convert

    if (name.isEmpty())
        name = i18n("<no name specified>");

    QString type = value.property("type").toString().toLower(); //to lower just to be safe

    if (type == QLatin1String("array"))
        returnVal = toArray(value, name);

    else if (type == QLatin1String("struct"))
        returnVal = toStruct(value, name);

    else if (type == QLatin1String("union"))
        returnVal = toUnion(value, name);

    else if (type == QLatin1String("bitfield"))
        returnVal = toBitfield(value, name);

    else if (type == QLatin1String("enum"))
        returnVal = toEnum(value, name);

    else if (type == QLatin1String("string"))
        returnVal = toString(value, name);

    //now it can only be a primitive type or something invalid
    else
        returnVal = toPrimitive(value, name);

    if (returnVal)
    {
        //successfully parsed -> add the validate and update functions to the additional data
        QScriptValue updateFunc = value.property("updateFunc");
        QScriptValue validationFunc = value.property("validationFunc");
        QScriptValue byteOrder = value.property("byteOrder");
        //TODO allow int values too
        returnVal->setByteOrder(AbstractStructureParser::byteOrderFromString(byteOrder.toString()));
        AdditionalData* aData = new AdditionalData();
        if (updateFunc.isFunction())
            aData->setUpdateFunction(new QScriptValue(updateFunc));
        if (validationFunc.isFunction())
            aData->setValidationFunction(new QScriptValue(validationFunc));
        returnVal->setAdditionalData(aData);
    }
    return returnVal;
}

AbstractArrayDataInformation * ScriptValueConverter::toArray(QScriptValue& value,
        QString& name) const
{
    //we can safely assume that type == "array"
    int length = value.property("length").toInt32();
    if (length <= 0)
    {
        ScriptUtils::object()->logScriptError("array length <= 0 -> return NULL");
        return NULL;
    }
    QScriptValue childType = value.property("childType");
    QScopedPointer<DataInformation> arrayType(toDataInformation(childType,
            childType.property("type").toString()));
    if (!arrayType)
    {
        ScriptUtils::object()->logScriptError(
                "could not parse array type -> return NULL");
        return NULL;
    }

    return new StaticLengthArrayDataInformation(name, length, *arrayType);

}

AbstractBitfieldDataInformation* ScriptValueConverter::toBitfield(
        QScriptValue& value, QString& name) const
{
    //we can safely assume that type == "bitfield"
    int width = value.property("width").toInt32();
    if (width <= 0)
    {
        ScriptUtils::object()->logScriptError(
                "bitfield has a length of <= 0 -> return NULL");
        return NULL;
    }
    QString bitfieldType = value.property("bitfieldType").toString();
    if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(name, width);

    else if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(name, width);

    else if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(name, width);

    ScriptUtils::object()->logScriptError("invalid bitfield type specified:"
            + bitfieldType);
    return NULL;
}

PrimitiveDataInformation * ScriptValueConverter::toPrimitive(QScriptValue& value,
        QString& name) const
{
    QString typeString = value.property("type").toString();
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(
            typeString);
    if (primitiveType == Type_NotPrimitive)
    {
        ScriptUtils::object()->logScriptError("unrecognised type: '" + typeString
                + "' -> return NULL");
        return NULL;
    }
    return PrimitiveFactory::newInstance(name, primitiveType);
}

StructureDataInformation* ScriptValueConverter::toStruct(QScriptValue& value,
        QString& name) const
{
    QList<DataInformation*> fields;

    QScriptValue valueChildren = value.property("children");
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
    if (fields.length() == 0)
    {
        ScriptUtils::object()->logScriptError("no children were found in element '"
                + name + "' -> return NULL");
        return NULL;
    }

    StructureDataInformation* structure = new StructureDataInformation(name);

    foreach(DataInformation* data, fields)
        {
            structure->addDataTypeToStruct(data);
        }
    return structure;
}

UnionDataInformation* ScriptValueConverter::toUnion(QScriptValue& value,
        QString& name) const
{
    QList<DataInformation*> fields;

    QScriptValue valueChildren = value.property("children");
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
    if (fields.length() == 0)
    {
        ScriptUtils::object()->logScriptError("no children were found in element '"
                + name + "' -> return NULL");
        return NULL;
    }

    UnionDataInformation* unionInf = new UnionDataInformation(name);

    foreach(DataInformation* data, fields)
        {
            unionInf->addDataTypeToUnion(data);
        }
    return unionInf;
}

EnumDataInformation* ScriptValueConverter::toEnum(QScriptValue& value, QString& name) const
{
    QMap<AllPrimitiveTypes, QString> enumValues;
    QScriptValueIterator it(value.property("enumValues"));
    while (it.hasNext())
    {
        it.next();
        QScriptValue val = it.value();
        if (val.isNumber())
        {
            enumValues.insert(val.toUInt32(), it.name());
        }
    }
    QString typeString = value.property("enumType").toString();
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(
            typeString);
    if (primitiveType == Type_NotPrimitive)
    {
        ScriptUtils::object()->logScriptError("unrecognised enum type: '"
                + typeString + "' -> return NULL");
        return NULL;
    }
    QString enumName = value.property("enumName").toString();
    EnumDefinition::Ptr def(new EnumDefinition(enumValues, enumName, primitiveType));
    PrimitiveDataInformation* primData = PrimitiveFactory::newInstance(name,
            primitiveType);
    if (primData)
        return new EnumDataInformation(name, primData, def);
    else
        return NULL;
}

StringDataInformation* ScriptValueConverter::toString(QScriptValue& value, QString& name) const
{
    const QScriptValue terminatedBy = value.property("terminatedBy");
    const QScriptValue charCount = value.property("charCount");
    const QScriptValue byteCount = value.property("byteCount");
    const QScriptValue encoding = value.property("encoding");

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
    bool ok;
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

QList<DataInformation*> ScriptValueConverter::convertValues()
{
    QList<DataInformation*> ret;
    QScriptValueIterator it(mValue);
    while (it.hasNext())
    {
        it.next();
        DataInformation* inf = toDataInformation(it.value(), it.name());
        if (inf)
            ret.append(inf);
        else
            kDebug() << "could not convert " << it.name();
    }
    return ret;
}
