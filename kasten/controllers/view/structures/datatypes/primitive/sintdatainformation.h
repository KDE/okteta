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
    virtual SIntDataInformation<T, typeValue>* clone() const;

    virtual PrimitiveDataType type() const;
    virtual BitCount32 size() const;
    static int displayBase();

    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);

    virtual QString typeName() const;

    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);

    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const;
    static T fromVariant(const QVariant& value);
    virtual QScriptValue valueAsQScriptValue() const;
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    virtual QString valueString() const;
    static QString valueString(T val);
protected:
    explicit SIntDataInformation(const SIntDataInformation& d);
protected:
    T mValue;
};

template<typename T, PrimitiveDataType typeValue>
inline PrimitiveDataType SIntDataInformation<T, typeValue>::type() const
{
    return typeValue;
}

template<typename T, PrimitiveDataType typeValue>
inline QWidget* SIntDataInformation<T, typeValue>::staticCreateEditWidget(QWidget* parent)
{
    SIntSpinBox* ret = new SIntSpinBox(parent, displayBase());
    ret->setRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return ret;
}

template<typename T, PrimitiveDataType typeValue>
QVariant SIntDataInformation<T, typeValue>::staticDataFromWidget(const QWidget* w)
{
    const SIntSpinBox* spin = dynamic_cast<const SIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        return spin->value();

    kWarning() << "could not cast widget";
    return QVariant();
}

template<typename T, PrimitiveDataType typeValue>
inline void SIntDataInformation<T, typeValue>::staticSetWidgetData(T value, QWidget* w)
{
    SIntSpinBox* spin = dynamic_cast<SIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        spin->setValue(value);
}

template<typename T, PrimitiveDataType typeValue>
inline SIntDataInformation<T, typeValue>::SIntDataInformation(QString name, DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(0)
{
}

template<typename T, PrimitiveDataType typeValue>
inline SIntDataInformation<T, typeValue>::SIntDataInformation(const SIntDataInformation& d)
        : PrimitiveDataInformation(d), mValue(d.mValue)
{
}

template<typename T, PrimitiveDataType typeValue>
inline SIntDataInformation<T, typeValue>* SIntDataInformation<T, typeValue>::clone() const
{
    return new SIntDataInformation<T, typeValue>(*this);
}

template<typename T, PrimitiveDataType typeValue>
inline AllPrimitiveTypes SIntDataInformation<T, typeValue>::qVariantToAllPrimitiveTypes(const QVariant& value) const
{
    Q_ASSERT(value.isValid());
    return AllPrimitiveTypes(value.toLongLong());
}

template<typename T, PrimitiveDataType typeValue>
inline int SIntDataInformation<T, typeValue>::displayBase()
{
    int base = Kasten2::StructViewPreferences::signedDisplayBase();
    if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
        return 2;
    else if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
        return 16;
    else
        return 10; //safe default value
}

template<typename T, PrimitiveDataType typeValue>
inline AllPrimitiveTypes SIntDataInformation<T, typeValue>::value() const
{
    return AllPrimitiveTypes(mValue);
}

template<typename T, PrimitiveDataType typeValue>
inline void SIntDataInformation<T, typeValue>::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.value<T>();
}

template<typename T, PrimitiveDataType typeValue>
inline BitCount32 SIntDataInformation<T, typeValue>::size() const
{
    return sizeof(T) * 8;
}

template<typename T, PrimitiveDataType typeValue>
inline QString SIntDataInformation<T, typeValue>::typeName() const
{
    return PrimitiveDataInformation::typeName(typeValue);
}

template<typename T, PrimitiveDataType typeValue>
inline T SIntDataInformation<T, typeValue>::fromVariant(const QVariant& value)
{
    return T(value.toInt());
}

template<>
inline qint64 SIntDataInformation<qint64, Type_Int64>::fromVariant(const QVariant& value)
{
    return value.toLongLong();
}

#endif // SINTDATAINFORMATION_H
