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

int SignedBitfieldDataInformation::displayBase() const
{
    int base = Kasten2::StructViewPreferences::unsignedDisplayBase();
    if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
    {
        return 2;
    }
    if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Decimal)
    {
        return 10;
    }
    if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
    {
        return 16;
    }
    return 10; //safe default value
}

QString SignedBitfieldDataInformation::valueString() const
{
    if (!mWasAbleToRead)
        return i18nc("invalid value (out of range)", "<invalid>");
    int base = displayBase();
    qint64 val = mValue.ulongValue & mask();

    //check if is negative (only when decimal):
    if (val & (1 << (width() - 1)) && base == 10)
    {
        //sign bit is set -> make value negative
        quint64 fill = 0xffffffff;
        fill <<= width();
        val |= fill;
    }
    QString num = QString::number(val, base);
    if (base == 16)
        num.prepend(QLatin1String("0x"));
    if (Kasten2::StructViewPreferences::localeAwareDecimalFormatting() && base == 10)
        num = KGlobal::locale()->formatNumber(num, false);
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
        spin->setValue(mValue.longValue & mask()); //& mask() not really necessary, just be on the safe side
}

QScriptValue SignedBitfieldDataInformation::valueAsQScriptValue() const
{
    // (|~mask()) makes first bits ones
    if (mValue.ulongValue & (1 << (size() - 1)))
    {
        //less than zero
        if (width() <= 32)
            return  qint32(mValue.intValue | quint32(~mask())); //32 bit or less -> can be put in as value
        else //have to save it as string since 64 bit values are not supported
            return QString::number(mValue.longValue | (~mask()));
    }
    else {
        if (width() <= 32)
            return  qint32(mValue.intValue & mask()); //32 bit or less -> can be put in as value
        else //have to save it as string since 64 bit values are not supported
            return QString::number(mValue.longValue & mask());
    }
}
