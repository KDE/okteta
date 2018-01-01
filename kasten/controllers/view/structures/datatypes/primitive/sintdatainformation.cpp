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
#include "sintdatainformation.h"
#include "../poddecoder/typeeditors/sintspinbox.h"
#include "../../structlogging.h"

// KF5
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QLocale>

template<typename T>
QString SIntDataInformationMethods<T>::staticValueString(T val, int base)
{
    QString num;
    if (base == 10)
    {
        return Kasten::StructureViewPreferences::localeAwareDecimalFormatting()
            ? QLocale().toString(val)
            : QString::number(val, base);
    }
    //the absolute value of negative minimum can not be represented as a signed integer
    //casting it to an unsigned value yields the correct result
    if (val == std::numeric_limits<T>::min())
        num = QString::number(typename QIntegerForSizeof<T>::Unsigned(val), base);
    else if (val < 0)
        num = QString::number(qAbs(val), base);
    else
        num = QString::number(val, base);
    //TODO non decimal negative values as unsigned? probably add option
    //add one space every 8 chars
    for (int i = 8; i < num.length(); i += 9)
    {
        num.insert(num.length() - i, QLatin1Char(' '));
    }
    if (val < 0)
        return QLatin1Char('-') + PrimitiveDataInformation::basePrefix(base) + num;
    else
        return PrimitiveDataInformation::basePrefix(base) + num;
}

template<typename T>
QScriptValue SIntDataInformationMethods<T>::asScriptValue(T value, QScriptEngine* engine,
        ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return QScriptValue(value);
}

template<>
QScriptValue SIntDataInformationMethods<qint64>::asScriptValue(qint64 value, QScriptEngine* engine,
        ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return QScriptValue(QString::number(value, 10));
}

template<typename T>
inline QWidget* SIntDataInformationMethods<T>::staticCreateEditWidget(QWidget* parent)
{
    SIntSpinBox* ret = new SIntSpinBox(parent, Kasten::StructureViewPreferences::signedDisplayBase());
    ret->setRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return ret;
}

template<typename T>
inline QVariant SIntDataInformationMethods<T>::staticDataFromWidget(const QWidget* w)
{
    const SIntSpinBox* spin = qobject_cast<const SIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin)
        return spin->value();

    qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast widget";
    return QVariant();
}

template<typename T>
inline void SIntDataInformationMethods<T>::staticSetWidgetData(T value, QWidget* w)
{
    SIntSpinBox* spin = qobject_cast<SIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin)
        spin->setValue(value);
}

//explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class SIntDataInformationMethods<qint8>;
template class SIntDataInformationMethods<qint16>;
template class SIntDataInformationMethods<qint32>;
template class SIntDataInformationMethods<qint64>;
