/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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


class QString;
class QScriptValue;
class ScriptLogger;
template<typename T> class QVector;
class DataInformation;

namespace ScriptValueConverter
{

    //FIXME remove the = 0
    /** If the value is one element */
    DataInformation* convert(const QScriptValue& value, const QString& name, ScriptLogger* logger,
            DataInformation* parent = 0);
    /** If the value is a list of elements or an object with many elements */
    QVector<DataInformation*> convertValues(const QScriptValue& value, ScriptLogger* logger,
            DataInformation* parent = 0);
}

#endif /* SCRIPTVALUECONVERTER_H_ */
