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


#ifndef UINTDATAINFORMATION_H
#define UINTDATAINFORMATION_H

#include "unsigneddatainformation.h"
#include <KDebug>


template<typename T, PrimitiveDataType typeValue>
class UIntDataInformation : public UnsignedDataInformation<T, typeValue>
{
public:
    explicit UIntDataInformation(QString name, DataInformation* parent = 0);
    virtual ~UIntDataInformation() {}
    virtual DataInformation* clone() const;

    virtual QScriptValue valueAsQScriptValue() const;
    virtual QString valueString() const;
protected:
    explicit UIntDataInformation(const UIntDataInformation& d);
};

template<typename T, PrimitiveDataType typeValue>
UIntDataInformation<T, typeValue>::UIntDataInformation(QString name, DataInformation* parent)
        : UnsignedDataInformation<T, typeValue>(name, parent)
{
}

template<typename T, PrimitiveDataType typeValue>
UIntDataInformation<T, typeValue>::UIntDataInformation(const UIntDataInformation& d)
        : UnsignedDataInformation<T, typeValue>(d)
{
}

template<typename T, PrimitiveDataType typeValue>
DataInformation* UIntDataInformation<T, typeValue>::clone() const
{
    return new UIntDataInformation<T, typeValue>(*this);
}


template<typename T, PrimitiveDataType typeValue>
QString UIntDataInformation<T, typeValue>::valueString() const
{
    if (!DataInformation::mWasAbleToRead)
        return i18nc("invalid value (out of range)", "&lt;invalid&gt;");
    int base = UnsignedDataInformation<T, typeValue>::displayBase();
    QString num = QString::number(UnsignedDataInformation<T, typeValue>::mValue, base);
    if (base == 16)
        num.prepend(QLatin1String("0x"));
    else if (base == 2)
        num.prepend(QLatin1String("0b"));
    else if (base == 8)
        num.prepend(QLatin1String("0o"));
    else if (base == 10 && Kasten::StructViewPreferences::localeAwareDecimalFormatting())
        num = KGlobal::locale()->formatNumber(num, false, 0);
    return num;
}

template<typename T, PrimitiveDataType typeValue>
QScriptValue UIntDataInformation<T, typeValue>::valueAsQScriptValue() const
{
    return QScriptValue(UnsignedDataInformation<T, typeValue>::mValue);
}

template<>
inline QScriptValue UIntDataInformation<quint64, Type_UInt64>::valueAsQScriptValue() const
{
    return QScriptValue(QString::number(mValue, 10));
}

#endif // UINTDATAINFORMATION_H
