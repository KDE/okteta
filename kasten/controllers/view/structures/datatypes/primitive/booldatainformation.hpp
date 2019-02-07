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

#ifndef KASTEN_BOOLDATAINFORMATION_HPP
#define KASTEN_BOOLDATAINFORMATION_HPP

#include <QScriptValue>

#include "uintdatainformation.hpp"

template <typename T>
class BoolDataInformationMethods
{
public:
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    static QString staticValueString(T val, int base = Kasten::StructureViewPreferences::unsignedDisplayBase());
    static PrimitiveDataType staticType();
    static T fromVariant(const QVariant& value, bool* ok);

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);
};

template <>
inline PrimitiveDataType BoolDataInformationMethods<quint8>::staticType()
{
    return PrimitiveDataType::Bool8;
}
template <>
inline PrimitiveDataType BoolDataInformationMethods<quint16>::staticType()
{
    return PrimitiveDataType::Bool16;
}
template <>
inline PrimitiveDataType BoolDataInformationMethods<quint32>::staticType()
{
    return PrimitiveDataType::Bool32;
}
template <>
inline PrimitiveDataType BoolDataInformationMethods<quint64>::staticType()
{
    return PrimitiveDataType::Bool64;
}

template <typename T>
inline QScriptValue BoolDataInformationMethods<T>::asScriptValue(T value, QScriptEngine* engine,
                                                                 ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return {(value != 0)};
}

template <typename T>
inline T BoolDataInformationMethods<T>::fromVariant(const QVariant& value, bool* ok)
{
    return UIntDataInformationMethods<T>::fromVariant(value, ok);
}

#endif // KASTEN_BOOLDATAINFORMATION_HPP
