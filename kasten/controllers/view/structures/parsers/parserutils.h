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
class AbstractEnumDataInformation;
class StringDataInformation;

class QScriptValue;

/** For use by the parsers so that the functions don't have as many parameters */
struct ParserInfo
{
    inline ParserInfo(const QString& name, ScriptLogger* logger, DataInformation* parent)
            : name(name.isEmpty() ? i18n("&lt;no name specified&gt;") : name), logger(logger), parent(parent)
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
    /** Only needed for .osd */
    const QString TYPE_ENUMDEF = QLatin1String("enumDef");


    //all types
    const QString PROPERTY_NAME = QLatin1String("name");
    const QString PROPERTY_BYTEORDER = QLatin1String("byteOrder");
    const QString PROPERTY_PARENT= QLatin1String("parent");
    const QString PROPERTY_VALIDATION_ERROR = QLatin1String("validationError");
    const QString PROPERTY_VALID = QLatin1String("valid");
    const QString PROPERTY_ABLE_TO_READ = QLatin1String("wasAbleToRead");
    const QString PROPERTY_UPDATE_FUNC = QLatin1String("updateFunc");
    const QString PROPERTY_VALIDATION_FUNC = QLatin1String("validationFunc");
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
    const QString PROPERTY_CHILDREN = QLatin1String("children");
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

    const QString PROPERTY_INTERNAL_TYPE = QLatin1String("__type");

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
    /** Checks whether the value is a number, and if it is converts it.
     * Since all script values use double internally, a valid number can be out of bounds, too
     * @param val the value to convert
     * @see ParserUtils::intFromString()
     */
    ParsedNumber<int> intFromScriptValue(const QScriptValue& val);
    /** @see ParserUtils::intFromScriptValue() */
    ParsedNumber<uint> uintFromScriptValue(const QScriptValue& val);

    DataInformation::DataInformationEndianess byteOrderFromString(const QString& string, const ParserInfo& info);
    QString byteOrderToString(DataInformation::DataInformationEndianess order);

    StringDataInformation::StringType toStringEncoding(const QString& str);

}

#endif /* PARSERUTILS_H_ */
