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


#ifndef BOOLDATAINFORMATION_H
#define BOOLDATAINFORMATION_H

#include "unsigneddatainformation.h"
#include <KDebug>


template<typename T, PrimitiveDataType typeValue>
class BoolDataInformation : public UnsignedDataInformation<T, typeValue>
{
public:
    explicit BoolDataInformation(QString name, DataInformation* parent = 0);
    virtual ~BoolDataInformation() {}
    virtual DataInformation* clone() const;

    virtual QScriptValue valueAsQScriptValue() const;
    virtual QString valueString() const;
protected:
    explicit BoolDataInformation(const BoolDataInformation& d);
};

template<typename T, PrimitiveDataType typeValue>
QScriptValue BoolDataInformation<T, typeValue>::valueAsQScriptValue() const
{
    return QScriptValue(bool(UnsignedDataInformation<T, typeValue>::mValue));
}

template<typename T, PrimitiveDataType typeValue>
QString BoolDataInformation<T, typeValue>::valueString() const
{
    if (!DataInformation::mWasAbleToRead)
        return i18nc("invalid value (out of range)", "&lt;invalid&gt;");
    int base = UnsignedDataInformation<T, typeValue>::displayBase();
    if (UnsignedDataInformation<T, typeValue>::mValue == 0)
        return i18nc("boolean value", "false");
    else if (UnsignedDataInformation<T, typeValue>::mValue == 1)
        return i18nc("boolean value", "true");
    else
    {
        QString num = QString::number(UnsignedDataInformation<T, typeValue>::mValue, base);
        if (base == 16)
            num.prepend(QLatin1String("0x"));
        else if (base == 2)
            num.prepend(QLatin1String("0b"));
        else if (base == 8)
            num.prepend(QLatin1String("0o"));
        else if (base == 10 && Kasten::StructViewPreferences::localeAwareDecimalFormatting())
            num = KGlobal::locale()->formatNumber(num, false, 0);
        return i18nc("boolean value with actual value", "true (%1)", num);
    }
}

template<typename T, PrimitiveDataType typeValue>
BoolDataInformation<T, typeValue>::BoolDataInformation(QString name, DataInformation* parent)
        : UnsignedDataInformation<T, typeValue>(name, parent)
{
}

template<typename T, PrimitiveDataType typeValue>
BoolDataInformation<T, typeValue>::BoolDataInformation(const BoolDataInformation& d)
        : UnsignedDataInformation<T, typeValue>(d)
{
}

template<typename T, PrimitiveDataType typeValue>
DataInformation* BoolDataInformation<T, typeValue>::clone() const
{
    return new BoolDataInformation<T, typeValue>(*this);
}

#endif // BOOLDATAINFORMATION_H
