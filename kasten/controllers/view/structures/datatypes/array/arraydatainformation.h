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
            DataInformation* parent = 0, const QScriptValue& lengthFuntion = QScriptValue());
    virtual ~ArrayDataInformation();
public:
    virtual QString typeName() const;
    uint length() const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;

    virtual QWidget* createChildEditWidget(uint index, QWidget* parent) const;
    virtual QVariant dataFromChildWidget(uint index, const QWidget* w) const;
    virtual void setChildWidgetData(uint index, QWidget* w) const;

    virtual BitCount32 size() const;

    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            BitCount64 bitsRemaining, quint8* bitOffset);
    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);
    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);

    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const;
    virtual QVariant childData(int row, int column, int role) const;
    virtual DataInformation* childAt(unsigned int idx) const;
    virtual unsigned int childCount() const;
    virtual bool canHaveChildren() const;
    virtual QString childTypeName(uint index) const;
    virtual int indexOf(const DataInformation* const data) const;
    BitCount32 childSize(uint index) const;

    virtual bool isArray() const;

    bool setArrayLength(uint newLength);
    /** Sets the new array type
     * @param newChildtype the new type (ownership is always taken, do not use anymore after this call!)
     */
    void setArrayType(DataInformation* newChildtype);
    DataInformation* arrayType() const;

    QScriptValue childType() const;
    QScriptValue lengthFunction() const;
    void setLengthFunction(QScriptValue newFunc);
    QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const;
    virtual BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const;
protected:
    virtual QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const;
private:
    /** Takes ownership of @p data ! */
    AbstractArrayData* arrayDataFromType(uint length, DataInformation* data);
    AbstractArrayData* primitiveArrayFromType(uint length, PrimitiveDataInformation* type);
protected:
    QScopedPointer<AbstractArrayData> mData;
    QScriptValue mLengthFunction;
    static const uint MAX_LEN = 10000;
};

inline uint ArrayDataInformation::length() const
{
    return mData->length();
}

inline QString ArrayDataInformation::typeName() const
{
    return mData->typeName();
    //don't show name of child
    //return i18nc("array type then length", "%1[%2]", data->typeName(), childCount()); //TODO
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
    return mLengthFunction;
}

inline void ArrayDataInformation::setLengthFunction(QScriptValue newFunc)
{
    if (!newFunc.isFunction())
        logError() << "New length function is not a function. It was: " << newFunc.toString();
    else
        mLengthFunction = newFunc;
}

#endif /* ARRAYDATAINFORMATION_H_ */
