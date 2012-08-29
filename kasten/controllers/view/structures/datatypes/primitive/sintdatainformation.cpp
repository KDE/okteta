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

#include <QScriptValue>
#include <KGlobal>
#include <KLocale>

template<typename T>
QString SIntDataInformationMethods<T>::staticValueString(T val, int base)
{
    QString num;
    if (base == 10) {
        num = QString::number(val, base);
        if (Kasten2::StructViewPreferences::localeAwareDecimalFormatting())
            num = KGlobal::locale()->formatNumber(num, false, 0);
        return num;
    }
    Q_ASSERT(base != 10); //base 10 must have been handeled by now!
    if (val >= 0)
    {
        //no need to do anything special since value is positive
        num = QString::number(val, base);
        //add one space every 8 chars
        for (int i = 8; i < num.length(); i += 9)
        {
            num.insert(num.length() - i, QLatin1Char(' '));
        }
        if (base == 16)
            num.prepend(QLatin1String("0x"));
        else if (base == 8)
            num.prepend(QLatin1String("0o"));
        else if (base == 2)
        {

            num.prepend(QLatin1String("0b"));
        }
        return num;
    }
    //value is less than zero, now the tricky bit starts
    //TODO non decimal negative values as unsigned? probably add option

    //the absolute value of negative minimum can not be represented as a signed integer
    //casting it to an unsigned value yields the correct result
    if (val == std::numeric_limits<T>::min())
        num = QString::number(typename QIntegerForSizeof<T>::Unsigned(val), base);
    else
        num = QString::number(qAbs(val), base);

    //add one space every 8 chars
    for (int i = 8; i < num.length(); i += 9)
    {
        num.insert(num.length() - i, QLatin1Char(' '));
    }
    if (base == 16)
        num.prepend(QLatin1String("-0x"));
    else if (base == 8)
        num.prepend(QLatin1String("-0o"));
    else if (base == 2)
        num.prepend(QLatin1String("-0b"));
    else
    {
        num.prepend(QLatin1String("-"));
        kDebug() << "unsupported number base" << base;
    }
    return num;
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
    SIntSpinBox* ret = new SIntSpinBox(parent, PrimitiveDataInformation::signedDisplayBase());
    ret->setRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return ret;
}

template<typename T>
inline QVariant SIntDataInformationMethods<T>::staticDataFromWidget(const QWidget* w)
{
    const SIntSpinBox* spin = dynamic_cast<const SIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin)
        return spin->value();

    kWarning() << "could not cast widget";
    return QVariant();
}

template<typename T>
inline void SIntDataInformationMethods<T>::staticSetWidgetData(T value, QWidget* w)
{
    SIntSpinBox* spin = dynamic_cast<SIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin)
        spin->setValue(value);
}

//explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class SIntDataInformationMethods<qint8>;
template class SIntDataInformationMethods<qint16>;
template class SIntDataInformationMethods<qint32>;
template class SIntDataInformationMethods<qint64>;
