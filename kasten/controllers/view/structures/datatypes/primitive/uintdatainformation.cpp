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
#include "uintdatainformation.hpp"

#include "../../structlogging.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QLocale>

#include "../../../poddecoder/typeeditors/uintspinbox.hpp"
#include "structureviewpreferences.hpp"

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
