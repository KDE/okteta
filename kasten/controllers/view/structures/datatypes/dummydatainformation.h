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

#ifndef DUMMYDATAINFORMATION_H
#define DUMMYDATAINFORMATION_H

#include "datainformation.h"

class DataInformationWithDummyChildren;

class DummyDataInformation : public DataInformation
{
    DATAINFORMATION_CLONE(DummyDataInformation, DataInformation), mIndex(d.mIndex) {}
public:
    DummyDataInformation(DataInformationBase* parent, const QString& name = QString());
    virtual ~DummyDataInformation() {}
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);

    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            BitCount64 bitsRemaining, quint8* bitOffset);
    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);

    virtual BitCount32 size() const;
    virtual void setWidgetData(QWidget* w) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    virtual QString typeName() const;
    virtual bool isDummy() const;
    virtual QVariant data(int column, int role) const;


    virtual bool canHaveChildren() const { return false; }
    virtual unsigned int childCount() const { return 0; }
    virtual DataInformation* childAt(unsigned int) const { Q_ASSERT(false); return 0; }
    virtual BitCount64 childPosition(const DataInformation*, Okteta::Address) const { Q_ASSERT(false); return 0; }
    virtual int indexOf(const DataInformation* const) const { Q_ASSERT(false); return -1; }


    inline void setDummyIndex(uint newIndex) { mIndex = newIndex; }
    inline uint dummyIndex() const { return mIndex; }

protected:
    virtual QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const { Q_ASSERT(false); return 0; }
private:
    uint mIndex;
    DataInformationWithDummyChildren* parentHelper() const;
};

/**
 * This class declares all methods that are needed if there can be dummy children
 */
class DataInformationWithDummyChildren : public DataInformation
{
protected:
    DataInformationWithDummyChildren(const DataInformationWithDummyChildren& d) : DataInformation(d) {}
public:
    explicit DataInformationWithDummyChildren(const QString& name, DataInformationBase* parent = 0)
            : DataInformation(name, parent) {}
    virtual ~DataInformationWithDummyChildren() {}

    /** the data of child at index @p row. Useful for arrays, or DataInformations with fake children*/
    virtual QVariant childData(int row, int column, int role) const = 0;
    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const = 0;
    virtual BitCount32 childSize(uint index) const = 0;
    /** create a QWidget for the QItemDelegate */
    virtual QWidget* createChildEditWidget(uint index, QWidget* parent) const = 0;
    /** get the needed data from the widget */
    virtual QVariant dataFromChildWidget(uint index, const QWidget* w) const = 0;
    /** initialize the delegate widget with the correct data */
    virtual void setChildWidgetData(uint index, QWidget* w) const = 0;
    virtual QString childTypeName(uint index) const = 0;
    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) = 0;
    virtual QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const = 0;
    virtual int indexOf(const DataInformation* const data) const;
};


inline int DataInformationWithDummyChildren::indexOf(const DataInformation* const data) const
{
    Q_ASSERT(data->isDummy());
    Q_ASSERT(data->parent() == this);
    return data->asDummy()->dummyIndex();
}

inline bool DummyDataInformation::isDummy() const
{
    return true;
}

#endif // DUMMYDATAINFORMATION_H
