/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "unsignedbitfielddatainformation.h"

#include <QScriptValue>

#include "../poddecoder/typeeditors/uintspinbox.h"
#include "../uintdatainformation.h"

QString UnsignedBitfieldDataInformation::valueString() const
{
    Q_ASSERT(mWasAbleToRead);
    return UIntDataInformationMethods<quint64>::staticValueString(mValue.ulongValue);
}

QWidget* UnsignedBitfieldDataInformation::createEditWidget(QWidget* parent) const
{
    UIntSpinBox* ret = new UIntSpinBox(parent);
    ret->setBase(PrimitiveDataInformation::unsignedDisplayBase());
    ret->setMaximum(mask());
    return ret;
}

QVariant UnsignedBitfieldDataInformation::dataFromWidget(const QWidget* w) const
{
    const UIntSpinBox* spin = dynamic_cast<const UIntSpinBox*> (w);
    if (spin)
        return spin->value();
    else
        return QVariant();
}

void UnsignedBitfieldDataInformation::setWidgetData(QWidget* w) const
{
    UIntSpinBox* spin = dynamic_cast<UIntSpinBox*> (w);
    if (spin)
        spin->setValue(mValue.ulongValue);
}

QScriptValue UnsignedBitfieldDataInformation::valueAsQScriptValue() const
{
    if (width() <= 32)
        return  mValue.uintValue & quint32(mask()); //32 bit or less -> can be put in as value
    else
        return QString::number(mValue.ulongValue);
}
