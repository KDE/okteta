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
#ifndef SCRIPTVALUECONVERTER_P_H_
#define SCRIPTVALUECONVERTER_P_H_

class ScriptLogger;
class DataInformation;
class ArrayDataInformation;
class PrimitiveDataInformation;
class AbstractBitfieldDataInformation;
class StructureDataInformation;
class UnionDataInformation;
class AbstractEnumDataInformation;
class StringDataInformation;

class QScriptValue;
class QString;

namespace ScriptValueConverter
{

DataInformation* toDataInformation(const QScriptValue& value, const QString& name,
        ScriptLogger* logger = 0);

ArrayDataInformation* toArray(const QScriptValue& value, const QString& name,
        ScriptLogger* logger = 0);

AbstractBitfieldDataInformation* toBitfield(const QScriptValue& value, const QString& name,
        ScriptLogger* logger = 0);

PrimitiveDataInformation* toPrimitive(const QScriptValue& value, const QString& name,
        ScriptLogger* logger = 0);

StructureDataInformation* toStruct(const QScriptValue& value, const QString& name,
        ScriptLogger* logger = 0);

UnionDataInformation* toUnion(const QScriptValue& value, const QString& name,
        ScriptLogger* logger = 0);

StringDataInformation* toString(const QScriptValue& value, const QString& name,
        ScriptLogger* logger = 0);

AbstractEnumDataInformation* toEnum(const QScriptValue& value, const QString& name, bool isFlags,
        ScriptLogger* logger = 0);

}

#endif //SCRIPTVALUECONVERTER_P_H_

