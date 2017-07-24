/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef ARRAYDATAINFORMATION_H_
#define ARRAYDATAINFORMATION_H_

#include "../dummydatainformation.h"
#include "abstractarraydata.h"

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
    static const uint MAX_LEN = 10000;
};

inline uint ArrayDataInformation::length() const
{
    return mData->length();
}

inline QString ArrayDataInformation::typeNameImpl() const
{
    return mData->typeName();
}

inline BitCount32 ArrayDataInformation::size() const
{
    return mData->size();
}

inline bool ArrayDataInformation::isArray() const
{
    return true;
}

inline DataInformation* ArrayDataInformation::childAt(unsigned int idx) const
{
    return mData->childAt(idx);
}

inline unsigned int ArrayDataInformation::childCount() const
{
    return mData->length();
}

inline bool ArrayDataInformation::canHaveChildren() const
{
    return true;
}

inline int ArrayDataInformation::indexOf(const DataInformation*const data) const
{
    return mData->indexOf(data);
}

inline BitCount32 ArrayDataInformation::childSize(uint index) const
{
    return mData->sizeAt(index);
}

inline Qt::ItemFlags ArrayDataInformation::childFlags(int row, int column, bool fileLoaded) const
{
    return mData->childFlags(row, column, fileLoaded);
}

inline QWidget* ArrayDataInformation::createChildEditWidget(uint index, QWidget* parent) const
{
    return mData->createChildEditWidget(index, parent);
}

inline QVariant ArrayDataInformation::dataFromChildWidget(uint index, const QWidget* w) const
{
    return mData->dataFromChildWidget(index, w);
}

inline void ArrayDataInformation::setChildWidgetData(uint index, QWidget* w) const
{
    mData->setChildWidgetData(index, w);
}

inline QString ArrayDataInformation::childTypeName(uint index) const
{
    return mData->dataAt(index, DataInformation::ColumnType, Qt::DisplayRole).toString();
}

inline DataInformation* ArrayDataInformation::arrayType() const
{
    return mData->childType();
}

inline QScriptValue ArrayDataInformation::lengthFunction() const
{
    return mAdditionalData.get(AdditionalData::ArrayLengthFunction).value<QScriptValue>();
}

inline void ArrayDataInformation::setLengthFunction(const QScriptValue& newFunc)
{
    setAdditionalFunction(AdditionalData::ArrayLengthFunction, newFunc, "array length function");
}

#endif /* ARRAYDATAINFORMATION_H_ */
