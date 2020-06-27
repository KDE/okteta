/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINTDATAINFORMATION_HPP
#define KASTEN_SINTDATAINFORMATION_HPP

#include "primitivedatainformation.hpp"
#include "structureviewpreferences.h"

template <typename T>
class SIntDataInformationMethods
{
public:
    static PrimitiveDataType staticType();
    static T fromVariant(const QVariant& value, bool* ok);
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    static QString staticValueString(T val, int base = Kasten::StructureViewPreferences::signedDisplayBase());

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);
};

template <>
inline PrimitiveDataType SIntDataInformationMethods<qint8>::staticType()
{
    return PrimitiveDataType::Int8;
}
template <>
inline PrimitiveDataType SIntDataInformationMethods<qint16>::staticType()
{
    return PrimitiveDataType::Int16;
}
template <>
inline PrimitiveDataType SIntDataInformationMethods<qint32>::staticType()
{
    return PrimitiveDataType::Int32;
}
template <>
inline PrimitiveDataType SIntDataInformationMethods<qint64>::staticType()
{
    return PrimitiveDataType::Int64;
}

template <typename T>
inline T SIntDataInformationMethods<T>::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    qint64 val = value.toLongLong(ok);
    T result = T(val);
    if (val != qint64(result)) {
        *ok = false;
    }
    return result;
}

#endif // KASTEN_SINTDATAINFORMATION_HPP
