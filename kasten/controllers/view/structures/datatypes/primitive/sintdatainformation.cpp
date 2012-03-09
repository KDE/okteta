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

#include <KGlobal>

template<typename T, PrimitiveDataType typeValue>
QString SIntDataInformation<T, typeValue>::valueString() const
{
    if (!mWasAbleToRead)
        return i18nc("invalid value (out of range)", "<invalid>");
    return valueString(mValue);
}

template<typename T, PrimitiveDataType typeValue>
QString SIntDataInformation<T, typeValue>::valueString(T val)
{
    int base = displayBase();
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

template<typename T, PrimitiveDataType typeValue>
QScriptValue SIntDataInformation<T, typeValue>::valueAsQScriptValue() const
{
    return asScriptValue(mValue, 0, 0);
}

template<typename T, PrimitiveDataType typeValue>
QScriptValue SIntDataInformation<T, typeValue>::asScriptValue(T value, QScriptEngine* engine,
        ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return QScriptValue(value);
}

template<>
QScriptValue SIntDataInformation<qint64, Type_Int64>::asScriptValue(qint64 value, QScriptEngine* engine,
        ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return QScriptValue(QString::number(value, 10));
}

template<typename T, PrimitiveDataType typeValue>
inline QWidget* SIntDataInformation<T, typeValue>::createEditWidget(QWidget* parent) const
{
    return staticCreateEditWidget(parent);
}

template<typename T, PrimitiveDataType typeValue>
QVariant SIntDataInformation<T, typeValue>::dataFromWidget(const QWidget* w) const
{
    return staticDataFromWidget(w);
}

template<typename T, PrimitiveDataType typeValue>
inline void SIntDataInformation<T, typeValue>::setWidgetData(QWidget* w) const
{
    staticSetWidgetData(mValue, w);
}

//explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class SIntDataInformation<qint8, Type_Int8>;
template class SIntDataInformation<qint16, Type_Int16>;
template class SIntDataInformation<qint32, Type_Int32>;
template class SIntDataInformation<qint64, Type_Int64>;
