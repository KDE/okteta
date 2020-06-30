/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
