/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "int32datainformation.h"

QString Int32DataInformation::valueString() const
{
    if (!mWasAbleToRead)
        return i18nc("invalid value (out of range)", "<invalid>");
    int base = displayBase();
    QString num;
    if (base != 10)
        num = QString::number((unsigned) mValue, base);
    else
        num = QString::number(mValue, base);
    //no need to correct, since int is big enough not to have leading useless chars
    if (base == 16)
        num = "0x" + num;
    if (Kasten::StructViewPreferences::localeAwareDecimalFormatting() && base == 10)
        num = KGlobal::locale()->formatNumber(num, false, 0);
    return num;
}

void Int32DataInformation::setWidgetData(QWidget* w) const
{
    SIntSpinBox* spin = dynamic_cast<SIntSpinBox*> (w);
    if (spin)
        spin->setValue(mValue);
}

QWidget* Int32DataInformation::createEditWidget(QWidget* parent) const
{
    SInt32Editor* ret = new SInt32Editor(parent);
    ret->setBase(displayBase());
    return ret;
}

AllPrimitiveTypes Int32DataInformation::value() const
{
    return AllPrimitiveTypes(mValue);
}

void Int32DataInformation::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.intValue;
}
