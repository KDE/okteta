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
    explicit DummyDataInformation(DataInformationBase* parent, const QString& name = QString());
    ~DummyDataInformation() override = default;
    QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) override;

    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;
    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

    BitCount32 size() const override;
    void setWidgetData(QWidget* w) const override;
    QVariant dataFromWidget(const QWidget* w) const override;
    QWidget* createEditWidget(QWidget* parent) const override;
    Qt::ItemFlags flags(int column, bool fileLoaded = true) const override;
    bool isDummy() const override;
    QVariant data(int column, int role) const override;

    bool canHaveChildren() const override { return false; }
    unsigned int childCount() const override { return 0; }
    DataInformation* childAt(unsigned int) const override { Q_ASSERT(false); return nullptr; }
    BitCount64 childPosition(const DataInformation*, Okteta::Address) const override { Q_ASSERT(false); return 0; }
    int indexOf(const DataInformation* const) const override { Q_ASSERT(false); return -1; }

    inline void setDummyIndex(uint newIndex) { mIndex = newIndex; }
    inline uint dummyIndex() const { return mIndex; }

private:
    QScriptClass* scriptClass(ScriptHandlerInfo*) const override { Q_ASSERT(false); return nullptr; }
    QString typeNameImpl() const override;

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
    DataInformationWithDummyChildren(const DataInformationWithDummyChildren& d) = default;

public:
    explicit DataInformationWithDummyChildren(const QString& name, DataInformationBase* parent = nullptr)
        : DataInformation(name, parent)
    {}
    ~DataInformationWithDummyChildren() override = default;

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
    int indexOf(const DataInformation* const data) const override;
    bool isDataInformationWithDummyChildren() const override;
};

#endif // DUMMYDATAINFORMATION_H
