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


#ifndef SINTDATAINFORMATION_H
#define SINTDATAINFORMATION_H

#include "primitivedatainformation.h"
#include "../poddecoder/typeeditors/sintspinbox.h"

template<typename T, PrimitiveDataType typeValue>
class SIntDataInformation : public PrimitiveDataInformation
{
public:
    explicit SIntDataInformation(QString name, DataInformation* parent = 0);
    virtual ~SIntDataInformation() {}
    virtual DataInformation* clone() const;

    virtual PrimitiveDataType type() const;
    virtual int size() const;
    virtual int displayBase() const;

    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);

    virtual QString typeName() const;

    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const;

    virtual QScriptValue valueAsQScriptValue() const;
    virtual QString valueString() const;
protected:
    explicit SIntDataInformation(const SIntDataInformation& d);
protected:
    T mValue;
};

template<typename T, PrimitiveDataType typeValue>
PrimitiveDataType SIntDataInformation<T, typeValue>::type() const
{
    return typeValue;
}

template<typename T, PrimitiveDataType typeValue>
QWidget* SIntDataInformation<T, typeValue>::createEditWidget(QWidget* parent) const
{
    SIntSpinBox* ret = new SIntSpinBox(parent, displayBase());
    ret->setRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return ret;
}

template<typename T, PrimitiveDataType typeValue>
QVariant SIntDataInformation<T, typeValue>::dataFromWidget(const QWidget* w) const
{
    const SIntSpinBox* spin = dynamic_cast<const SIntSpinBox*> (w);
    if (spin)
        return spin->value();
    else
    {
        kWarning() << "could not cast widget";
        return QVariant();
    }
}

template<typename T, PrimitiveDataType typeValue>
void SIntDataInformation<T, typeValue>::setWidgetData(QWidget* w) const
{
    SIntSpinBox* spin = dynamic_cast<SIntSpinBox*> (w);
    if (spin)
        spin->setValue(mValue);
}

template<typename T, PrimitiveDataType typeValue>
SIntDataInformation<T, typeValue>::SIntDataInformation(QString name, DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(0)
{
}

template<typename T, PrimitiveDataType typeValue>
SIntDataInformation<T, typeValue>::SIntDataInformation(const SIntDataInformation& d)
        : PrimitiveDataInformation(d), mValue(d.mValue)
{
}

template<typename T, PrimitiveDataType typeValue>
DataInformation* SIntDataInformation<T, typeValue>::clone() const
{
    return new SIntDataInformation<T, typeValue>(*this);
}



template<typename T, PrimitiveDataType typeValue>
AllPrimitiveTypes SIntDataInformation<T, typeValue>::qVariantToAllPrimitiveTypes(const QVariant& value) const
{
    if (!value.isValid())
        kDebug() << "invalid QVariant passed.";

    return AllPrimitiveTypes(value.toLongLong());
}

template<typename T, PrimitiveDataType typeValue>
int SIntDataInformation<T, typeValue>::displayBase() const
{
    int base = Kasten::StructViewPreferences::signedDisplayBase();
    if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
        return 2;
    else if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
        return 16;
    else
        return 10; //safe default value
}

template<typename T, PrimitiveDataType typeValue>
AllPrimitiveTypes SIntDataInformation<T, typeValue>::value() const
{
    return AllPrimitiveTypes(mValue);
}

template<typename T, PrimitiveDataType typeValue>
void SIntDataInformation<T, typeValue>::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.ulongValue; //This is safe since the value is unsigned
}

template<typename T, PrimitiveDataType typeValue>
int SIntDataInformation<T, typeValue>::size() const
{
    return sizeof(T) * 8;
}

template<typename T, PrimitiveDataType typeValue>
QString SIntDataInformation<T, typeValue>::typeName() const
{
    return PrimitiveDataInformation::typeName(typeValue);
}

template<typename T, PrimitiveDataType typeValue>
QScriptValue SIntDataInformation<T, typeValue>::valueAsQScriptValue() const
{
    return QScriptValue(mValue);
}

template<>
QScriptValue SIntDataInformation<qint64, Type_Int64>::valueAsQScriptValue() const
{
    return QScriptValue(QString::number(mValue, 10));
}

template<typename T, PrimitiveDataType typeValue>
QString SIntDataInformation<T, typeValue>::valueString() const
{
    if (!mWasAbleToRead)
        return i18nc("invalid value (out of range)", "<invalid>");
    int base = displayBase();
    QString num;
    if (mValue >= 0)
    {
        //no need to do anything special
        num = QString::number(mValue, base);
        if (base == 16)
            num.prepend(QLatin1String("0x"));
        else if (base == 8)
            num.prepend(QLatin1String("0o"));
        else if (base == 2)
            num.prepend(QLatin1String("0b"));
        else if (base == 10 && Kasten::StructViewPreferences::localeAwareDecimalFormatting())
            num = KGlobal::locale()->formatNumber(num, false, 0);
        return num;
    }
    //value is less than zero, now the tricky bit starts
    //TODO non decimal negative values as unsigned? probably add option
    if (base == 10) {
        num = QString::number(mValue, base);
        if (Kasten::StructViewPreferences::localeAwareDecimalFormatting())
            num = KGlobal::locale()->formatNumber(num, false, 0);
        return num;
    }
    if (mValue == std::numeric_limits<T>::min())
        num = QString::number(quint64(mValue) & ((quint64(std::numeric_limits<T>::max()) << 1) + 1), base);
    else
        num = QString::number(qAbs(mValue), base);

    if (base == 16)
        num.prepend(QLatin1String("-0x"));
    else if (base == 8)
        num.prepend(QLatin1String("-0o"));
    else if (base == 2)
        num.prepend(QLatin1String("-0b"));

    return num;
}

#endif // SINTDATAINFORMATION_H
