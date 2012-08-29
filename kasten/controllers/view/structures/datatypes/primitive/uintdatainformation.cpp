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
#include "uintdatainformation.h"

#include <QScriptValue>

#include <KGlobal>
#include <KLocale>

#include "../../../poddecoder/typeeditors/uintspinbox.h"


template<typename T>
QScriptValue UIntDataInformationMethods<T>::asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    Q_UNUSED(engine);
    Q_UNUSED(handlerInfo);
    return QScriptValue(value);
}

template<>
QScriptValue UIntDataInformationMethods<quint64>::asScriptValue(quint64 value, QScriptEngine* engine,
        ScriptHandlerInfo* handlerInfo)
{
    Q_UNUSED(engine);
    Q_UNUSED(handlerInfo);
    return QScriptValue(QString::number(value, 10));
}

template<typename T>
QString UIntDataInformationMethods<T>::staticValueString(T value, int base)
{
    QString num = QString::number(value, base);
    if (base == 10)
    {
        if (Kasten2::StructViewPreferences::localeAwareDecimalFormatting())
            num = KGlobal::locale()->formatNumber(num, false, 0);
    }
    else
    {
        //add one space every 8 chars
        for (int i = 8; i < num.length(); i += 9) {
            num.insert(num.length() - i, QLatin1Char(' '));
        }
        if (base == 16)
            num.prepend(QLatin1String("0x"));
        else if (base == 2)
            num.prepend(QLatin1String("0b"));
        else if (base == 8)
            num.prepend(QLatin1String("0o"));
        else
            kDebug() << "unsupported number base" << base;
    }
    return num;
}

template<typename T>
inline QWidget* UIntDataInformationMethods<T>::staticCreateEditWidget(QWidget* parent)
{
    UIntSpinBox* ret = new UIntSpinBox(parent, PrimitiveDataInformation::unsignedDisplayBase());
    ret->setMaximum(std::numeric_limits<T>::max());
    return ret;
}

template<typename T>
inline QVariant UIntDataInformationMethods<T>::staticDataFromWidget(const QWidget* w)
{
    const UIntSpinBox* spin = dynamic_cast<const UIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        return QVariant(spin->value());
    kWarning() << "could not cast widget";
    return QVariant();
}

template<typename T>
inline void UIntDataInformationMethods<T>::staticSetWidgetData(T value, QWidget* w)
{
    UIntSpinBox* spin = dynamic_cast<UIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        spin->setValue(value);
}

//explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class UIntDataInformationMethods<quint8>;
template class UIntDataInformationMethods<quint16>;
template class UIntDataInformationMethods<quint32>;
template class UIntDataInformationMethods<quint64>;
