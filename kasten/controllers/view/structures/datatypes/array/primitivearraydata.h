/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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
    Q_DISABLE_COPY(PrimitiveArrayData)
public:
    using T = typename PrimitiveInfo<type>::valueType;
    using DisplayClass = typename PrimitiveInfo<type>::Methods;

    explicit PrimitiveArrayData(unsigned int initialLength, PrimitiveDataInformation* childType,
            ArrayDataInformation* parent);
    ~PrimitiveArrayData() override;

    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining) override;
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining) override;

    DataInformation* childAt(unsigned int idx) override;
    QVariant dataAt(uint index, int column, int role) override;
    AllPrimitiveTypes valueAt(int index) const;
    int indexOf(const DataInformation* data) const override;
    unsigned int length() const override;
    void setLength(uint newLength) override;
    BitCount64 offset(const DataInformation* child) const override;
    BitCount32 size() const override;
    PrimitiveDataType primitiveType() const override;
    BitCount32 sizeAt(uint index) override;
    Qt::ItemFlags childFlags(int row, int column, bool fileLoaded) override;

    QScriptValue toScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) override;
    QString typeName() const override;

    bool isComplex() const override;


    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;

    static void writeOneItem(T value, Okteta::Address addr, Okteta::AbstractByteArrayModel* out, bool littleEndian);
protected:
    /** sets mChildType and mDummy as @p index, which must have been checked before calling this method!! */
    void activateIndex(uint index);
    /** reads @p numItems items from the input, sizes must have been checked before calling this method!! */
    void readDataNativeOrder(uint numItems, Okteta::AbstractByteArrayModel* input, Okteta::Address addr);
    /** reads @p numItems items from the input, sizes must have been checked before calling this method!! */
    void readDataNonNativeOrder(uint numItems, Okteta::AbstractByteArrayModel* input, Okteta::Address addr);
    void setNewParentForChildren() override;

    QVector<T> mData;
    uint mNumReadValues; //the number of values read before EOF
    DummyDataInformation mDummy;
};


template<PrimitiveDataType type>
inline PrimitiveArrayData<type>::~PrimitiveArrayData()
{
}

template<PrimitiveDataType type>
inline BitCount64 PrimitiveArrayData<type>::offset(const DataInformation* data) const
{
    Q_ASSERT(data->isDummy());
    const uint index = data->asDummy()->dummyIndex();
    Q_ASSERT(index < length());
    return index * sizeof(T) * 8;
}

template<PrimitiveDataType type>
inline DataInformation* PrimitiveArrayData<type>::childAt(unsigned int idx)
{
    Q_ASSERT(idx < length());
    Q_UNUSED(idx);
    mDummy.setDummyIndex(idx);
    mDummy.setName(QString::number(idx));
    return &mDummy;
}

template<PrimitiveDataType type>
inline void PrimitiveArrayData<type>::setNewParentForChildren()
{
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
void PrimitiveArrayData<type>::setLength(uint newLength)
{
    mData.resize(newLength);
    mData.squeeze();
}

template<PrimitiveDataType type>
inline PrimitiveDataType PrimitiveArrayData<type>::primitiveType() const
{
    return type;
}

template<PrimitiveDataType type>
inline BitCount32 PrimitiveArrayData<type>::sizeAt(uint index)
{
    Q_ASSERT(index < length());
    Q_UNUSED(index)
    return sizeof(T) * 8;
}

template<PrimitiveDataType type>
inline Qt::ItemFlags PrimitiveArrayData<type>::childFlags(int row, int column, bool fileLoaded)
{
    Q_ASSERT(row >= 0 && uint(row) < length());
    Q_UNUSED(row)
    if (column == 2 && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

template<PrimitiveDataType type>
bool PrimitiveArrayData<type>::isComplex() const
{
    return false;
}

template<PrimitiveDataType type>
AllPrimitiveTypes PrimitiveArrayData<type>::valueAt(int index) const
{
    Q_ASSERT(index >= 0 && index < mData.size());
    return AllPrimitiveTypes(mData.at(index));
}

#endif // PRIMITIVEARRAYDATA_H
