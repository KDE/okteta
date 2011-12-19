/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

#include "../../allprimitivetypes.h"
#include "../datainformation.h"
#include "abstractarraydata.h"

#include <QtScript/QScriptValue>

class DummyDataInformation;
class AbstractArrayData;
class ArrayDataInformation : public DataInformation
{
protected:
    explicit ArrayDataInformation(const ArrayDataInformation& d);
public:
    /** creates a new array with initial length @p length.
     *  takes ownership over @p childType
     *  length should be > 0
     */
    ArrayDataInformation(QString name, uint length, DataInformation* childType,
            DataInformation* parent = 0);
    virtual ~ArrayDataInformation();
    DATAINFORMATION_CLONE(Array)

public:
    virtual QString typeName() const;
    int length() const;
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
    virtual int indexOf(const DataInformation* const data) const;
    BitCount32 childSize(int index) const;

    virtual bool isArray() const;

    QScriptValue setArrayLength(int newLength, QScriptContext* context);
    QScriptValue setArrayType(QScriptValue type, QScriptContext* context);

    virtual QScriptValue childType() const;
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const;
private:
    /** Takes ownership of @p data ! */
    AbstractArrayData* arrayDataFromType(uint length, DataInformation* data);
    AbstractArrayData* primitiveArrayFromType(uint length, PrimitiveDataType type);
protected:
    virtual BitCount32 offset(unsigned int index) const;

    AbstractArrayData* mData;
    static const uint MAX_LEN = 10000;
};

inline int ArrayDataInformation::length() const
{
    if (!mData)
        return 0;
    return mData->length();
}

inline QString ArrayDataInformation::typeName() const
{
    if (!mData)
        return QString();
    return mData->typeName();
    //don't show name of child
    //return i18nc("array type then length", "%1[%2]", data->typeName(), childCount()); //TODO
}

inline BitCount32 ArrayDataInformation::size() const
{
    if (!mData)
        return 0;
    return mData->size();
}

inline bool ArrayDataInformation::isArray() const
{
    return true;
}

inline DataInformation* ArrayDataInformation::childAt(unsigned int idx) const
{
    if (!mData)
        return 0;
    return mData->childAt(idx);
}

inline unsigned int ArrayDataInformation::childCount() const
{
    if (!mData)
        return 0;
    return mData->length();
}

inline bool ArrayDataInformation::canHaveChildren() const
{
    return true;
}

inline int ArrayDataInformation::indexOf(const DataInformation*const data) const
{
    if (!mData)
        return 0;
    return mData->indexOf(data);
}

inline QScriptValue ArrayDataInformation::childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const
{
    if (!mData)
        return QScriptValue();
    return mData->toScriptValue(index, engine, handlerInfo);
}

inline BitCount32 ArrayDataInformation::childSize(int index) const
{
    if (!mData)
        return 0;
    return mData->sizeAt(index);
}

inline Qt::ItemFlags ArrayDataInformation::childFlags(int row, int column, bool fileLoaded) const
{
    if (!mData)
        return 0;
    return mData->childFlags(row, column, fileLoaded);
}

inline QWidget* ArrayDataInformation::createChildEditWidget(uint index, QWidget* parent) const
{
    if (!mData)
        return 0;
    return mData->createChildEditWidget(index, parent);
}

inline QVariant ArrayDataInformation::dataFromChildWidget(uint index, const QWidget* w) const
{
    if (!mData)
        return QVariant();
    return mData->dataFromChildWidget(index, w);
}

inline void ArrayDataInformation::setChildWidgetData(uint index, QWidget* w) const
{
    if (!mData)
        return;
    mData->setChildWidgetData(index, w);
}

#endif /* ARRAYDATAINFORMATION_H_ */
