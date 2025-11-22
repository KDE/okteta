/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UINTDATAINFORMATION_HPP
#define KASTEN_UINTDATAINFORMATION_HPP

// lib
#include "primitivedatainformation.hpp"
#include <structureviewpreferences.hpp>
#include <allprimitivetypes.hpp>

template <typename T>
class UIntDataInformationMethods
{
public:
    [[nodiscard]]
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    [[nodiscard]]
    static QString staticValueString(T val, int base = Kasten::StructureViewPreferences::unsignedDisplayBase());
    [[nodiscard]]
    static PrimitiveDataType staticType();
    [[nodiscard]]
    static T fromVariant(const QVariant& value, bool* ok);
    [[nodiscard]]
    static QVariant staticToQVariant(T value);

    [[nodiscard]]
    static QWidget* staticCreateEditWidget(QWidget* parent);
    [[nodiscard]]
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);
};

template <>
inline PrimitiveDataType UIntDataInformationMethods<quint8>::staticType() { return PrimitiveDataType::UInt8; }
template <>
inline PrimitiveDataType UIntDataInformationMethods<quint16>::staticType() { return PrimitiveDataType::UInt16; }
template <>
inline PrimitiveDataType UIntDataInformationMethods<quint32>::staticType() { return PrimitiveDataType::UInt32; }
template <>
inline PrimitiveDataType UIntDataInformationMethods<quint64>::staticType() { return PrimitiveDataType::UInt64; }

template <typename T>
inline QVariant UIntDataInformationMethods<T>::staticToQVariant(T value)
{
    return QVariant(value);
}

template <typename T>
inline T UIntDataInformationMethods<T>::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    quint64 val = value.toULongLong(ok);
    T result = T(val);
    if (val != quint64(result)) {
        *ok = false;
    }
    return result;
}

#endif // KASTEN_UINTDATAINFORMATION_HPP
