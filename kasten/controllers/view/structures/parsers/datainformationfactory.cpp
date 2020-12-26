/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "datainformationfactory.hpp"

#include "../datatypes/primitive/bitfield/boolbitfielddatainformation.hpp"
#include "../datatypes/primitive/bitfield/unsignedbitfielddatainformation.hpp"
#include "../datatypes/primitive/bitfield/signedbitfielddatainformation.hpp"
#include "../datatypes/primitivefactory.hpp"

#include "../script/scriptlogger.hpp"
#include <QScriptEngine>

AbstractBitfieldDataInformation* DataInformationFactory::newBitfield(const BitfieldParsedData& pd)
{
    if (!pd.width.isValid) {
        if (pd.width.string.isEmpty()) {
            pd.error() << "Bitfield is missing width.";
        } else {
            pd.error() << "Width of bitfield is not a valid number: " << pd.width.string;
        }
        return nullptr;
    }
    if (pd.width.value <= 0 || pd.width.value > 64) {
        pd.error() << "Width of bitfield is not a value from 1-64:" << pd.width.value;
        return nullptr;
    }
    AbstractBitfieldDataInformation* bitf = nullptr;
    const QString type = pd.type.toLower();
    if (type.isEmpty()) {
        pd.info() << "No bitfield type specified, defaulting to unsigned.";
        bitf = new UnsignedBitfieldDataInformation(pd.name, pd.width.value, pd.parent);
    } else if (type == QLatin1String("bool")) {
        bitf = new BoolBitfieldDataInformation(pd.name, pd.width.value, pd.parent);
    } else if (type == QLatin1String("unsigned")) {
        bitf = new UnsignedBitfieldDataInformation(pd.name, pd.width.value, pd.parent);
    } else if (type == QLatin1String("signed")) {
        bitf = new SignedBitfieldDataInformation(pd.name, pd.width.value, pd.parent);
    } else {
        pd.error() << "invalid bitfield type attribute given:" << type;
        return nullptr;
    }
    return bitf;
}

PrimitiveDataInformation* DataInformationFactory::newPrimitive(const PrimitiveParsedData& pd)
{
    if (pd.type.isEmpty()) {
        pd.error() << "Type of primitive not specified, cannot create it!";
        return nullptr;
    }
    LoggerWithContext lwc(pd.logger, pd.context());
    PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(pd.type, lwc);
    if (primitiveType == PrimitiveDataType::Invalid || primitiveType == PrimitiveDataType::Bitfield) {
        pd.error() << "Unrecognized primitive type: " << pd.type;
        return nullptr;
    }
    return PrimitiveFactory::newInstance(pd.name, primitiveType, lwc, pd.parent);
}

namespace {
template <class T>
T* newEnumOrFlags(const EnumParsedData& pd)
{
    LoggerWithContext lwc(pd.logger, pd.context() + QLatin1String(" (type)"));
    const PrimitiveDataType primitiveType = PrimitiveFactory::typeStringToType(pd.type, lwc);
    if (primitiveType == PrimitiveDataType::Invalid || primitiveType == PrimitiveDataType::Bitfield) {
        pd.error() << "Unrecognized enum type: " << pd.type;
        return nullptr;
    }
    if (primitiveType == PrimitiveDataType::Float || primitiveType == PrimitiveDataType::Double) {
        pd.error() << "Floating-point enums are not allowed since they make little sense.";
        return nullptr;
    }
    EnumDefinition::Ptr definition = pd.enumDef;
    if (!definition) {
        QMap<AllPrimitiveTypes, QString> enumValues =
            EnumDefinition::parseEnumValues(pd.enumValuesObject, lwc, primitiveType);
        if (enumValues.isEmpty()) {
            pd.error() << "No enum values specified!";
            return nullptr;
        }
        definition = EnumDefinition::Ptr(new EnumDefinition(enumValues, pd.enumName, primitiveType));
    }
    if (definition->type() != primitiveType) {
        pd.error().nospace() << "Enum type (" << definition->type() << ") and value type (" << primitiveType
                             << ") do not match!";
        return nullptr;
    }
    PrimitiveDataInformation* primData = PrimitiveFactory::newInstance(pd.name, primitiveType, lwc);
    // TODO allow bitfields?
    if (!primData) {
        pd.error() << "Could not create a value object for this enum!";
        return nullptr;
    }
    return new T(pd.name, primData, definition, pd.parent);
}

template <class T>
T* newStructOrUnion(const StructOrUnionParsedData& supd)
{
    T* structOrUnion = new T(supd.name, QVector<DataInformation*>(), supd.parent);
    supd.children->setParent(structOrUnion);
    while (supd.children->hasNext()) {
        DataInformation* data = supd.children->next();
        if (data) {
            structOrUnion->appendChild(data, false);
        } else {
            return nullptr; // error message should be logged already
        }
    }
    if (structOrUnion->childCount() == 0) {
        supd.info() << "No children were found, this is probably a mistake.";
    }
    return structOrUnion;
}

QString generateLengthFunction(DataInformation* current, DataInformation* last, const QString& elemName,
                               const QString& currentString, const ParserInfo& info)
{
    if (!current) { // reached top
        return {};
    }
    for (int i = current->childCount() - 1; i >= 0; --i) {
        DataInformation* child = current->childAt(i);
        if (child == last) {
            return {}; // don't go down again after going up one level

        }
        QString childName = child->name();
        if (childName == elemName) {
            QString function = QLatin1String("function() { return this.parent.") + currentString
                               + elemName + QLatin1String(".value; }");
            info.info() << "Found element for dynamic array length: " << child->fullObjectPath()
                        << ", resulting function is: " << function;
            return function;
        }
        if (child->childCount() > 0) {
            QString func = generateLengthFunction(child, current, elemName,
                                                  currentString + childName + QLatin1Char('.'), info);
            // if func is empty no valid child was found, just continue
            if (!func.isEmpty()) {
                return func;
            }
        }
        // has no children and was not the desired element -> continue loop
    }

    // now check parents
    DataInformationBase* nextBase = current->parent();
    if (!nextBase) {
        return {};
    }

    DataInformation* next = nextBase->asDataInformation();
    if (next == last) {
        return {}; // we moved one level down previously, don't move up again
    }

    return generateLengthFunction(current->parent()->asDataInformation(), current, elemName,
                                  currentString + QLatin1String("parent."), info);
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
    if (!pd.arrayType) {
        pd.error() << "Failed to parse array type!";
        return nullptr;
    }
    if (!pd.length.isValid()) {
        pd.error() << "No array length specified!";
        return nullptr;
    }
    const ParsedNumber<uint> fixedLength = ParserUtils::uintFromScriptValue(pd.length);
    if (fixedLength.isValid) {
        return new ArrayDataInformation(pd.name, fixedLength.value, pd.arrayType, pd.parent, QScriptValue());
    }
    if (pd.length.isFunction()) {
        return new ArrayDataInformation(pd.name, 0, pd.arrayType, pd.parent, pd.length);
    }

    // neither integer nor function, must be a string containing the name of another element.
    const QString lengthStr = pd.length.toString();
    if (!pd.parent) {
        pd.error() << "Toplevel array has length depending on other field (" << lengthStr
                    << "). This is not possible.";
        return nullptr;
    }
    if (lengthStr.contains(QLatin1Char('.'))) {
        pd.error() << "Referenced array length element (" << lengthStr << ") contains '.', this is not allowed!";
        return nullptr; // TODO maybe add possible shorthand length="this.parent.length"
    }
    QString lengthFunctionString = generateLengthFunction(pd.parent, nullptr, lengthStr, QString(), pd);
    if (lengthFunctionString.isEmpty()) {
        pd.error() << "Could not find element " << lengthStr << " referenced as array length!";
        return nullptr;
    }
    QScriptValue lengthFunction = ParserUtils::functionSafeEval(pd.engine, lengthFunctionString);
    return new ArrayDataInformation(pd.name, 0, pd.arrayType, pd.parent, lengthFunction);
}

StringDataInformation* DataInformationFactory::newString(const StringParsedData& pd)
{
    if (pd.maxByteCount.isValid && pd.maxCharCount.isValid) {
        pd.error() << "Both maxCharCount and maxByteCount are set, only one is allowed.";
        return nullptr;
    }
    StringDataInformation::StringType encoding = ParserUtils::toStringEncoding(pd.encoding,
                                                                               LoggerWithContext(pd.logger, pd.context()));
    if (encoding == StringDataInformation::StringType::InvalidEncoding) {
        pd.error() << "Bad string encoding given:" << pd.encoding;
        return nullptr;
    }
    auto* data = new StringDataInformation(pd.name, encoding, pd.parent);
    bool modeSet = false;
    if (pd.termination.isValid) {
        data->setTerminationCodePoint(pd.termination.value);
        modeSet = true;
    }
    if (pd.maxByteCount.isValid) {
        data->setMaxByteCount(pd.maxByteCount.value);
        modeSet = true;
    }
    if (pd.maxCharCount.isValid) {
        data->setMaxCharCount(pd.maxCharCount.value);
        modeSet = true;
    }
    // if mode is None, we assume zero terminated strings
    if (!modeSet) {
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

    if (data->name().isEmpty()) {
        pd.warn() << "Name is empty!";
    }
    if (pd.updateFunc.isValid()) {
        if (!pd.updateFunc.isFunction()) {
            pd.error() << "Update function is not a function: " << pd.updateFunc.toString();
            return false;
        }

        data->setUpdateFunc(pd.updateFunc);
    }
    if (pd.validationFunc.isValid()) {
        if (!pd.validationFunc.isFunction()) {
            pd.error() << "Validation function is not a function: " << pd.validationFunc.toString();
            return false;
        }

        data->setValidationFunc(pd.validationFunc);
    }
    if (pd.toStringFunc.isValid()) {
        if (!pd.toStringFunc.isFunction()) {
            pd.error() << "To string function is not a function: " << pd.toStringFunc.toString();
            return false;
        }

        data->setToStringFunction(pd.toStringFunc);
    }
    if (!pd.customTypeName.isEmpty()) {
        data->setCustomTypeName(pd.customTypeName);
    }
    return true;

}

PointerDataInformation* DataInformationFactory::newPointer(const PointerParsedData& pd)
{
    if (!pd.pointerTarget) {
        pd.error() << "Missing pointer target";
        return nullptr;
    }
    if (!pd.valueType) {
        pd.error() << "Missing pointer type";
        return nullptr;
    }
    if (!pd.valueType->isPrimitive()) {
        pd.error() << "Bad pointer type, only unsigned integers are allowed";
        return nullptr;
    }
    if (pd.interpretFunc.isValid() && !pd.interpretFunc.isFunction()) {
        pd.error() << "Bad pointer interpretation, only functions are allowed";
        return nullptr;
    }
    PrimitiveDataInformation* primValue = pd.valueType->asPrimitive();
    if (!(primValue->type() == PrimitiveDataType::UInt8 || primValue->type() == PrimitiveDataType::UInt16
          || primValue->type() == PrimitiveDataType::UInt32 || primValue->type() == PrimitiveDataType::UInt64)) {
        pd.error() << "Bad pointer type, only unsigned integers are allowed"; // TODO offsets (signed int + bitfields)
        return nullptr;
    }
    return new PointerDataInformation(pd.name, pd.pointerTarget, primValue, pd.parent,
                                      pd.pointerScale, pd.interpretFunc);
}

TaggedUnionDataInformation* DataInformationFactory::newTaggedUnion(const TaggedUnionParsedData& pd)
{
    QScopedPointer<TaggedUnionDataInformation> tagged(new TaggedUnionDataInformation(pd.name, pd.parent));
    pd.children->setParent(tagged.data());
    while (pd.children->hasNext()) {
        DataInformation* data = pd.children->next();
        if (data) {
            tagged->appendChild(data, false);
        } else {
            return nullptr; // error message should be logged already
        }
    }
    if (tagged->childCount() == 0) {
        pd.error() << "No children in tagged union. At least one is required so that tag can be evaluated.";
        return nullptr;
    }
    // verify alternatives
    bool alternativesValid = true;
    QVector<TaggedUnionDataInformation::FieldInfo> altInfo;
    altInfo.reserve(pd.alternatives.size());
    for (int i = 0; i < pd.alternatives.size(); ++i) {
        const TaggedUnionParsedData::Alternatives& fi = pd.alternatives.at(i);
        if (!fi.selectIf.isFunction()) {
            ParsedNumber<quint64> number = ParserUtils::uint64FromScriptValue(fi.selectIf);
            if (!number.isValid) {
                pd.error() << "Alternative number" << i << "is not valid. SelectIf is neither function nor number!";
                alternativesValid = false;
            }
            // number is valid -> there must be exactly one field
            if (tagged->childCount() != 1) {
                pd.error() << "Alternative number" << i << "is not valid. SelectIf is number,"
                    " but there is not exactly one child!";
                alternativesValid = false;
            }
        }
        QVector<DataInformation*> children;
        while (fi.fields->hasNext()) {
            DataInformation* next = fi.fields->next();
            if (next) {
                children.append(next);
            } else {
                pd.error() << "Alternative number" << i << "has an invalid field!";
                alternativesValid = false;
            }
        }
        altInfo.append(TaggedUnionDataInformation::FieldInfo(fi.name, fi.selectIf, children));

    }

    if (!alternativesValid) {
        for (auto& info : qAsConst(altInfo)) {
            qDeleteAll(info.fields);
        }

        return nullptr;
    }
    tagged->setAlternatives(altInfo, false);

    pd.defaultFields->setParent(tagged.data());
    while (pd.defaultFields->hasNext()) {
        DataInformation* data = pd.defaultFields->next();
        if (data) {
            tagged->appendDefaultField(data, false);
        } else {
            return nullptr; // error message should be logged already
        }
    }
    return tagged.take();
}
