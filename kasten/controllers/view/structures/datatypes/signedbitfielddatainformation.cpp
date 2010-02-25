/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
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

int SignedBitfieldDataInformation::displayBase() const
{
    int base = Kasten::StructViewPreferences::unsignedDisplayBase();
    if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
    {
        return 2;
    }
    if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Decimal)
    {
        return 10;
    }
    if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
    {
        return 16;
    }
    return 10; //safe default value
}

QString SignedBitfieldDataInformation::valueString() const
{
    //FIXME shift so that it is negative
    if (!mIsValid)
        return i18nc("invalid value (out of range)", "<invalid>");
    int base = displayBase();
    quint64 val = mValue.ulongValue & mask();
    QString num = QString::number(val, base);
    if (base == 16)
        num = "0x" + num;
    if (Kasten::StructViewPreferences::localeAwareDecimalFormatting() && base == 10)
        num = KGlobal::locale()->formatNumber(num, false, 0);
    return num;
}

QWidget* SignedBitfieldDataInformation::createEditWidget(QWidget* parent) const
{
    SIntSpinBox* ret = new SIntSpinBox(parent);
    ret->setBase(displayBase());
    ret->setRange(mask(), mask() >> 1); //mask is unsigned, so shift will do the right thing
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
        spin->setValue(this->value().longValue & mask()); //& mask() not really necessary, just be on the safe side
}

