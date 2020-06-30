/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ARRAYDATAINFORMATION_HPP
#define KASTEN_ARRAYDATAINFORMATION_HPP

#include "../dummydatainformation.hpp"
#include "abstractarraydata.hpp"

#include <QVariant>
#include <QScriptValue>

class DummyDataInformation;
class AbstractArrayData;
class PrimitiveDataInformation;

class ArrayDataInformation : public DataInformationWithDummyChildren
{
    friend class PrimitiveArrayTest;
    DATAINFORMATION_CLONE_DECL(ArrayDataInformation, DataInformationWithDummyChildren);

public:
    /** creates a new array with initial length @p length.
     *  takes ownership over @p childType
     *  length should be > 0
     */
    ArrayDataInformation(const QString& name, uint length, DataInformation* childType,
                         DataInformation* parent = nullptr, const QScriptValue& lengthFuntion = QScriptValue());
    ~ArrayDataInformation() override;

public:
    uint length() const;
    QWidget* createEditWidget(QWidget* parent) const override;
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;

    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;

    BitCount32 size() const override;

    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;
    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

    Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const override;
    QVariant childData(int row, int column, int role) const override;
    DataInformation* childAt(unsigned int idx) const override;
    unsigned int childCount() const override;
    bool canHaveChildren() const override;
    QString childTypeName(uint index) const override;
    int indexOf(const DataInformation* const data) const override;
    BitCount32 childSize(uint index) const override;

    bool isArray() const override;

    bool setArrayLength(uint newLength);
    /** Sets the new array type
     * @param newChildtype the new type (ownership is always taken, do not use anymore after this call!)
     */
    void setArrayType(DataInformation* newChildtype);
    DataInformation* arrayType() const;

    QScriptValue childType() const;
    QScriptValue lengthFunction() const;
    void setLengthFunction(const QScriptValue& newFunc);
    QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const override;
    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

private:
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;
    QString typeNameImpl() const override;

private:
    QScopedPointer<AbstractArrayData> mData;
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
