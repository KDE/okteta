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


#ifndef UNSIGNEDDATAINFORMATION_H
#define UNSIGNEDDATAINFORMATION_H

#include "primitivedatainformation.h"
#include "../poddecoder/typeeditors/uintspinbox.h"

template<typename T>
class UnsignedDataInformation : public PrimitiveDataInformation
{
public:
    explicit UnsignedDataInformation(QString name, DataInformation* parent = 0);
    virtual ~UnsignedDataInformation() {}

    virtual BitCount32 size() const;

    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);

    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const;
    static T fromVariant(const QVariant& value);
    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);
    static int displayBase();

    virtual QScriptValue valueAsQScriptValue() const = 0;
    virtual QString valueString() const = 0;
protected:
    explicit UnsignedDataInformation(const UnsignedDataInformation& d);
protected:
    T mValue;
};

template<typename T>
inline QWidget* UnsignedDataInformation<T>::createEditWidget(QWidget* parent) const
{
    return staticCreateEditWidget(parent);
}

template<typename T>
inline QVariant UnsignedDataInformation<T>::dataFromWidget(const QWidget* w) const
{
    return staticDataFromWidget(w);
}

template<typename T>
inline void UnsignedDataInformation<T>::setWidgetData(QWidget* w) const
{
    staticSetWidgetData(mValue, w);
}

template<typename T>
inline QWidget* UnsignedDataInformation<T>::staticCreateEditWidget(QWidget* parent)
{
    UIntSpinBox* ret = new UIntSpinBox(parent, displayBase());
    ret->setMaximum(std::numeric_limits<T>::max());
    return ret;
}

template<typename T>
inline QVariant UnsignedDataInformation<T>::staticDataFromWidget(const QWidget* w)
{
    const UIntSpinBox* spin = dynamic_cast<const UIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        return QVariant(spin->value());
    kWarning() << "could not cast widget";
    return QVariant();
}

template<typename T>
inline void UnsignedDataInformation<T>::staticSetWidgetData(T value, QWidget* w)
{
    UIntSpinBox* spin = dynamic_cast<UIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        spin->setValue(value);
}

template<typename T>
inline UnsignedDataInformation<T>::UnsignedDataInformation(QString name, DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(0)
{
}

template<typename T>
inline UnsignedDataInformation<T>::UnsignedDataInformation(const UnsignedDataInformation& d)
        : PrimitiveDataInformation(d), mValue(d.mValue)
{
}

template<typename T>
inline AllPrimitiveTypes UnsignedDataInformation<T>::qVariantToAllPrimitiveTypes(const QVariant& value) const
{
    if (!value.isValid())
        kDebug() << "invalid QVariant passed.";

    //This is fine since all the values are unsigned
    return AllPrimitiveTypes(value.toULongLong());
}

template<typename T>
inline int UnsignedDataInformation<T>::displayBase()
{
    int base = Kasten2::StructViewPreferences::unsignedDisplayBase();
    if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
        return 2;
    else if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
        return 16;
    else
        return 10; //safe default value
}

template<typename T>
inline AllPrimitiveTypes UnsignedDataInformation<T>::value() const
{
    return AllPrimitiveTypes(mValue);
}

template<typename T>
inline void UnsignedDataInformation<T>::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.value<T>();
}

template<typename T>
inline BitCount32 UnsignedDataInformation<T>::size() const
{
    return sizeof(T) * 8;
}

template<typename T>
inline T UnsignedDataInformation<T>::fromVariant(const QVariant& value)
{
    return T(value.toUInt());
}

//specialize for 64 bit
template<>
inline quint64 UnsignedDataInformation<quint64>::fromVariant(const QVariant& value)
{
    return value.toULongLong();
}


#endif // UNSIGNEDDATAINFORMATION_H
