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

template<typename T, PrimitiveDataType typeValue>
QString BoolDataInformation<T, typeValue>::valueString() const
{
    if (!BoolDataInformation::mWasAbleToRead)
        return i18nc("invalid value (out of range)", "&lt;invalid&gt;");
    return BoolDataInformation<T, typeValue>::valueString(UnsignedDataInformation<T>::mValue);
}

template<typename T, PrimitiveDataType typeValue>
QString BoolDataInformation<T, typeValue>::valueString(T value) {
    int base = UnsignedDataInformation<T>::displayBase();
    if (value == 0)
        return i18nc("boolean value", "false");
    else if (value == 1)
        return i18nc("boolean value", "true");
    else
    {
        QString num = QString::number(value, base);
        if (base == 16)
            num.prepend(QLatin1String("0x"));
        else if (base == 2)
            num.prepend(QLatin1String("0b"));
        else if (base == 8)
            num.prepend(QLatin1String("0o"));
        else if (base == 10 && Kasten2::StructViewPreferences::localeAwareDecimalFormatting())
            num = KGlobal::locale()->formatNumber(num, false, 0);
        return i18nc("boolean value with actual value", "true (%1)", num);
    }
}

//explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class BoolDataInformation<quint8, Type_Bool8>;
template class BoolDataInformation<quint16, Type_Bool16>;
template class BoolDataInformation<quint32, Type_Bool32>;
template class BoolDataInformation<quint64, Type_Bool64>;
