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

DummyDataInformation::DummyDataInformation(DataInformationBase* parent)
    : DataInformation(QLatin1String("dummy"), parent)
{
}


//NONE OF THESE SHOULD EVER BE CALLED
BitCount32 DummyDataInformation::offset(unsigned int) const
{
    Q_ASSERT(false);
    return 0;
}

QScriptValue DummyDataInformation::toScriptValue(QScriptEngine*, ScriptHandlerInfo*)
{
    Q_ASSERT(false);
    return QScriptValue();
}

bool DummyDataInformation::setChildData(uint, const QVariant&, Okteta::AbstractByteArrayModel*,
        Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT(false);
    return false;
}

bool DummyDataInformation::setData(const QVariant&, Okteta::AbstractByteArrayModel*,
        Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT(false);
    return false;
}

qint64 DummyDataInformation::readData(Okteta::AbstractByteArrayModel*, Okteta::Address, BitCount64, quint8*)
{
    Q_ASSERT(false);
    return -1;
}

BitCount32 DummyDataInformation::size() const
{
    Q_ASSERT(false);
    return 0;
}

void DummyDataInformation::setWidgetData(QWidget*) const
{
    Q_ASSERT(false);
}

QVariant DummyDataInformation::dataFromWidget(const QWidget*) const
{
    Q_ASSERT(false);
    return QVariant();
}

QWidget* DummyDataInformation::createEditWidget(QWidget*) const
{
    Q_ASSERT(false);
    return 0;
}

QString DummyDataInformation::typeName() const
{
    Q_ASSERT(false);
    return QLatin1String("dummy");
}

DummyDataInformation* DummyDataInformation::clone() const
{
    Q_ASSERT(false);
    return 0;
}

BitCount32 DummyDataInformation::childSize(int) const
{
    Q_ASSERT(false);
    return 0;
}
