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

#include "dummydatainformation.h"

#include <QScriptValue>
#include <QVariant>

DummyDataInformation::DummyDataInformation(DataInformationBase* parent, const QString& name)
    : DataInformation(name, parent), mIndex(0)
{
}

inline DataInformationWithDummyChildren* DummyDataInformation::parentHelper() const
{
    DataInformationWithDummyChildren* parent = dynamic_cast<DataInformationWithDummyChildren*>(mParent);
    Q_CHECK_PTR(parent);
    return parent;
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

qint64 DummyDataInformation::readData(Okteta::AbstractByteArrayModel* in, Okteta::Address addr,
        BitCount64 remaining, quint8* offset)
{
    Q_ASSERT(false); //should never be called
    return -1;
}

BitCount32 DummyDataInformation::size() const
{
    return parentHelper()->childSize(mIndex);
}

void DummyDataInformation::setWidgetData(QWidget* w) const
{
    return parentHelper()->setChildWidgetData(mIndex, w);
}

QVariant DummyDataInformation::dataFromWidget(const QWidget* w) const
{
    return parentHelper()->dataFromChildWidget(mIndex, w);
}

QWidget* DummyDataInformation::createEditWidget(QWidget* parent) const
{
    return parentHelper()->createChildEditWidget(mIndex, parent);
}

QString DummyDataInformation::typeName() const
{
    return parentHelper()->childTypeName(mIndex);
}
