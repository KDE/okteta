/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbitfielddatainformation.hpp"

#include <KLocalizedString>

#include "../../topleveldatainformation.hpp"

#include "../../../script/classes/bitfieldscriptclass.hpp"
#include "../../../script/scripthandlerinfo.hpp"

AbstractBitfieldDataInformation::AbstractBitfieldDataInformation(const QString& name, BitCount32 width,
                                                                 DataInformation* parent)
    : PrimitiveDataInformation(name, parent)
    , mWidth(qMin(width, 64u))
{
    Q_ASSERT(width <= 64);
}

AbstractBitfieldDataInformation::AbstractBitfieldDataInformation(const AbstractBitfieldDataInformation& d) = default;

AbstractBitfieldDataInformation::~AbstractBitfieldDataInformation() = default;

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
    Q_ASSERT((newVal.value<quint64>() & mask()) == newVal.value<quint64>());
    mValue = newVal.value<quint64>() & mask();
}

PrimitiveDataType AbstractBitfieldDataInformation::type() const
{
    return PrimitiveDataType::Bitfield;
}

qint64 AbstractBitfieldDataInformation::readData(Okteta::AbstractByteArrayModel* input,
                                                 Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); // update must have been called prior to reading
    if (bitsRemaining < BitCount64(width())) {
        mWasAbleToRead = false;
        mValue = 0;
        return -1;
    }
    bool wasValid = mWasAbleToRead;
    AllPrimitiveTypes oldVal(mValue);
    AllPrimitiveTypes newVal(mValue);

    mWasAbleToRead = newVal.readBits(size(), input, effectiveByteOrder(), address, bitsRemaining,
                                     bitOffset);

    if (oldVal != newVal || wasValid != mWasAbleToRead) {
        topLevelDataInformation()->setChildDataChanged();
        mValue = newVal;
    }
    return width();
}

bool AbstractBitfieldDataInformation::setData(const QVariant& valueVariant,
                                              Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining,
                                              quint8 bitOffset)
{
    AllPrimitiveTypes oldVal(mValue);
    bool ok;
    AllPrimitiveTypes valToWrite = valueVariant.toULongLong(&ok);
    Q_ASSERT(ok);
    if (!ok) {
        return false;
    }
    AllPrimitiveTypes newVal(oldVal);
    // this handles remaining < size() for us
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

BitCount32 AbstractBitfieldDataInformation::size() const
{
    return mWidth;
}

bool AbstractBitfieldDataInformation::isBitfield() const
{
    return true;
}

Qt::ItemFlags AbstractBitfieldDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == (int) DataInformation::ColumnValue && fileLoaded) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
