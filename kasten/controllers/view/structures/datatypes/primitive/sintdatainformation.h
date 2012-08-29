/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011  Alex Richardson <alex.richardson@gmx.de>
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
#ifndef SINTDATAINFORMATION_H
#define SINTDATAINFORMATION_H

#include "primitivedatainformation.h"

template<typename T>
class SIntDataInformationMethods
{
public:
    static PrimitiveDataType staticType();
    static T fromVariant(const QVariant& value, bool* ok);
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    static QString staticValueString(T val, int base = PrimitiveDataInformation::signedDisplayBase());

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);
};

template<>
inline PrimitiveDataType SIntDataInformationMethods<qint8>::staticType()
{
    return Type_Int8;
}
template<>
inline PrimitiveDataType SIntDataInformationMethods<qint16>::staticType()
{
    return Type_Int16;
}
template<>
inline PrimitiveDataType SIntDataInformationMethods<qint32>::staticType()
{
    return Type_Int32;
}
template<>
inline PrimitiveDataType SIntDataInformationMethods<qint64>::staticType()
{
    return Type_Int64;
}

template<typename T>
inline T SIntDataInformationMethods<T>::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    qint64 val = value.toLongLong(ok);
    T result = T(val);
    if (val != qint64(result))
        *ok = false;
    return result;
}

#endif // SINTDATAINFORMATION_H
