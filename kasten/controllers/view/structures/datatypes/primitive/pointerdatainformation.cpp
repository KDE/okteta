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
        : PrimitiveDataInformation(name, parent), mValue(valueType), mPointerTarget(childType)
{
    Q_CHECK_PTR(valueType);
    Q_CHECK_PTR(childType);

    //currently only absolute unsigned pointers are allowed
    const PrimitiveDataType pdt = mValue->type();
    Q_ASSERT(pdt == Type_UInt8 || pdt == Type_UInt16 || pdt == Type_UInt32 || pdt == Type_UInt64);
    Q_UNUSED(pdt)
    mValue->setParent(this);
    mPointerTarget->setParent(this);
}

PointerDataInformation::~PointerDataInformation()
{
}

PointerDataInformation::PointerDataInformation(const PointerDataInformation& d)
        : PrimitiveDataInformation(d), mValue(d.mValue->clone()), mPointerTarget(d.mPointerTarget->clone())
{
    mValue->setParent(this);
    mPointerTarget->setParent(this);
}

QScriptValue PointerDataInformation::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    QScriptValue ret = engine->newObject(handlerInfo->mPointerClass.data());
    ret.setData(engine->toScriptValue(static_cast<DataInformation*>(this)));
    return ret;
}

bool PointerDataInformation::setChildData(uint /*row*/, const QVariant& /*value*/, Okteta::AbstractByteArrayModel* /*out*/, Okteta::Address /*address*/, BitCount64 /*bitsRemaining*/, quint8 /*bitOffset*/)
{
    Q_ASSERT_X(false, "PointerDataInformation::setChildData()", "this should never be called!!");
    return false;
}

bool PointerDataInformation::setData(const QVariant& value, Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    return mValue->setData(value, out, address, bitsRemaining, bitOffset);
}

qint64 PointerDataInformation::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    qint64 retVal = mValue->readData(input, address, bitsRemaining, bitOffset);
    mWasAbleToRead = retVal >= 0; //not able to read if mValue->readData returns -1

    // If the pointer it's outside the boundaries of the input simply ignore it
    if (mValue->value().ulongValue < quint64(input->size()))
    {
        // Enqueue for later reading of the destination
        topLevelDataInformation()->enqueueReadData(this);
    }

    return retVal;
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
    return 0;
}

void PointerDataInformation::delayedReadData(Okteta::AbstractByteArrayModel *input, Okteta::Address address)
{
    Q_UNUSED(address); //TODO offsets
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
    topLevelDataInformation()->updateElement(mPointerTarget.data());
    // Let the child do the work
    mPointerTarget->readData(input, newAddress, (input->size() - newAddress) * 8, &childBitOffset);
}

BitCount32 PointerDataInformation::size() const
{
    return mValue->size();
}

void PointerDataInformation::setWidgetData(QWidget* w) const
{
    return mValue->setWidgetData(w);
}

QVariant PointerDataInformation::dataFromWidget(const QWidget* w) const
{
    return mValue->dataFromWidget(w);
}

QWidget* PointerDataInformation::createEditWidget(QWidget* parent) const
{
    return mValue->createEditWidget(parent);
}

QString PointerDataInformation::typeName() const
{
    return i18nc("memory pointer with underlying type", "%1 pointer", mValue->typeName());
}

QString PointerDataInformation::valueString() const
{
    return mValue->valueString();
}

Qt::ItemFlags PointerDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == (int)DataInformation::ColumnValue && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

uint PointerDataInformation::childCount() const
{
    return 1;
}

AllPrimitiveTypes PointerDataInformation::value() const
{
    return mValue->value();
}

void PointerDataInformation::setValue(AllPrimitiveTypes newValue)
{
    mValue->setValue(newValue);
}

DataInformation* PointerDataInformation::childAt(uint index) const
{
    Q_ASSERT(index == 0);
    return index == 0 ? mPointerTarget.data() : 0;
}

QScriptValue PointerDataInformation::valueAsQScriptValue() const
{
    return mValue->valueAsQScriptValue();
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
