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
#include "booldatainformation.h"

template<typename T>
QString BoolDataInformation<T>::staticValueString(T value, int base)
{
    if (value == 0)
        return i18nc("boolean value", "false");
    else if (value == 1)
        return i18nc("boolean value", "true");
    else
    {
        //we can reuse the uint valueString() here
        QString num = UIntDataInformation<T>::staticValueString(value, base);
        return i18nc("boolean value with actual value", "true (%1)", num);
    }
}

template<typename T>
inline QWidget* BoolDataInformation<T>::staticCreateEditWidget(QWidget* parent)
{
    return UIntDataInformation<T>::staticCreateEditWidget(parent);
}

template<typename T>
inline QVariant BoolDataInformation<T>::staticDataFromWidget(const QWidget* w)
{
    return UIntDataInformation<T>::staticDataFromWidget(w);
}

template<typename T>
inline void BoolDataInformation<T>::staticSetWidgetData(T value, QWidget* w)
{
    UIntDataInformation<T>::staticSetWidgetData(value, w);
}

//explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class BoolDataInformation<quint8> ;
template class BoolDataInformation<quint16> ;
template class BoolDataInformation<quint32> ;
template class BoolDataInformation<quint64> ;
