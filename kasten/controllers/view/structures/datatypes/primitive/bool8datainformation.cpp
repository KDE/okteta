/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
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
#include "bool8datainformation.h"

QString Bool8DataInformation::valueString() const
{
    if (!mWasAbleToRead)
        return i18nc("invalid value (out of range)", "<invalid>");
    int base = displayBase();
    if (mValue == 0)
        return i18nc("boolean value", "false");
    else if (mValue == 1)
        return i18nc("boolean value", "true");
    else
    {
        QString num = QString::number(mValue, base);
        if (base == 16)
            num = "0x" + num;
        if (Kasten::StructViewPreferences::localeAwareDecimalFormatting() && base
                == 10)
            num = KGlobal::locale()->formatNumber(num, false, 0);
        return i18nc("boolean value with actual value", "true (%1)", num);
    }
}

QWidget* Bool8DataInformation::createEditWidget(QWidget* parent) const
{
    UInt8Editor* ret = new UInt8Editor(parent);
    ret->setBase(displayBase());
    return ret;
}

AllPrimitiveTypes Bool8DataInformation::value() const
{
    return AllPrimitiveTypes(mValue);
}

void Bool8DataInformation::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.ubyteValue;
}
