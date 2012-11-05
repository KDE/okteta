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

#include <KLocalizedString>

#include "../../topleveldatainformation.h"

#include "../../../script/classes/bitfieldscriptclass.h"
#include "../../../script/scripthandlerinfo.h"

QString AbstractBitfieldDataInformation::sizeString() const
{
    return i18np("%1 bit", "%1 bits", width());
}

AllPrimitiveTypes AbstractBitfieldDataInformation::value() const
{
    return mValue;
}

void AbstractBitfieldDataInformation::setValue(AllPrimitiveTypes newVal)
{
    Q_ASSERT((newVal.ulongValue & mask()) == newVal.ulongValue);
    mValue.ulongValue = newVal.ulongValue & mask();
}

PrimitiveDataType AbstractBitfieldDataInformation::type() const
{
    return Type_Bitfield;
}

AbstractBitfieldDataInformation::AbstractBitfieldDataInformation(const QString& name, BitCount32 width,
        DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(0), mWidth(qMin(width, 64u))
{
    Q_ASSERT(width <= 64);
}

AbstractBitfieldDataInformation::~AbstractBitfieldDataInformation()
{
}

AbstractBitfieldDataInformation::AbstractBitfieldDataInformation(const AbstractBitfieldDataInformation& d)
        : PrimitiveDataInformation(d), mValue(d.mValue), mWidth(d.mWidth)
{
}

qint64 AbstractBitfieldDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); //update must have been called prior to reading
    if (bitsRemaining < BitCount64(width()))
    {
        mWasAbleToRead = false;
        mValue = 0;
        return -1;
    }
    bool wasValid = mWasAbleToRead;
    AllPrimitiveTypes oldVal(mValue);
    AllPrimitiveTypes newVal(mValue);

    mWasAbleToRead = newVal.readBits(size(), input, effectiveByteOrder(), address, bitsRemaining,
            bitOffset);

    if (oldVal != newVal || wasValid != mWasAbleToRead)
    {
        topLevelDataInformation()->setChildDataChanged();
        mValue = newVal;
    }
    return width();
}

bool AbstractBitfieldDataInformation::setData(const QVariant& valueVariant,
        Okteta::AbstractByteArrayModel *out, Okteta::Address address, BitCount64 bitsRemaining,
        quint8 bitOffset)
{
    AllPrimitiveTypes oldVal(mValue);
    bool ok;
    AllPrimitiveTypes valToWrite = valueVariant.toULongLong(&ok);
    Q_ASSERT(ok);
    if (!ok)
        return false;
    AllPrimitiveTypes newVal(oldVal);
    //this handles remaining < size() for us
    bool wasAbleToWrite = newVal.writeBits(width(), valToWrite, out, effectiveByteOrder(), address,
            bitsRemaining, &bitOffset);
    return wasAbleToWrite;
}

AllPrimitiveTypes AbstractBitfieldDataInformation::fromVariant(const QVariant& variant, bool* ok) const
{
    return AllPrimitiveTypes(variant.toULongLong(ok));
}

QScriptClass* AbstractBitfieldDataInformation::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mBitfieldClass.data();
}
