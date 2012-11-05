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
#ifndef PARSERUTILS_H_
#define PARSERUTILS_H_

#include <QDebug>
#include <QStringList>
#include <KLocalizedString>

#include "../datatypes/datainformation.h"
#include "../datatypes/strings/stringdatainformation.h"
#include "../script/scriptlogger.h"

//forward declare so I don't have to repeat in all the parsers
class ArrayDataInformation;
class PrimitiveDataInformation;
class AbstractBitfieldDataInformation;
class StructureDataInformation;
class UnionDataInformation;
class EnumDataInformation;
class StringDataInformation;
class TaggedUnionDataInformation;
class PointerDataInformation;

class QScriptValue;

/** For use by the parsers so that the functions don't have as many parameters */
struct ParserInfo
{
    inline ParserInfo(const QString& name, ScriptLogger* logger, DataInformation* parent)
            : name(name), logger(logger), parent(parent)
    {
        Q_CHECK_PTR(logger);
    }
    inline ParserInfo(const ParserInfo& i)
            : name(i.name), logger(i.logger), parent(i.parent) {}
    inline ~ParserInfo() {}
    QString name;
    ScriptLogger* logger;
    DataInformation* parent;

    inline QString context() const
    {
        return parent ? parent->fullObjectPath() + QLatin1Char('.') + name : name;
    }
    inline QDebug info() const { return logger->info(context()); }
    inline QDebug warn() const { return logger->warn(context()); }
    inline QDebug error() const { return logger->error(context()); }
};

class ChildrenParser {
public:
    virtual DataInformation* next() = 0;
    virtual bool hasNext() = 0;
    virtual void setParent(DataInformation* parent) = 0;
    virtual ~ChildrenParser() {};
};

/**
 * Holds a number that was converted either from a QScriptValue or a QString
 */
template<typename T>
struct ParsedNumber {
    ParsedNumber() : value(0), isValid(false) {}
    ParsedNumber(T val, const QString& str, bool ok) : string(str), value(val), isValid(ok) {}
    QString string;
    T value;
    bool isValid;
    static ParsedNumber<T> badInput(const QString& str) { return ParsedNumber<T>(T(), str, false); }
    inline bool isError() { return !isValid && !string.isEmpty(); }
    inline bool isEmpty() { Q_ASSERT(!isValid); return string.isEmpty(); }
};

namespace ParserStrings {
    const QString TYPE_ARRAY = QLatin1String("array");
    const QString TYPE_BITFIELD = QLatin1String("bitfield");
    const QString TYPE_ENUM = QLatin1String("enum");
    const QString TYPE_FLAGS = QLatin1String("flags");
    const QString TYPE_PRIMITIVE = QLatin1String("primitive");
    const QString TYPE_STRING = QLatin1String("string");
    const QString TYPE_STRUCT = QLatin1String("struct");
    const QString TYPE_UNION = QLatin1String("union");
    const QString TYPE_POINTER = QLatin1String("pointer");
    const QString TYPE_TAGGED_UNION = QLatin1String("taggedUnion");
    /** Only needed for .osd */
    const QString TYPE_ENUMDEF = QLatin1String("enumDef");
    const QString TYPE_ALTERNATIVES = QLatin1String("alternatives");
    const QString TYPE_GROUP= QLatin1String("group");


    //all types
    const QString PROPERTY_NAME = QLatin1String("name");
    const QString PROPERTY_BYTEORDER = QLatin1String("byteOrder");
    const QString PROPERTY_PARENT= QLatin1String("parent");
    const QString PROPERTY_VALIDATION_ERROR = QLatin1String("validationError");
    const QString PROPERTY_VALID = QLatin1String("valid");
    const QString PROPERTY_ABLE_TO_READ = QLatin1String("wasAbleToRead");
    const QString PROPERTY_UPDATE_FUNC = QLatin1String("updateFunc");
    const QString PROPERTY_VALIDATION_FUNC = QLatin1String("validationFunc");
    const QString PROPERTY_DATATYPE = QLatin1String("datatype");
    //enum
    const QString PROPERTY_ENUM_VALUES = QLatin1String("enumValues");
    //this one is javascript only
    const QString PROPERTY_ENUM_NAME = QLatin1String("enumName");
    //array/bitfield
    const QString PROPERTY_TYPE = QLatin1String("type");
    //array
    const QString PROPERTY_LENGTH = QLatin1String("length");
    //bitfield
    const QString PROPERTY_WIDTH = QLatin1String("width");
    //struct/union
    const QString PROPERTY_CHILDREN = QLatin1String("fields");
    const QString PROPERTY_CHILD_COUNT = QLatin1String("childCount");
    const QString PROPERTY_CHILD = QLatin1String("child");
    //strings
    const QString PROPERTY_CHAR_COUNT = QLatin1String("charCount");
    const QString PROPERTY_BYTE_COUNT = QLatin1String("byteCount");
    const QString PROPERTY_MAX_CHAR_COUNT = QLatin1String("maxCharCount");
    const QString PROPERTY_MAX_BYTE_COUNT = QLatin1String("maxByteCount");
    const QString PROPERTY_TERMINATED_BY = QLatin1String("terminatedBy");
    const QString PROPERTY_ENCODING = QLatin1String("encoding");
    //primitive
    const QString PROPERTY_VALUE = QLatin1String("value");
    //pointer
    const QString PROPERTY_TARGET = QLatin1String("target");
    //tagged union
    const QString PROPERTY_ALTERNATIVES = QLatin1String("alternatives");
    const QString PROPERTY_DEFAULT_CHILDREN = QLatin1String("defaultFields");
    const QString PROPERTY_SELECT_IF = QLatin1String("selectIf");
    const QString PROPERTY_STRUCT_NAME = QLatin1String("structName");

    const QString PROPERTY_INTERNAL_TYPE = QLatin1String("__type");

    const QStringList ALL_PROPERTIES = QStringList()
            << PROPERTY_ABLE_TO_READ << PROPERTY_ALTERNATIVES << PROPERTY_BYTEORDER
            << PROPERTY_BYTE_COUNT << PROPERTY_CHAR_COUNT << PROPERTY_CHAR_COUNT << PROPERTY_CHILD
            << PROPERTY_CHILDREN << PROPERTY_CHILD_COUNT << PROPERTY_DATATYPE << PROPERTY_DEFAULT_CHILDREN
            << PROPERTY_ENCODING << PROPERTY_ENUM_NAME << PROPERTY_ENUM_VALUES << PROPERTY_INTERNAL_TYPE <<
            PROPERTY_LENGTH << PROPERTY_MAX_BYTE_COUNT << PROPERTY_MAX_CHAR_COUNT << PROPERTY_NAME
            << PROPERTY_PARENT << PROPERTY_SELECT_IF << PROPERTY_STRUCT_NAME << PROPERTY_TARGET
            << PROPERTY_TERMINATED_BY << PROPERTY_TYPE << PROPERTY_UPDATE_FUNC << PROPERTY_VALID <<
            PROPERTY_VALIDATION_ERROR << PROPERTY_VALIDATION_FUNC << PROPERTY_VALUE << PROPERTY_WIDTH;

    const QString NAME_POINTER_VALUE_TYPE = QLatin1String("<pointer value type>");
    const QString NAME_POINTER_TARGET = QLatin1String("<pointer target>");

}

namespace ParserUtils
{
    /** If string starts with 0x, the remainder is interpreted as a hexadecimal (unsigned) number
     * otherwise it will be parsed as a decimal number
     * @param str the string to convert
     * @return a parsed number (check the isValid member to see if conversion succeeded)
     */
    ParsedNumber<int> intFromString(const QString& str);
    /** @see ParserUtils::intFromString() */
    ParsedNumber<uint> uintFromString(const QString& str);
    /** @see ParserUtils::intFromString() */
    ParsedNumber<quint64> uint64FromString(const QString& str);
    /** Checks whether the value is a number, and if it is converts it.
     * Since all script values use double internally, a valid number can be out of bounds, too
     * @param val the value to convert
     * @see ParserUtils::intFromString()
     */
    ParsedNumber<int> intFromScriptValue(const QScriptValue& val);
    /** @see ParserUtils::intFromScriptValue() */
    ParsedNumber<uint> uintFromScriptValue(const QScriptValue& val);
    /** @see ParserUtils::intFromScriptValue() */
    ParsedNumber<quint64> uint64FromScriptValue(const QScriptValue& val);

    DataInformation::DataInformationEndianess byteOrderFromString(const QString& string, const LoggerWithContext& logger);
    QString byteOrderToString(DataInformation::DataInformationEndianess order);

    StringDataInformation::StringType toStringEncoding(const QString& str, const LoggerWithContext& logger);

}

#endif /* PARSERUTILS_H_ */
