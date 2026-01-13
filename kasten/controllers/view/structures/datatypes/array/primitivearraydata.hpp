/*
    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRIMITIVEARRAYDATA_HPP
#define KASTEN_PRIMITIVEARRAYDATA_HPP

// lib
#include "abstractarraydata.hpp"
#include <datainformation.hpp>
#include <dummydatainformation.hpp>
#include <topleveldatainformation.hpp>
#include <primitivetemplateinfo.hpp>
// Std
#include <vector>

template <PrimitiveDataType type>
class PrimitiveArrayData : public AbstractArrayData
{
public:
    using T = typename PrimitiveInfo<type>::valueType;
    using DisplayClass = typename PrimitiveInfo<type>::Methods;

public:
    explicit PrimitiveArrayData(unsigned int supportedLength, unsigned int length,
                                std::unique_ptr<PrimitiveDataInformation>&& childType,
                                ArrayDataInformation* parent);
    ~PrimitiveArrayData() override;

public: // AbstractArrayData API
    [[nodiscard]]
    unsigned int supportedLength() const override;
    [[nodiscard]]
    unsigned int length() const override;
    void setLength(uint supportedLength, uint length) override;

    [[nodiscard]]
    QString typeName() const override;
    [[nodiscard]]
    QString valueString() const override;

    [[nodiscard]]
    BitCount32 size() const override;

    [[nodiscard]]
    qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining) override;
    [[nodiscard]]
    QScriptValue toScriptValue(uint index, QScriptEngine* engine,
                               ScriptHandlerInfo* handlerInfo) override;
    [[nodiscard]]
    PrimitiveDataType primitiveType() const override;
    [[nodiscard]]
    bool isComplex() const override;

    [[nodiscard]]
    DataInformation* childAt(unsigned int idx) override;
    [[nodiscard]]
    int indexOf(const DataInformation* data) const override;
    [[nodiscard]]
    BitCount64 offset(const DataInformation* data) const override;
    [[nodiscard]]
    QVariant dataAt(uint index, int column, int role) override;
    [[nodiscard]]
    BitCount32 sizeAt(uint index) const override;
    [[nodiscard]]
    Qt::ItemFlags childFlags(int row, int column, bool fileLoaded) const override;
    [[nodiscard]]
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining) override;

    [[nodiscard]]
    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;

public:
    [[nodiscard]]
    QString nameAt(uint index) const;
    [[nodiscard]]
    AllPrimitiveTypes valueAt(int index) const;
    [[nodiscard]]
    QString valueStringAt(uint index); // cannot be const due to calling activateIndex()
    [[nodiscard]]
    QString sizeStringAt(uint index) const;

public:
    static void writeOneItem(T value, Okteta::Address addr, Okteta::AbstractByteArrayModel* out, QSysInfo::Endian endianness);

protected: // AbstractArrayData API
    void setNewParentForChildren() override;

protected:
    /** sets mChildType and mDummy as @p index, which must have been checked before calling this method!! */
    void activateIndex(uint index);
    /** reads @p numItems items from the input, sizes must have been checked before calling this method!! */
    void readDataNativeOrder(uint numItems, const Okteta::AbstractByteArrayModel* input, Okteta::Address addr);
    /** reads @p numItems items from the input, sizes must have been checked before calling this method!! */
    void readDataNonNativeOrder(uint numItems, const Okteta::AbstractByteArrayModel* input, Okteta::Address addr);

protected:
    std::vector<T> mData;
    uint m_length = 0;
    uint mNumReadValues = 0; // the number of values read before EOF
    DummyDataInformation mDummy;
};

template <PrimitiveDataType type>
inline PrimitiveArrayData<type>::~PrimitiveArrayData() = default;

template <PrimitiveDataType type>
inline BitCount64 PrimitiveArrayData<type>::offset(const DataInformation* data) const
{
    Q_ASSERT(data->isDummy());
    const uint index = data->asDummy()->dummyIndex();
    Q_ASSERT(index < supportedLength());
    return index * sizeof(T) * 8;
}

template <PrimitiveDataType type>
inline DataInformation* PrimitiveArrayData<type>::childAt(unsigned int idx)
{
    Q_ASSERT(idx < supportedLength());
    Q_UNUSED(idx)
    mDummy.setDummyIndex(idx);
    mDummy.setName(QString::number(idx));
    return &mDummy;
}

template <PrimitiveDataType type>
inline void PrimitiveArrayData<type>::setNewParentForChildren()
{
}

template <PrimitiveDataType type>
inline unsigned int PrimitiveArrayData<type>::supportedLength() const
{
    return mData.size();
}

template <PrimitiveDataType type>
inline unsigned int PrimitiveArrayData<type>::length() const
{
    return m_length;
}

template <PrimitiveDataType type>
inline BitCount32 PrimitiveArrayData<type>::size() const
{
    return m_length * sizeof(T) * 8;
}

template <PrimitiveDataType type>
void PrimitiveArrayData<type>::setLength(uint supportedLength, uint length)
{
    mData.resize(supportedLength);
    mData.shrink_to_fit();
    m_length = length;
}

template <PrimitiveDataType type>
inline PrimitiveDataType PrimitiveArrayData<type>::primitiveType() const
{
    return type;
}

template <PrimitiveDataType type>
inline BitCount32 PrimitiveArrayData<type>::sizeAt(uint index) const
{
    Q_ASSERT(index < length());
    Q_UNUSED(index)
    return sizeof(T) * 8;
}

template <PrimitiveDataType type>
inline Qt::ItemFlags PrimitiveArrayData<type>::childFlags(int row, int column, bool fileLoaded) const
{
    Q_ASSERT(row >= 0 && uint(row) < length());
    Q_UNUSED(row)
    if (column == 2 && fileLoaded) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

template <PrimitiveDataType type>
bool PrimitiveArrayData<type>::isComplex() const
{
    return false;
}

template <PrimitiveDataType type>
AllPrimitiveTypes PrimitiveArrayData<type>::valueAt(int index) const
{
    Q_ASSERT(0 <= index && index < static_cast<int>(mData.size()));
    return AllPrimitiveTypes(mData[index]);
}

#endif // KASTEN_PRIMITIVEARRAYDATA_HPP
