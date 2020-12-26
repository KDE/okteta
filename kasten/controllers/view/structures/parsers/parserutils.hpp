/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PARSERUTILS_HPP
#define KASTEN_PARSERUTILS_HPP

#include <QDebug>
#include <QStringList>
#include <KLocalizedString>

#include "../datatypes/datainformation.hpp"
#include "../datatypes/strings/stringdatainformation.hpp"
#include "../script/scriptlogger.hpp"

// forward declare so I don't have to repeat in all the parsers
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
class QScriptEngine;

/** For use by the parsers so that the functions don't have as many parameters */
struct ParserInfo
{
    inline ParserInfo(const QString& name, ScriptLogger* logger, DataInformation* parent, QScriptEngine* engine)
        : name(name)
        , logger(logger)
        , parent(parent)
        , engine(engine)
    {
        Q_CHECK_PTR(logger);
    }
    inline ParserInfo(const ParserInfo& i) = default;
    inline ~ParserInfo() = default;

    QString name;
    ScriptLogger* logger;
    DataInformation* parent;
    QScriptEngine* engine;

    inline QString context() const
    {
        return parent ? parent->fullObjectPath() + QLatin1Char('.') + name : name;
    }
    inline QDebug info() const { return logger->info(context()); }
    inline QDebug warn() const { return logger->warn(context()); }
    inline QDebug error() const { return logger->error(context()); }
};

class ChildrenParser
{
public:
    virtual DataInformation* next() = 0;
    virtual bool hasNext() = 0;
    virtual void setParent(DataInformation* parent) = 0;
    virtual ~ChildrenParser() = default;
};

/**
 * Holds a number that was converted either from a QScriptValue or a QString
 */
template <typename T>
struct ParsedNumber
{
    ParsedNumber() = default;
    ParsedNumber(T val, const QString& str, bool ok)
        : string(str)
        , value(val)
        , isValid(ok)
    {}

    QString string;
    T value = 0;
    bool isValid = false;

    static ParsedNumber<T> badInput(const QString& str) { return ParsedNumber<T>(T(), str, false); }
    inline bool isError() { return !isValid && !string.isEmpty(); }
    inline bool isEmpty() { Q_ASSERT(!isValid); return string.isEmpty(); }
};

namespace ParserStrings {

QString TYPE_ARRAY();
QString TYPE_BITFIELD();
QString TYPE_ENUM();
QString TYPE_FLAGS();
QString TYPE_PRIMITIVE();
QString TYPE_STRING();
QString TYPE_STRUCT();
QString TYPE_UNION();
QString TYPE_POINTER();
QString TYPE_TAGGED_UNION();
/** Only needed for .osd */
QString TYPE_ENUMDEF();
QString TYPE_ALTERNATIVES();
QString TYPE_GROUP();

// lock offset
QString PROPERTY_DEFAULT_LOCK_OFFSET();
// all types
QString PROPERTY_NAME();
QString PROPERTY_BYTEORDER();
QString PROPERTY_PARENT();
QString PROPERTY_VALIDATION_ERROR();
QString PROPERTY_VALID();
QString PROPERTY_ABLE_TO_READ();
QString PROPERTY_UPDATE_FUNC();
QString PROPERTY_VALIDATION_FUNC();
QString PROPERTY_TO_STRING_FUNC();
QString PROPERTY_DATATYPE();
QString PROPERTY_CUSTOM_TYPE_NAME();
// enum
QString PROPERTY_ENUM_VALUES();
// this one is javascript only
QString PROPERTY_ENUM_NAME();
// array/bitfield
QString PROPERTY_TYPE();
// array
QString PROPERTY_LENGTH();
// bitfield
QString PROPERTY_WIDTH();
// struct/union
QString PROPERTY_CHILDREN();
QString PROPERTY_CHILD_COUNT();
QString PROPERTY_CHILD();
// strings
QString PROPERTY_CHAR_COUNT();
QString PROPERTY_BYTE_COUNT();
QString PROPERTY_MAX_CHAR_COUNT();
QString PROPERTY_MAX_BYTE_COUNT();
QString PROPERTY_TERMINATED_BY();
QString PROPERTY_ENCODING();
// primitive
QString PROPERTY_VALUE();
// pointer
QString PROPERTY_TARGET();
QString PROPERTY_SCALE();
QString PROPERTY_INTERPRET_FUNC();
// tagged union
QString PROPERTY_ALTERNATIVES();
QString PROPERTY_DEFAULT_CHILDREN();
QString PROPERTY_SELECT_IF();
QString PROPERTY_STRUCT_NAME();

QString PROPERTY_INTERNAL_TYPE();

QStringList ALL_PROPERTIES();

QString NAME_POINTER_VALUE_TYPE();
QString NAME_POINTER_TARGET();
QString NAME_ARRAY_TYPE();

}

namespace ParserUtils {

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

/** This essentially calls engine->evaluate(str), but ensures it can be a function (QTBUG-5757)  */
QScriptValue functionSafeEval(QScriptEngine* engine, const QString& str);

}

#endif /* KASTEN_PARSERUTILS_HPP */
