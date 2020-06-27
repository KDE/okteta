/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uintdatainformation.hpp"

#include "../../structlogging.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QLocale>

#include "../../../poddecoder/typeeditors/uintspinbox.hpp"
#include "structureviewpreferences.h"

template <typename T>
QScriptValue UIntDataInformationMethods<T>::asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    Q_UNUSED(engine);
    Q_UNUSED(handlerInfo);
    return {value};
}

template <>
QScriptValue UIntDataInformationMethods<quint64>::asScriptValue(quint64 value, QScriptEngine* engine,
                                                                ScriptHandlerInfo* handlerInfo)
{
    Q_UNUSED(engine);
    Q_UNUSED(handlerInfo);
    return {QString::number(value, 10)};
}

template <typename T>
QString UIntDataInformationMethods<T>::staticValueString(T value, int base)
{
    QString num = QString::number(value, base);
    if (base == 10) {
        if (Kasten::StructureViewPreferences::localeAwareDecimalFormatting()) {
            num = QLocale().toString(value);
        }
    } else {
        // add one space every 8 chars
        for (int i = 8; i < num.length(); i += 9) {
            num.insert(num.length() - i, QLatin1Char(' '));
        }
    }
    return PrimitiveDataInformation::basePrefix(base) + num;
}

template <typename T>
inline QWidget* UIntDataInformationMethods<T>::staticCreateEditWidget(QWidget* parent)
{
    auto* ret = new UIntSpinBox(parent, Kasten::StructureViewPreferences::unsignedDisplayBase());
    ret->setMaximum(std::numeric_limits<T>::max());
    return ret;
}

template <typename T>
inline QVariant UIntDataInformationMethods<T>::staticDataFromWidget(const QWidget* w)
{
    const auto* spin = qobject_cast<const UIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin) {
        return {spin->value()};
    }
    qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast widget";
    return {};
}

template <typename T>
inline void UIntDataInformationMethods<T>::staticSetWidgetData(T value, QWidget* w)
{
    auto* spin = qobject_cast<UIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin) {
        spin->setValue(value);
    }
}

// explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class UIntDataInformationMethods<quint8>;
template class UIntDataInformationMethods<quint16>;
template class UIntDataInformationMethods<quint32>;
template class UIntDataInformationMethods<quint64>;
