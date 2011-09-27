/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef PRIMITIVEARRAYDATA_H
#define PRIMITIVEARRAYDATA_H

#include "abstractarraydata.h"
#include "../datainformation.h"
#include "../dummydatainformation.h"
#include "../topleveldatainformation.h"
#include "../primitive/primitivetemplateinfo.h"

template<PrimitiveDataType type>
class PrimitiveArrayData : public AbstractArrayData
{
public:
    typedef typename PrimitiveInfo<type>::valueType T;
    typedef typename PrimitiveInfo<type>::Class DisplayClass;
    explicit PrimitiveArrayData(unsigned int initialLength, DataInformation* parent);
    PrimitiveArrayData(const PrimitiveArrayData& a);
    virtual ~PrimitiveArrayData();
    virtual PrimitiveArrayData* clone();

    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            BitCount64 bitsRemaining);
    virtual bool setChildData(uint row, QVariant value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining);

    virtual DataInformation* childAt(unsigned int idx);
    virtual QVariant dataAt(int index, int column, int role);
    virtual int indexOf(const DataInformation* data) const;
    virtual unsigned int length() const;
    virtual BitCount32 offset(uint row) const;
    virtual void setLength(int newLength);
    virtual BitCount32 size() const;
    virtual PrimitiveDataType primitiveType() const;

    virtual QScriptValue toScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const;
    virtual QString typeName() const;

    //has to be public so PrimitiveArrayData<Type_Float | Type_Double> can call it
    /** reads exactly one item at specified address, it must have been verified that enough data is remaining*/
    static T readOneItem(Okteta::Address addr, Okteta::AbstractByteArrayModel* input, bool littleEndian);
    /** reads exactly one item at specified address, it must have been verified that enough space is remaining*/
    static void writeOneItem(T value, Okteta::Address addr, Okteta::AbstractByteArrayModel* out, bool littleEndian);
protected:

    QVector<T> mData;
    uint mNumReadValues; //the number of values read before EOF
    DummyDataInformation* mDummy;
};

//constructors
template<PrimitiveDataType type>
inline PrimitiveArrayData<type>::PrimitiveArrayData(unsigned int initialLength, DataInformation* parent)
        : AbstractArrayData(parent), mNumReadValues(0), mDummy(new DummyDataInformation(mParent))
{
    mData.reserve(initialLength);
    mData.resize(initialLength);
}

template<PrimitiveDataType type>
inline PrimitiveArrayData<type>::PrimitiveArrayData(const PrimitiveArrayData& a)
    : AbstractArrayData(a), mData(a.mData), mNumReadValues(a.mNumReadValues), mDummy(new DummyDataInformation(mParent))
{
}

template<PrimitiveDataType type>
inline PrimitiveArrayData<type>::~PrimitiveArrayData()
{
    delete mDummy;
}

template<PrimitiveDataType type>
inline PrimitiveArrayData<type>* PrimitiveArrayData<type>::clone()
{
    return new PrimitiveArrayData<type>(*this);
}

template<PrimitiveDataType type>
inline BitCount32 PrimitiveArrayData<type>::offset(uint index) const
{
    Q_ASSERT(index < length());
    return index * sizeof(T) * 8;
}

template<PrimitiveDataType type>
inline DataInformation* PrimitiveArrayData<type>::childAt(unsigned int idx)
{
    Q_ASSERT(idx < length());
    Q_UNUSED(idx);
    return mDummy;
}

template<PrimitiveDataType type>
inline unsigned int PrimitiveArrayData<type>::length() const
{
    return mData.size();
}

template<PrimitiveDataType type>
inline BitCount32 PrimitiveArrayData<type>::size() const
{
    return mData.size() * sizeof(T) * 8;
}

template<PrimitiveDataType type>
void PrimitiveArrayData<type>::setLength(int newLength)
{
    Q_ASSERT(newLength >= 0);
    mData.resize(newLength);
    mData.squeeze();
}

template<PrimitiveDataType type>
inline PrimitiveDataType PrimitiveArrayData<type>::primitiveType() const
{
    return type;
}


#endif // PRIMITIVEARRAYDATA_H
