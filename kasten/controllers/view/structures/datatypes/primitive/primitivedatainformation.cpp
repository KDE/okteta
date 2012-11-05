/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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
#include "primitivedatainformation.h"
#include "../datainformation.h"
#include "../topleveldatainformation.h"

Qt::ItemFlags PrimitiveDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == (int) DataInformation::ColumnValue && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

// The inline destructor makes the compiler unhappy
PrimitiveDataInformation::~PrimitiveDataInformation()
{
}

PrimitiveDataInformationWrapper::PrimitiveDataInformationWrapper(const PrimitiveDataInformationWrapper& d)
        : PrimitiveDataInformation(d), mValue(d.mValue->clone())
{
    mValue->setParent(this);
}

PrimitiveDataInformationWrapper::PrimitiveDataInformationWrapper(const QString& name,
        PrimitiveDataInformation* valueType, DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(valueType)
{
    Q_CHECK_PTR(valueType);
    mValue->setParent(this);

}

QScriptValue PrimitiveDataInformationWrapper::valueAsQScriptValue() const
{
    return mValue->valueAsQScriptValue();
}

qint64 PrimitiveDataInformationWrapper::readData(Okteta::AbstractByteArrayModel* input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); //update must have been called prior to reading
    mValue->mHasBeenUpdated = true; //value does not get updated
    qint64 retVal = mValue->readData(input, address, bitsRemaining, bitOffset);
    mWasAbleToRead = mValue->wasAbleToRead();
    return retVal;
}
