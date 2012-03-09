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

template<typename T>
class SIntDataInformation : public PrimitiveDataInformation
{
public:
    explicit SIntDataInformation(QString name, DataInformation* parent = 0);
    virtual ~SIntDataInformation() {}
    virtual SIntDataInformation<T>* clone() const;

    virtual PrimitiveDataType type() const;
    virtual BitCount32 size() const;

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
    static QString valueString(T val, int base);
protected:
    explicit SIntDataInformation(const SIntDataInformation& d);
protected:
    T mValue;
};

template<>
inline PrimitiveDataType SIntDataInformation<qint8>::type() const { return Type_Int8; }
template<>
inline PrimitiveDataType SIntDataInformation<qint16>::type() const { return Type_Int16; }
template<>
inline PrimitiveDataType SIntDataInformation<qint32>::type() const { return Type_Int32; }
template<>
inline PrimitiveDataType SIntDataInformation<qint64>::type() const { return Type_Int64; }

template<typename T>
inline QWidget* SIntDataInformation<T>::staticCreateEditWidget(QWidget* parent)
{
    SIntSpinBox* ret = new SIntSpinBox(parent, PrimitiveDataInformation::signedDisplayBase());
    ret->setRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return ret;
}

template<typename T>
inline QVariant SIntDataInformation<T>::staticDataFromWidget(const QWidget* w)
{
    const SIntSpinBox* spin = dynamic_cast<const SIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        return spin->value();

    kWarning() << "could not cast widget";
    return QVariant();
}

template<typename T>
inline void SIntDataInformation<T>::staticSetWidgetData(T value, QWidget* w)
{
    SIntSpinBox* spin = dynamic_cast<SIntSpinBox*> (w);
    Q_CHECK_PTR(spin);
    if (spin)
        spin->setValue(value);
}

template<typename T>
inline SIntDataInformation<T>::SIntDataInformation(QString name, DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(0)
{
}

template<typename T>
inline SIntDataInformation<T>::SIntDataInformation(const SIntDataInformation& d)
        : PrimitiveDataInformation(d), mValue(d.mValue)
{
}

template<typename T>
inline SIntDataInformation<T>* SIntDataInformation<T>::clone() const
{
    return new SIntDataInformation<T>(*this);
}

template<typename T>
inline AllPrimitiveTypes SIntDataInformation<T>::qVariantToAllPrimitiveTypes(const QVariant& value) const
{
    Q_ASSERT(value.isValid());
    return AllPrimitiveTypes(value.toLongLong());
}

template<typename T>
inline AllPrimitiveTypes SIntDataInformation<T>::value() const
{
    return AllPrimitiveTypes(mValue);
}

template<typename T>
inline void SIntDataInformation<T>::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.value<T>();
}

template<typename T>
inline BitCount32 SIntDataInformation<T>::size() const
{
    return sizeof(T) * 8;
}

template<typename T>
inline QString SIntDataInformation<T>::valueString(T value)
{
    return SIntDataInformation<T>::valueString(value, PrimitiveDataInformation::signedDisplayBase());
}

template<typename T>
inline QString SIntDataInformation<T>::typeName() const
{
    return PrimitiveDataInformation::typeName(this->type());
}

template<typename T>
inline T SIntDataInformation<T>::fromVariant(const QVariant& value)
{
    return T(value.toInt());
}

template<>
inline qint64 SIntDataInformation<qint64>::fromVariant(const QVariant& value)
{
    return value.toLongLong();
}

#endif // SINTDATAINFORMATION_H
