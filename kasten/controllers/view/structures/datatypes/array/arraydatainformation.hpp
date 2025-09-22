/*
    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ARRAYDATAINFORMATION_HPP
#define KASTEN_ARRAYDATAINFORMATION_HPP

// lib
#include "abstractarraydata.hpp"
#include <dummydatainformation.hpp>
// Qt
#include <QVariant>
#include <QScriptValue>
// Std
#include <memory>

class AbstractArrayData;

class ArrayDataInformation : public DataInformationWithDummyChildren
{
    friend class PrimitiveArrayTest;
    DATAINFORMATION_CLONE_DECL(ArrayDataInformation, DataInformationWithDummyChildren);

public:
    /** creates a new array with initial length @p length.
     *  takes ownership over @p childType
     *  length should be > 0
     */
    ArrayDataInformation(const QString& name, uint length,
                         std::unique_ptr<DataInformation>&& childType,
                         DataInformation* parent = nullptr, const QScriptValue& lengthFunction = QScriptValue());
    ~ArrayDataInformation() override;

public: // DataInformationWithDummyChildren API
    [[nodiscard]]
    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;

    [[nodiscard]]
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

    [[nodiscard]]
    Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const override;
    [[nodiscard]]
    QVariant childData(int row, int column, int role) const override;

    [[nodiscard]]
    QString childTypeName(uint index) const override;
    [[nodiscard]]
    QString childString(uint index) const override;
    [[nodiscard]]
    int indexOf(const DataInformation* const data) const override;
    [[nodiscard]]
    BitCount32 childSize(uint index) const override;

    [[nodiscard]]
    QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const override;

public: // DataInformation API
    [[nodiscard]]
    unsigned int childCount() const override;
    [[nodiscard]]
    DataInformation* childAt(unsigned int idx) const override;
    [[nodiscard]]
    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

    [[nodiscard]]
    QWidget* createEditWidget(QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;

    [[nodiscard]]
    BitCount32 size() const override;

    [[nodiscard]]
    qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;
    [[nodiscard]]
    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isArray() const override;

public:
    [[nodiscard]]
    uint length() const;
    [[nodiscard]]
    bool setArrayLength(uint newLength);
    /** Sets the new array type
     * @param newChildtype the new type (ownership is always taken, do not use anymore after this call!)
     */
    void setArrayType(std::unique_ptr<DataInformation>&& newChildtype);
    [[nodiscard]]
    DataInformation* arrayType() const;

    [[nodiscard]]
    QScriptValue childType() const;
    [[nodiscard]]
    QScriptValue lengthFunction() const;
    void setLengthFunction(const QScriptValue& newFunc);

private: // DataInformation API
    [[nodiscard]]
    QString typeNameImpl() const override;
    [[nodiscard]]
    QString valueStringImpl() const override;
    [[nodiscard]]
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;

private:
    std::unique_ptr<AbstractArrayData> mData;
    // Limit the length to 64K to avoid excessive memory consumption
    static constexpr uint MAX_LEN = 65535;
};

inline uint ArrayDataInformation::length() const
{
    return mData->length();
}

inline DataInformation* ArrayDataInformation::arrayType() const
{
    return mData->childType();
}

inline QScriptValue ArrayDataInformation::lengthFunction() const
{
    return mAdditionalData.get(AdditionalData::AdditionalDataType::ArrayLengthFunction).value<QScriptValue>();
}

inline void ArrayDataInformation::setLengthFunction(const QScriptValue& newFunc)
{
    setAdditionalFunction(AdditionalData::AdditionalDataType::ArrayLengthFunction, newFunc, "array length function");
}

#endif /* KASTEN_ARRAYDATAINFORMATION_HPP */
