/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
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

#ifndef SCRIPTVALUECONVERTER_H_
#define SCRIPTVALUECONVERTER_H_

#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>
#include <QtCore/QString>

class DataInformation;
class AbstractArrayDataInformation;
class PrimitiveDataInformation;
class AbstractBitfieldDataInformation;
class StructureDataInformation;
class UnionDataInformation;
class EnumDataInformation;

class ScriptValueConverter
{
public:
    ScriptValueConverter(QScriptValue& value, QString name);
    virtual ~ScriptValueConverter();
    DataInformation* convert();
private:
    QScriptValue& mValue;
    const QString mName;
private:
    DataInformation
            * toDataInformation(QScriptValue& value, QString name) const;

    AbstractArrayDataInformation* toArray(QScriptValue& value, QString& name) const;
    AbstractBitfieldDataInformation
    * toBitfield(QScriptValue& value, QString& name) const;
    PrimitiveDataInformation* toPrimitive(QScriptValue& value, QString& name) const;
    StructureDataInformation* toStruct(QScriptValue& value, QString& name) const;
    UnionDataInformation* toUnion(QScriptValue& value, QString& name) const;

    //    EnumDataInformation* toEnum(QScriptValue& value, QString name) const;
};

#endif /* SCRIPTVALUECONVERTER_H_ */
