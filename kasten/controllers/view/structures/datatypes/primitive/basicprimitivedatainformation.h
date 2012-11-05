/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012  Alex Richardson <alex.richardson@gmx.de>
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
#ifndef BASICPRIMITIVEDATAINFORMATION_H_
#define BASICPRIMITIVEDATAINFORMATION_H_

#include "primitivedatainformation.h"
#include "../../allprimitivetypes.h"


/** A basic implementation for all primitive types.
 *  T is the data type (e.g. quint8 or float)
 *  C is a namespace containing all needed functions */
template<typename T, class C>
class BasicPrimitiveDataInformation : public PrimitiveDataInformation
{
public:
    BasicPrimitiveDataInformation(const QString& name, DataInformation* parent = 0);
    virtual ~BasicPrimitiveDataInformation();
    virtual QString typeName() const;
    virtual PrimitiveDataType type() const;
    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);
    virtual QString valueString() const;
    virtual BasicPrimitiveDataInformation<T, C>* clone() const;

    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;

    virtual QScriptValue valueAsQScriptValue() const;
    virtual BitCount32 size() const; //TODO declare final with c++11 so it can be inlined
    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);
    virtual qint64 readData(Okteta::AbstractByteArrayModel *input, Okteta::Address address,
            BitCount64 bitsRemaining, quint8* bitOffset);

protected:
    virtual QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const;
    BasicPrimitiveDataInformation(const BasicPrimitiveDataInformation<T, C>& d);
protected:
    T mValue;
};

template<typename T, typename C>
inline BasicPrimitiveDataInformation<T, C>::BasicPrimitiveDataInformation(const QString& name,
        DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(0)
{
}

template<typename T, typename C>
inline BasicPrimitiveDataInformation<T, C>::BasicPrimitiveDataInformation(
        const BasicPrimitiveDataInformation<T, C>& d)
        : PrimitiveDataInformation(d), mValue(d.mValue)
{
}

template<typename T, typename C>
inline BasicPrimitiveDataInformation<T, C>::~BasicPrimitiveDataInformation()
{
}

template<typename T, typename C>
inline QString BasicPrimitiveDataInformation<T, C>::typeName() const
{
    return PrimitiveType::typeName(C::staticType());
}

template<typename T, typename C>
inline AllPrimitiveTypes BasicPrimitiveDataInformation<T, C>::value() const
{
    return AllPrimitiveTypes(mValue);
}

template<typename T, typename C>
inline void BasicPrimitiveDataInformation<T, C>::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.value<T>();

}

template<typename T, typename C>
inline BitCount32 BasicPrimitiveDataInformation<T, C>::size() const
{
    return sizeof(T) * 8;
}

template<typename T, typename C>
inline QVariant BasicPrimitiveDataInformation<T, C>::dataFromWidget(const QWidget* w) const
{
    return C::staticDataFromWidget(w);
}

template<typename T, typename C>
inline QWidget* BasicPrimitiveDataInformation<T, C>::createEditWidget(QWidget* parent) const
{
    return C::staticCreateEditWidget(parent);
}

template<typename T, typename C>
inline void BasicPrimitiveDataInformation<T, C>::setWidgetData(QWidget* w) const
{
    C::staticSetWidgetData(mValue, w);
}

template<typename T, typename C>
inline PrimitiveDataType BasicPrimitiveDataInformation<T, C>::type() const
{
    return C::staticType();
}

template<typename T, typename C>
inline BasicPrimitiveDataInformation<T, C>* BasicPrimitiveDataInformation<T, C>::clone() const
{
    return new BasicPrimitiveDataInformation<T, C>(*this);
}

#endif /* BASICPRIMITIVEDATAINFORMATION_H_ */
