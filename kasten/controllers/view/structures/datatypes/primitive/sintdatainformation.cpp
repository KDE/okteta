/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "sintdatainformation.hpp"
#include "../../structlogging.hpp"
#include <view/poddecoder/typeeditors/sintspinbox.hpp>

// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QLocale>

template <typename T>
QString SIntDataInformationMethods<T>::staticValueString(T val, int base)
{
    QString num;
    if (base == 10) {
        return Kasten::StructureViewPreferences::localeAwareDecimalFormatting()
               ? QLocale().toString(val)
               : QString::number(val, base);
    }
    // the absolute value of negative minimum can not be represented as a signed integer
    // casting it to an unsigned value yields the correct result
    if (val == std::numeric_limits<T>::min()) {
        num = QString::number(typename QIntegerForSizeof<T>::Unsigned(val), base);
    } else if (val < 0) {
        num = QString::number(qAbs(val), base);
    } else {
        num = QString::number(val, base);
    }
    // TODO non decimal negative values as unsigned? probably add option
    // add one space every 8 chars
    for (int i = 8; i < num.length(); i += 9) {
        num.insert(num.length() - i, QLatin1Char(' '));
    }

    if (val < 0) {
        return QLatin1Char('-') + PrimitiveDataInformation::basePrefix(base) + num;
    }

    return PrimitiveDataInformation::basePrefix(base) + num;
}

template <typename T>
QScriptValue SIntDataInformationMethods<T>::asScriptValue(T value, QScriptEngine* engine,
                                                          ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return {value};
}

template <>
QScriptValue SIntDataInformationMethods<qint64>::asScriptValue(qint64 value, QScriptEngine* engine,
                                                               ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return {QString::number(value, 10)};
}

template <typename T>
inline QWidget* SIntDataInformationMethods<T>::staticCreateEditWidget(QWidget* parent)
{
    auto* ret = new SIntSpinBox(parent, Kasten::StructureViewPreferences::signedDisplayBase());
    ret->setRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return ret;
}

template <typename T>
inline QVariant SIntDataInformationMethods<T>::staticDataFromWidget(const QWidget* w)
{
    const auto* spin = qobject_cast<const SIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin) {
        return spin->value();
    }

    qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast widget";
    return {};
}

template <typename T>
inline void SIntDataInformationMethods<T>::staticSetWidgetData(T value, QWidget* w)
{
    auto* spin = qobject_cast<SIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin) {
        spin->setValue(value);
    }
}

// explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class SIntDataInformationMethods<qint8>;
template class SIntDataInformationMethods<qint16>;
template class SIntDataInformationMethods<qint32>;
template class SIntDataInformationMethods<qint64>;
