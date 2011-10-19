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
#include "abstractbitfielddatainformation.h"

QString AbstractBitfieldDataInformation::sizeString() const
{
    return i18np("%1 bit", "%1 bits", width());
}

AllPrimitiveTypes AbstractBitfieldDataInformation::qVariantToAllPrimitiveTypes(
        const QVariant& value) const
{
    if (!value.isValid())
        kDebug() << "invalid QVariant passed.";

    //This is fine since all the values are unsigned
    return AllPrimitiveTypes(value.toULongLong());
}

QString AbstractBitfieldDataInformation::typeName() const
{
    return i18ncp("Data type", "bitfield (%1 bit wide)", "bitfield (%1 bits wide)", width());
}

AllPrimitiveTypes AbstractBitfieldDataInformation::value() const
{
    return mValue;
}

void AbstractBitfieldDataInformation::setValue(AllPrimitiveTypes newVal)
{
    mValue.ulongValue = newVal.ulongValue & mask();
}

PrimitiveDataType AbstractBitfieldDataInformation::type() const
{
    return Type_Bitfield;
}

AbstractBitfieldDataInformation::AbstractBitfieldDataInformation(QString name, uint width, DataInformation* parent) :
    PrimitiveDataInformation(name, parent), mWidth(width), mValue(0)
{
    if (width > 64)
    {
        kDebug() << "bitfield (" << name << ") width is greater 64: " << width << " , setting to 64";
        mWidth = 64;
    }
}

AbstractBitfieldDataInformation::~AbstractBitfieldDataInformation()
{
}

AbstractBitfieldDataInformation::AbstractBitfieldDataInformation(const AbstractBitfieldDataInformation& d) :
    PrimitiveDataInformation(d), mWidth(d.mWidth), mValue(d.mValue)
{
}
