/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "primitivedatainformation.hpp"
#include "../datainformation.hpp"
#include "../topleveldatainformation.hpp"
#include "structureviewpreferences.h"

PrimitiveDataInformationWrapper::PrimitiveDataInformationWrapper(const PrimitiveDataInformationWrapper& d)
    : PrimitiveDataInformation(d)
    , mValue(d.mValue->clone())
{
    mValue->setParent(this);
}

PrimitiveDataInformationWrapper::PrimitiveDataInformationWrapper(const QString& name,
                                                                 PrimitiveDataInformation* valueType, DataInformation* parent)
    : PrimitiveDataInformation(name, parent)
    , mValue(valueType)
{
    Q_CHECK_PTR(valueType);
    mValue->setParent(this);

}

// The inline destructor makes the compiler unhappy
PrimitiveDataInformation::~PrimitiveDataInformation() = default;

Qt::ItemFlags PrimitiveDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == (int) DataInformation::ColumnValue && fileLoaded) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool PrimitiveDataInformation::isPrimitive() const
{
    return true;
}

unsigned int PrimitiveDataInformation::childCount() const
{
    return 0;
}

DataInformation* PrimitiveDataInformation::childAt(unsigned int) const
{
    Q_ASSERT(false);
    return nullptr;
}

bool PrimitiveDataInformation::canHaveChildren() const
{
    return false;
}

BitCount64 PrimitiveDataInformation::childPosition(const DataInformation*, Okteta::Address) const
{
    Q_ASSERT(false);
    return 0;
}

int PrimitiveDataInformation::indexOf(const DataInformation* const) const
{
    Q_ASSERT(false);
    return -1;
}

QScriptValue PrimitiveDataInformationWrapper::valueAsQScriptValue() const
{
    return mValue->valueAsQScriptValue();
}

qint64 PrimitiveDataInformationWrapper::readData(Okteta::AbstractByteArrayModel* input,
                                                 Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); // update must have been called prior to reading
    mValue->mHasBeenUpdated = true; // value does not get updated
    qint64 retVal = mValue->readData(input, address, bitsRemaining, bitOffset);
    mWasAbleToRead = mValue->wasAbleToRead();
    return retVal;
}

BitCount32 PrimitiveDataInformation::offset(unsigned int index) const
{
    Q_UNUSED(index)
    Q_ASSERT_X(false, "PrimitiveDataInformation::offset", "This should never be called");
    return 0;
}

bool PrimitiveDataInformationWrapper::setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                                              Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    return mValue->setData(value, out, address, bitsRemaining, bitOffset);
}

BitCount32 PrimitiveDataInformationWrapper::size() const
{
    return mValue->size();
}

void PrimitiveDataInformationWrapper::setWidgetData(QWidget* w) const
{
    mValue->setWidgetData(w);
}

QVariant PrimitiveDataInformationWrapper::dataFromWidget(const QWidget* w) const
{
    return mValue->dataFromWidget(w);
}

QWidget* PrimitiveDataInformationWrapper::createEditWidget(QWidget* parent) const
{
    return mValue->createEditWidget(parent);
}

AllPrimitiveTypes PrimitiveDataInformationWrapper::value() const
{
    return mValue->value();
}

void PrimitiveDataInformationWrapper::setValue(AllPrimitiveTypes newValue)
{
    mValue->setValue(newValue);
}

// classes derived from this are not true primitive types (they provide additional information)
PrimitiveDataType PrimitiveDataInformationWrapper::type() const
{
    return PrimitiveDataType::Invalid;
}
