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
};

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
}

#endif /* PARSERUTILS_H_ */
