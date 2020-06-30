/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "dummydatainformation.hpp"

#include <QScriptValue>
#include <QVariant>

DummyDataInformation::DummyDataInformation(DataInformationBase* parent, const QString& name)
    : DataInformation(name, parent)
{
}

DataInformationWithDummyChildren* DummyDataInformation::parentHelper() const
{
    DataInformationWithDummyChildren* parent = mParent->asDataInformationWithDummyChildren();
    Q_CHECK_PTR(parent);
    return parent;
}

int DataInformationWithDummyChildren::indexOf(const DataInformation* const data) const
{
    Q_ASSERT(data->isDummy());
    Q_ASSERT(data->parent() == this);
    return data->asDummy()->dummyIndex();
}

bool DataInformationWithDummyChildren::isDataInformationWithDummyChildren() const
{
    return true;
}

QVariant DummyDataInformation::data(int column, int role) const
{
    return parentHelper()->childData(mIndex, column, role);
}

Qt::ItemFlags DummyDataInformation::flags(int column, bool fileLoaded) const
{
    return parentHelper()->childFlags(mIndex, column, fileLoaded);
}

QScriptValue DummyDataInformation::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* info)
{
    return parentHelper()->childToScriptValue(mIndex, engine, info);
}

bool DummyDataInformation::setData(const QVariant& v, Okteta::AbstractByteArrayModel* out,
                                   Okteta::Address addr, BitCount64 remaining, quint8 offset)
{
    return parentHelper()->setChildData(mIndex, v, out, addr, remaining, offset);
}

qint64 DummyDataInformation::readData(Okteta::AbstractByteArrayModel*, Okteta::Address,
                                      BitCount64, quint8*)
{
    Q_ASSERT(false); // should never be called
    return -1;
}

BitCount32 DummyDataInformation::size() const
{
    return parentHelper()->childSize(mIndex);
}

void DummyDataInformation::setWidgetData(QWidget* w) const
{
    parentHelper()->setChildWidgetData(mIndex, w);
}

QVariant DummyDataInformation::dataFromWidget(const QWidget* w) const
{
    return parentHelper()->dataFromChildWidget(mIndex, w);
}

QWidget* DummyDataInformation::createEditWidget(QWidget* parent) const
{
    return parentHelper()->createChildEditWidget(mIndex, parent);
}

QString DummyDataInformation::typeNameImpl() const
{
    return parentHelper()->childTypeName(mIndex);
}

bool DummyDataInformation::isDummy() const
{
    return true;
}
