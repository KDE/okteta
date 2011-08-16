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

#include "primitivedatainformation.h"
#include "../poddecoder/typeeditors/uintspinbox.h"

template<typename T, PrimitiveDataType typeValue>
class UIntDataInformation : public PrimitiveDataInformation
{
public:
    explicit UIntDataInformation(QString name, DataInformation* parent = 0);
    virtual ~UIntDataInformation() {}
    virtual DataInformation* clone() const;

    virtual PrimitiveDataType type() const;
    virtual int size() const;
    virtual int displayBase() const;
    virtual QString valueString() const;

    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);

    virtual QString typeName() const;

    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const;

    virtual QScriptValue valueAsQScriptValue() const;
protected:
    explicit UIntDataInformation(const UIntDataInformation& d);
private:
    T mValue;
};

template<typename T, PrimitiveDataType typeValue>
PrimitiveDataType UIntDataInformation<T, typeValue>::type() const
{
    return typeValue;
}

template<typename T, PrimitiveDataType typeValue>
QWidget* UIntDataInformation<T, typeValue>::createEditWidget(QWidget* parent) const
{
    UIntSpinBox* ret = new UIntSpinBox(parent, displayBase());
    ret->setMaximum(std::numeric_limits<T>::max());
    return ret;
}

template<typename T, PrimitiveDataType typeValue>
QVariant UIntDataInformation<T, typeValue>::dataFromWidget(const QWidget* w) const
{
    const UIntSpinBox* spin = dynamic_cast<const UIntSpinBox*> (w);
    if (spin)
        return spin->value();
    else
    {
        kWarning() << "could not cast widget";
        return QVariant();
    }
}

template<typename T, PrimitiveDataType typeValue>
void UIntDataInformation<T, typeValue>::setWidgetData(QWidget* w) const
{
    UIntSpinBox* spin = dynamic_cast<UIntSpinBox*> (w);
    if (spin)
        spin->setValue(mValue);
}

template<typename T, PrimitiveDataType typeValue>
UIntDataInformation<T, typeValue>::UIntDataInformation(QString name, DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(0)
{
}

template<typename T, PrimitiveDataType typeValue>
UIntDataInformation<T, typeValue>::UIntDataInformation(const UIntDataInformation& d)
        : PrimitiveDataInformation(d), mValue(d.mValue)
{
}

template<typename T, PrimitiveDataType typeValue>
AllPrimitiveTypes UIntDataInformation<T, typeValue>::qVariantToAllPrimitiveTypes(const QVariant& value) const
{
    if (!value.isValid())
        kDebug() << "invalid QVariant passed.";

    //This is fine since all the values are unsigned
    return AllPrimitiveTypes(value.toULongLong());
}

template<typename T, PrimitiveDataType typeValue>
int UIntDataInformation<T, typeValue>::displayBase() const
{
    int base = Kasten::StructViewPreferences::unsignedDisplayBase();
    if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
        return 2;
    else if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
        return 16;
    else
        return 10; //safe default value
}

template<typename T, PrimitiveDataType typeValue>
DataInformation* UIntDataInformation<T, typeValue>::clone() const
{
    return new UIntDataInformation<T, typeValue>(*this);
}

template<typename T, PrimitiveDataType typeValue>
AllPrimitiveTypes UIntDataInformation<T, typeValue>::value() const
{
    return AllPrimitiveTypes(mValue);
}

template<typename T, PrimitiveDataType typeValue>
void UIntDataInformation<T, typeValue>::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.ulongValue; //This is safe since the value is unsigned
}

template<typename T, PrimitiveDataType typeValue>
int UIntDataInformation<T, typeValue>::size() const
{
    return sizeof(T) * 8;
}

template<typename T, PrimitiveDataType typeValue>
QString UIntDataInformation<T, typeValue>::typeName() const
{
    return PrimitiveDataInformation::typeName(typeValue);
}

template<typename T, PrimitiveDataType typeValue>
QString UIntDataInformation<T, typeValue>::valueString() const
{
    if (!mWasAbleToRead)
        return i18nc("invalid value (out of range)", "&lt;invalid&gt;");
    int base = displayBase();
    QString num = QString::number(mValue, base);
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
    return QScriptValue(mValue);
}

template<>
inline QScriptValue UIntDataInformation<quint64, Type_UInt64>::valueAsQScriptValue() const
{
    return QString::number(mValue, 10);
}

#endif // UINTDATAINFORMATION_H
