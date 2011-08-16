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
    virtual quint64 offset(unsigned int index) const;
    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            quint64 bitsRemaining, quint8* bitOffset);
    virtual bool setData(const QVariant& value, DataInformation* inf, Okteta::AbstractByteArrayModel* input,
            Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset);
    virtual int size() const;

    virtual QVariant childData(int row, int column, int role) const;
    virtual DataInformation* childAt(unsigned int idx) const;
    virtual unsigned int childCount() const;
    virtual bool canHaveChildren() const;
    virtual int indexOf(const DataInformation* const data) const;

    virtual bool isArray() const;

    QScriptValue setArrayLength(int newLength, QScriptContext* context);
    QScriptValue setArrayType(QScriptValue type, QScriptContext* context);

    virtual QScriptValue childType() const;
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const;

protected:
    AbstractArrayData* mData;
    static const uint MAX_LEN = 10000;
};

inline int ArrayDataInformation::length() const
{
    return mData->length();
}

inline QString ArrayDataInformation::typeName() const
{
    return mData->typeName();
    //don't show name of child
    //return i18nc("array type then length", "%1[%2]", data->typeName(), childCount()); //TODO
}

inline int ArrayDataInformation::size() const
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

inline QScriptValue ArrayDataInformation::childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const
{
    return mData->toScriptValue(index, engine, handlerInfo);
}



#endif /* ARRAYDATAINFORMATION_H_ */
