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
#include "signedbitfielddatainformation.h"

#include <QScriptValue>

#include "../poddecoder/typeeditors/sintspinbox.h"
#include "../sintdatainformation.h"


QString SignedBitfieldDataInformation::valueString() const
{
    Q_ASSERT(mWasAbleToRead);
    return SIntDataInformationMethods<qint64>::staticValueString(mValue.longValue);
}

QWidget* SignedBitfieldDataInformation::createEditWidget(QWidget* parent) const
{
    SIntSpinBox* ret = new SIntSpinBox(parent);
    ret->setBase(PrimitiveDataInformation::signedDisplayBase());
    ret->setRange(~mask(), mask() >> 1); //mask is unsigned, so shift will do the right thing
    return ret;
}

QVariant SignedBitfieldDataInformation::dataFromWidget(const QWidget* w) const
{
    const SIntSpinBox* spin = dynamic_cast<const SIntSpinBox*> (w);
    if (spin)
        return spin->value();
    else
        return QVariant();
}

void SignedBitfieldDataInformation::setWidgetData(QWidget* w) const
{
    SIntSpinBox* spin = dynamic_cast<SIntSpinBox*> (w);
    if (spin)
        spin->setValue(mValue.longValue);
}

QScriptValue SignedBitfieldDataInformation::valueAsQScriptValue() const
{
    if (width() <= 32)
        return  qint32(mValue.intValue); //32 bit or less -> can be put in as value
    else //have to save it as string since 64 bit values are not supported
        return QString::number(mValue.longValue);
}

void SignedBitfieldDataInformation::setValue(AllPrimitiveTypes newVal)
{
    //check that values are not too large
    Q_ASSERT((newVal.longValue < 0 && (newVal.longValue | ~mask()) == newVal.ulongValue)
        || (newVal.ulongValue & mask()) == newVal.ulongValue);
    mValue = newVal.ulongValue & mask();
    //check if MSB is set -> negative -> sign extend
    if (newVal.ulongValue & (quint64(1) << (width() - 1))) {
        mValue = mValue.ulongValue | (~mask());
    }
}

AllPrimitiveTypes SignedBitfieldDataInformation::fromVariant(const QVariant& variant, bool* ok) const
{
    return AllPrimitiveTypes(variant.toLongLong(ok));
}
