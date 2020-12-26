/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2016 Aaron Bishop <erroneous@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "parserutils.hpp"

#include <QScriptValue>
#include <QScriptEngine>

namespace ParserStrings {

QString TYPE_ARRAY() { return QStringLiteral("array"); }
QString TYPE_BITFIELD() { return QStringLiteral("bitfield"); }
QString TYPE_ENUM() { return QStringLiteral("enum"); }
QString TYPE_FLAGS() { return QStringLiteral("flags"); }
QString TYPE_PRIMITIVE() { return QStringLiteral("primitive"); }
QString TYPE_STRING() { return QStringLiteral("string"); }
QString TYPE_STRUCT() { return QStringLiteral("struct"); }
QString TYPE_UNION() { return QStringLiteral("union"); }
QString TYPE_POINTER() { return QStringLiteral("pointer"); }
QString TYPE_TAGGED_UNION() { return QStringLiteral("taggedUnion"); }

QString TYPE_ENUMDEF() { return QStringLiteral("enumDef"); }
QString TYPE_ALTERNATIVES() { return QStringLiteral("alternatives"); }
QString TYPE_GROUP() { return QStringLiteral("group"); }

QString PROPERTY_DEFAULT_LOCK_OFFSET() { return QStringLiteral("defaultLockOffset"); }

QString PROPERTY_NAME() { return QStringLiteral("name"); }
QString PROPERTY_BYTEORDER() { return QStringLiteral("byteOrder"); }
QString PROPERTY_PARENT() { return QStringLiteral("parent"); }
QString PROPERTY_VALIDATION_ERROR() { return QStringLiteral("validationError"); }
QString PROPERTY_VALID() { return QStringLiteral("valid"); }
QString PROPERTY_ABLE_TO_READ() { return QStringLiteral("wasAbleToRead"); }
QString PROPERTY_UPDATE_FUNC() { return QStringLiteral("updateFunc"); }
QString PROPERTY_VALIDATION_FUNC() { return QStringLiteral("validationFunc"); }
QString PROPERTY_TO_STRING_FUNC() { return QStringLiteral("toStringFunc"); }
QString PROPERTY_DATATYPE() { return QStringLiteral("datatype"); }
QString PROPERTY_CUSTOM_TYPE_NAME() { return QStringLiteral("typeName"); }

QString PROPERTY_ENUM_VALUES() { return QStringLiteral("enumValues"); }

QString PROPERTY_ENUM_NAME() { return QStringLiteral("enumName"); }

QString PROPERTY_TYPE() { return QStringLiteral("type"); }

QString PROPERTY_LENGTH() { return QStringLiteral("length"); }

QString PROPERTY_WIDTH() { return QStringLiteral("width"); }
QString PROPERTY_CHILDREN() { return QStringLiteral("fields"); }
QString PROPERTY_CHILD_COUNT() { return QStringLiteral("childCount"); }
QString PROPERTY_CHILD() { return QStringLiteral("child"); }

QString PROPERTY_CHAR_COUNT() { return QStringLiteral("charCount"); }
QString PROPERTY_BYTE_COUNT() { return QStringLiteral("byteCount"); }
QString PROPERTY_MAX_CHAR_COUNT() { return QStringLiteral("maxCharCount"); }
QString PROPERTY_MAX_BYTE_COUNT() { return QStringLiteral("maxByteCount"); }
QString PROPERTY_TERMINATED_BY() { return QStringLiteral("terminatedBy"); }
QString PROPERTY_ENCODING() { return QStringLiteral("encoding"); }
// primitive
QString PROPERTY_VALUE() { return QStringLiteral("value"); }
// pointer
QString PROPERTY_TARGET() { return QStringLiteral("target"); }
QString PROPERTY_SCALE() { return QStringLiteral("scale"); }
QString PROPERTY_INTERPRET_FUNC() { return QStringLiteral("interpretFunc"); }
// tagged union
QString PROPERTY_ALTERNATIVES() { return QStringLiteral("alternatives"); }
QString PROPERTY_DEFAULT_CHILDREN() { return QStringLiteral("defaultFields"); }
QString PROPERTY_SELECT_IF() { return QStringLiteral("selectIf"); }
QString PROPERTY_STRUCT_NAME() { return QStringLiteral("structName"); }

QString PROPERTY_INTERNAL_TYPE() { return QStringLiteral("__type"); }

QStringList ALL_PROPERTIES()
{
    return QStringList {
        PROPERTY_ABLE_TO_READ(),
        PROPERTY_ALTERNATIVES(),
        PROPERTY_BYTEORDER(),
        PROPERTY_BYTE_COUNT(),
        PROPERTY_CHAR_COUNT(),
        PROPERTY_CHAR_COUNT(),
        PROPERTY_CHILD(),
        PROPERTY_CHILDREN(),
        PROPERTY_CHILD_COUNT(),
        PROPERTY_DATATYPE(),
        PROPERTY_DEFAULT_CHILDREN(),
        PROPERTY_ENCODING(),
        PROPERTY_ENUM_NAME(),
        PROPERTY_ENUM_VALUES(),
        PROPERTY_INTERNAL_TYPE(),
        PROPERTY_INTERPRET_FUNC(),
        PROPERTY_LENGTH(),
        PROPERTY_MAX_BYTE_COUNT(),
        PROPERTY_MAX_CHAR_COUNT(),
        PROPERTY_NAME(),
        PROPERTY_PARENT(),
        PROPERTY_SCALE(),
        PROPERTY_SELECT_IF(),
        PROPERTY_STRUCT_NAME(),
        PROPERTY_TARGET(),
        PROPERTY_TERMINATED_BY(),
        PROPERTY_TYPE(),
        PROPERTY_UPDATE_FUNC(),
        PROPERTY_VALID(),
        PROPERTY_VALIDATION_ERROR(),
        PROPERTY_VALIDATION_FUNC(),
        PROPERTY_VALUE(),
        PROPERTY_WIDTH()
    };
}

QString NAME_POINTER_VALUE_TYPE() { return QStringLiteral("<pointer value type>"); }
QString NAME_POINTER_TARGET() { return QStringLiteral("<pointer target>"); }
QString NAME_ARRAY_TYPE() { return QStringLiteral("<array type>"); }
}

ParsedNumber<int> ParserUtils::intFromString(const QString& str)
{
    int value = 0;
    bool okay = false;
    if (str.startsWith(QLatin1String("0x"))) {
        value = str.midRef(2).toInt(&okay, 16);
    } else if (str.startsWith(QLatin1String("-0x"))) {
        // special case for minimum possible value
        if (str == QLatin1String("-0x80000000")) {
            return ParsedNumber<int>(-0x80000000, str, true);
        }
        value = -str.midRef(3).toInt(&okay, 16);
    } else {
        value = str.toInt(&okay, 10);
    }
    return ParsedNumber<int>(value, str, okay);
}

ParsedNumber<uint> ParserUtils::uintFromString(const QString& str)
{
    uint value = 0;
    bool okay;
    if (str.startsWith(QLatin1String("0x"))) {
        value = str.midRef(2).toUInt(&okay, 16);
    } else {
        value = str.toUInt(&okay, 10);
    }
    return ParsedNumber<uint>(value, str, okay);
}

ParsedNumber<quint64> ParserUtils::uint64FromString(const QString& str)
{
    quint64 value = 0;
    bool okay;
    if (str.startsWith(QLatin1String("0x"))) {
        value = str.midRef(2).toULongLong(&okay, 16);
    } else {
        value = str.toULongLong(&okay, 10);
    }
    return ParsedNumber<quint64>(value, str, okay);
}

DataInformation::DataInformationEndianess ParserUtils::byteOrderFromString(const QString& string,
                                                                           const LoggerWithContext& logger)
{
    const QString lower = string.toLower();
    if (lower == QLatin1String("bigendian") || lower == QLatin1String("big-endian")) {
        return DataInformation::DataInformationEndianess::EndianessBig;
    }
    if (lower == QLatin1String("littleendian") || lower == QLatin1String("little-endian")) {
        return DataInformation::DataInformationEndianess::EndianessLittle;
    }
    if (lower == QLatin1String("fromsettings") || lower == QLatin1String("from-settings")) {
        return DataInformation::DataInformationEndianess::EndianessFromSettings;
    }
    if (lower == QLatin1String("inherit")) {
        return DataInformation::DataInformationEndianess::EndianessInherit;
    }

    logger.warn().nospace() << "Unrecognized byte order '" << string << "', defaulting to 'inherit'";
    return DataInformation::DataInformationEndianess::EndianessInherit;
}

ParsedNumber<int> ParserUtils::intFromScriptValue(const QScriptValue& val)
{
    if (val.isNumber()) {
        // check whether it is in range
        const qsreal doubleVal = val.toNumber();
        const int value = val.toInt32();
        if (doubleVal != qsreal(value)) {
            return ParsedNumber<int>::badInput(val.toString());
        }
        return ParsedNumber<int>(value, val.toString(), true);
    }
    if (val.isString()) {
        return intFromString(val.toString());
    }
    return ParsedNumber<int>::badInput(val.toString());
}

ParsedNumber<uint> ParserUtils::uintFromScriptValue(const QScriptValue& val)
{
    if (val.isNumber()) {
        // check whether it is in range
        const uint value = val.toUInt32();
        const qsreal doubleVal = val.toNumber();
        if (doubleVal != qsreal(value)) {
            return ParsedNumber<uint>::badInput(val.toString());
        }
        return ParsedNumber<uint>(value, val.toString(), true);
    }
    if (val.isString()) {
        return uintFromString(val.toString());
    }
    return ParsedNumber<uint>::badInput(val.toString());
}

ParsedNumber<quint64> ParserUtils::uint64FromScriptValue(const QScriptValue& val)
{
    if (val.isNumber()) {
        // check whether it is in range
        const uint value = val.toUInt32();
        const qsreal doubleVal = val.toNumber();
        if (doubleVal != qsreal(value)) {
            return ParsedNumber<quint64>::badInput(val.toString());
        }
        return ParsedNumber<quint64>(value, val.toString(), true);
    }
    if (val.isString()) {
        return uint64FromString(val.toString());
    }
    return ParsedNumber<quint64>::badInput(val.toString());
}

QString ParserUtils::byteOrderToString(DataInformation::DataInformationEndianess order)
{
    if (order == DataInformation::DataInformationEndianess::EndianessLittle) {
        return QStringLiteral("littleEndian");
    }
    if (order == DataInformation::DataInformationEndianess::EndianessBig) {
        return QStringLiteral("bigEndian");
    }
    if (order == DataInformation::DataInformationEndianess::EndianessFromSettings) {
        return QStringLiteral("fromSettings");
    }
    return QStringLiteral("inherit");
}

StringDataInformation::StringType ParserUtils::toStringEncoding(const QString& str, const LoggerWithContext& logger)
{
    QString enc = str.toLower();
    if (enc == QLatin1String("ascii")) {
        return StringDataInformation::StringType::ASCII;
    }
    if (enc == QLatin1String("ebcdic")) {
        return StringDataInformation::StringType::EBCDIC;
    }
    if (enc == QLatin1String("latin1") || enc == QLatin1String("latin-1")) {
        return StringDataInformation::StringType::Latin1;
    }
    if (enc.startsWith(QLatin1String("utf"))) {
        QStringRef ref = enc.midRef(3);
        if (ref.at(0) == QLatin1Char('-')) {
            ref = enc.midRef(4); // strip '-'
        }
        if (ref == QLatin1String("8")) {
            return StringDataInformation::StringType::UTF8;
        }

        if (ref == QLatin1String("16") || ref == QLatin1String("16le") || ref == QLatin1String("16-le")) {
            return StringDataInformation::StringType::UTF16_LE;
        }
        if (ref == QLatin1String("16be") || ref == QLatin1String("16-be")) {
            return StringDataInformation::StringType::UTF16_BE;
        }
        if (ref == QLatin1String("32") || ref == QLatin1String("32le") || ref == QLatin1String("32-le")) {
            return StringDataInformation::StringType::UTF32_LE;
        }
        if (ref == QLatin1String("32be") || ref == QLatin1String("32-be")) {
            return StringDataInformation::StringType::UTF32_BE;
        }
    }
    logger.warn() << "Unrecognized string encoding: " << enc;
    return StringDataInformation::StringType::InvalidEncoding;
}

QScriptValue ParserUtils::functionSafeEval(QScriptEngine* engine, const QString& str)
{
    if (str.isEmpty()) {
        return {};
    }
    // must wrap in parentheses, see https://bugreports.qt-project.org/browse/QTBUG-5757
    QScriptValue ret = engine->evaluate(QLatin1Char('(') + str + QLatin1Char(')'));
    if (!ret.isFunction()) {
        return {str};
    }
    return ret;
}
