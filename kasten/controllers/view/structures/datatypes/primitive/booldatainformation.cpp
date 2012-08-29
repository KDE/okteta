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

#include <KLocalizedString>

template<typename T>
QString BoolDataInformationMethods<T>::staticValueString(T value, int base)
{
    if (value == 0)
        return i18nc("boolean value", "false");
    else if (value == 1)
        return i18nc("boolean value", "true");
    else
    {
        //we can reuse the uint valueString() here
        QString num = UIntDataInformationMethods<T>::staticValueString(value, base);
        return i18nc("boolean value with actual value", "true (%1)", num);
    }
}

template<typename T>
inline QWidget* BoolDataInformationMethods<T>::staticCreateEditWidget(QWidget* parent)
{
    return UIntDataInformationMethods<T>::staticCreateEditWidget(parent);
}

template<typename T>
inline QVariant BoolDataInformationMethods<T>::staticDataFromWidget(const QWidget* w)
{
    return UIntDataInformationMethods<T>::staticDataFromWidget(w);
}

template<typename T>
inline void BoolDataInformationMethods<T>::staticSetWidgetData(T value, QWidget* w)
{
    UIntDataInformationMethods<T>::staticSetWidgetData(value, w);
}

//explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class BoolDataInformationMethods<quint8> ;
template class BoolDataInformationMethods<quint16> ;
template class BoolDataInformationMethods<quint32> ;
template class BoolDataInformationMethods<quint64> ;
