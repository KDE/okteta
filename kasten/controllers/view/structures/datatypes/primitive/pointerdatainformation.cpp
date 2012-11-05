/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alessandro Di Federico <ale@clearmind.me>
 *   Copyright 2012 Alex Richardson <alex.richardosn@gmx.de>
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

#include "pointerdatainformation.h"
#include "../topleveldatainformation.h"
#include "../primitivedatatype.h"
#include "../../allprimitivetypes.h"
#include "../../script/scripthandlerinfo.h"
#include "../../script/classes/pointerscriptclass.h"
#include "../../script/scriptlogger.h"

#include <QScriptEngine>
#include <KLocalizedString>

#include <limits>

PointerDataInformation::PointerDataInformation(QString name, DataInformation* childType,
        PrimitiveDataInformation* valueType, DataInformation* parent)
        : PrimitiveDataInformationWrapper(name, valueType, parent), mPointerTarget(childType)
{
    Q_CHECK_PTR(childType);

    //currently only absolute unsigned pointers are allowed
    const PrimitiveDataType pdt = mValue->type();
    Q_ASSERT(pdt == Type_UInt8 || pdt == Type_UInt16 || pdt == Type_UInt32 || pdt == Type_UInt64);
    Q_UNUSED(pdt)
    mPointerTarget->setParent(this);
}

PointerDataInformation::~PointerDataInformation()
{
}

PointerDataInformation::PointerDataInformation(const PointerDataInformation& d)
        : PrimitiveDataInformationWrapper(d), mPointerTarget(d.mPointerTarget->clone())
{
    mPointerTarget->setParent(this);
}

qint64 PointerDataInformation::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        BitCount64 bitsRemaining, quint8* bitOffset)
{
    qint64 ret = PrimitiveDataInformationWrapper::readData(input, address, bitsRemaining, bitOffset);
    if (!mWasAbleToRead)
    {
        mPointerTarget->mWasAbleToRead = false;
    }
    // If the pointer it's outside the boundaries of the input simply ignore it
    else if (mValue->value().ulongValue < quint64(input->size()))
    {
        // Enqueue for later reading of the destination
        topLevelDataInformation()->enqueueReadData(this);
    }
    return ret;
}

BitCount64 PointerDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    //TODO other pointer modes
    Q_ASSERT(child == mPointerTarget.data());
    return mWasAbleToRead ? mValue->value().ulongValue * 8 : 0;
}

int PointerDataInformation::indexOf(const DataInformation* const data) const
{
    Q_ASSERT(data == mPointerTarget.data());
    return data == mPointerTarget.data() ? 0 : -1;
}

void PointerDataInformation::delayedReadData(Okteta::AbstractByteArrayModel *input, Okteta::Address address)
{
    Q_UNUSED(address); //TODO offsets
    Q_ASSERT(mHasBeenUpdated); //update must have been called prior to reading
    Q_ASSERT(mWasAbleToRead);
    quint8 childBitOffset = 0;
    // Compute the destination offset
    const quint64 pointer = mValue->value().ulongValue;
    if (pointer > quint64(std::numeric_limits<Okteta::Address>::max()))
    {
        logError() << "Pointer" << mValue->valueString() << "does not point to an existing address.";
        return;
    }
    Okteta::Address newAddress(pointer);
    // If the computed destination it's outside the boundaries of the input ignore it
    if (newAddress < 0 || newAddress >= input->size())
    {
        logError() << "Pointer" << mValue->valueString() << "does not point to an existing address.";
        return;
    }
    //update the child now
    DataInformation* oldTarget = mPointerTarget.data();
    topLevelDataInformation()->scriptHandler()->updateDataInformation(mPointerTarget.data());
    // Let the child do the work (maybe different than before now)
    if (mPointerTarget.data() != oldTarget)
    {
        logInfo() << "Pointer target was replaced.";
        topLevelDataInformation()->setChildDataChanged();
    }
    mPointerTarget->readData(input, newAddress, (input->size() - newAddress) * 8, &childBitOffset);
}

QString PointerDataInformation::typeName() const
{
    return i18nc("memory pointer with underlying type", "%1 pointer", mValue->typeName());
}

QString PointerDataInformation::valueString() const
{
    Q_ASSERT(mWasAbleToRead);
    return mValue->valueString();
}

uint PointerDataInformation::childCount() const
{
    return 1;
}

DataInformation* PointerDataInformation::childAt(uint index) const
{
    Q_ASSERT(index == 0);
    return index == 0 ? mPointerTarget.data() : 0;
}

bool PointerDataInformation::setPointerType(DataInformation* type)
{
    Q_CHECK_PTR(type);
    if (!type->isPrimitive())
    {
        logError() << "New pointer type is not primitive!";
        return false;
    }
    PrimitiveDataInformation* prim = type->asPrimitive();
    const PrimitiveDataType pdt = prim->type();
    if (pdt == Type_UInt8 || pdt == Type_UInt16 || pdt == Type_UInt32 || pdt == Type_UInt64)
    {
        mValue.reset(prim);
        mValue->setParent(this);
        return true;
    }
    else
    {
        logError() << "New pointer type is not an unsigned integer: " << pdt;
        return false;
    }
}

QScriptClass* PointerDataInformation::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mPointerClass.data();
}
