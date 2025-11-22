/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINTDATAINFORMATION_HPP
#define KASTEN_SINTDATAINFORMATION_HPP

// lib
#include "primitivedatainformation.hpp"
#include <structureviewpreferences.hpp>
#include <allprimitivetypes.hpp>

template <typename T>
class SIntDataInformationMethods
{
public:
    [[nodiscard]]
    static PrimitiveDataType staticType();
    [[nodiscard]]
    static T fromVariant(const QVariant& value, bool* ok);
    [[nodiscard]]
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    [[nodiscard]]
    static QString staticValueString(T val, int base = Kasten::StructureViewPreferences::signedDisplayBase());
    [[nodiscard]]
    static QVariant staticToQVariant(T value);

    [[nodiscard]]
    static QWidget* staticCreateEditWidget(QWidget* parent);
    [[nodiscard]]
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
inline QVariant SIntDataInformationMethods<T>::staticToQVariant(T value)
{
    return QVariant(value);
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
