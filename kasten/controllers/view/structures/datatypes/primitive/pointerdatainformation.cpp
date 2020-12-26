/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alessandro Di Federico <ale@clearmind.me>
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardosn@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "pointerdatainformation.hpp"
#include "../topleveldatainformation.hpp"
#include "../primitivedatatype.hpp"
#include "../../allprimitivetypes.hpp"
#include "../../script/scripthandlerinfo.hpp"
#include "../../script/classes/pointerscriptclass.hpp"
#include "../../script/scriptlogger.hpp"
#include "../../parsers/parserutils.hpp"

#include <QScriptEngine>
#include <KLocalizedString>

#include <limits>

PointerDataInformation::PointerDataInformation(const QString& name, DataInformation* childType,
                                               PrimitiveDataInformation* valueType, DataInformation* parent,
                                               qint64 pointerScale, const QScriptValue& interpretFunction)
    : PrimitiveDataInformationWrapper(name, valueType, parent)
    , mPointerTarget(childType), mPointerScale(pointerScale)
{
    Q_CHECK_PTR(childType);

    if (interpretFunction.isValid() && interpretFunction.isFunction()) {
        setInterpreterFunction(interpretFunction);
    }

    // currently only absolute unsigned pointers are allowed
    const PrimitiveDataType pdt = mValue->type();
    Q_ASSERT(pdt == PrimitiveDataType::UInt8 || pdt == PrimitiveDataType::UInt16 || pdt == PrimitiveDataType::UInt32 || pdt == PrimitiveDataType::UInt64);
    Q_UNUSED(pdt)
    mPointerTarget->setParent(this);
}

PointerDataInformation::PointerDataInformation(const PointerDataInformation& d)
    : PrimitiveDataInformationWrapper(d)
    , mPointerTarget(d.mPointerTarget->clone()), mPointerScale(d.mPointerScale)
{
    mPointerTarget->setParent(this);
    if (d.interpreterFunction().isValid() &&
        d.interpreterFunction().isFunction()) {
        setInterpreterFunction(d.interpreterFunction());
    }
}

PointerDataInformation::~PointerDataInformation() = default;

qint64 PointerDataInformation::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                                        BitCount64 bitsRemaining, quint8* bitOffset)
{
    qint64 ret = PrimitiveDataInformationWrapper::readData(input, address, bitsRemaining, bitOffset);
    if (!mWasAbleToRead) {
        mPointerTarget->mWasAbleToRead = false;
    }
    // If the pointer is outside the boundaries of the input simply ignore it
    else if (interpret(address) < quint64(input->size())) {
        // Enqueue for later reading of the destination
        topLevelDataInformation()->enqueueReadData(this);
    }
    return ret;
}

BitCount64 PointerDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    // TODO other pointer modes
    Q_ASSERT(child == mPointerTarget.data());
    Q_UNUSED(child);

    return mWasAbleToRead ? interpret(start) * 8 : 0;
}

int PointerDataInformation::indexOf(const DataInformation* const data) const
{
    Q_ASSERT(data == mPointerTarget.data());
    return data == mPointerTarget.data() ? 0 : -1;
}

void PointerDataInformation::delayedReadData(Okteta::AbstractByteArrayModel* input, Okteta::Address address)
{
    Q_ASSERT(mHasBeenUpdated); // update must have been called prior to reading
    Q_ASSERT(mWasAbleToRead);
    quint8 childBitOffset = 0;
    // Compute the destination offset
    const quint64 pointer = interpret(address);
    if (pointer > quint64(std::numeric_limits<Okteta::Address>::max())) {
        logError() << "Pointer" << mValue->valueString() << "does not point to an existing address.";
        return;
    }
    Okteta::Address newAddress(pointer);
    // If the computed destination it's outside the boundaries of the input ignore it
    if (newAddress < 0 || newAddress >= input->size()) {
        logError() << "Pointer" << mValue->valueString() << "does not point to an existing address.";
        return;
    }
    // update the child now
    DataInformation* oldTarget = mPointerTarget.data();
    topLevelDataInformation()->scriptHandler()->updateDataInformation(mPointerTarget.data());
    // Let the child do the work (maybe different than before now)
    if (mPointerTarget.data() != oldTarget) {
        logInfo() << "Pointer target was replaced.";
        topLevelDataInformation()->setChildDataChanged();
    }
    mPointerTarget->readData(input, newAddress, BitCount64(input->size() - newAddress) * 8, &childBitOffset);
}

QString PointerDataInformation::typeNameImpl() const
{
    return i18nc("memory pointer with underlying type", "%1 pointer", mValue->typeName());
}

QString PointerDataInformation::valueStringImpl() const
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
    return index == 0 ? mPointerTarget.data() : nullptr;
}

bool PointerDataInformation::setPointerType(DataInformation* type)
{
    Q_CHECK_PTR(type);
    if (!type->isPrimitive()) {
        logError() << "New pointer type is not primitive!";
        return false;
    }
    PrimitiveDataInformation* prim = type->asPrimitive();
    const PrimitiveDataType pdt = prim->type();
    if (pdt == PrimitiveDataType::UInt8 || pdt == PrimitiveDataType::UInt16 || pdt == PrimitiveDataType::UInt32 || pdt == PrimitiveDataType::UInt64) {
        mValue.reset(prim);
        mValue->setParent(this);
        return true;
    }

    logError() << "New pointer type is not an unsigned integer: " << pdt;
    return false;
}

QScriptClass* PointerDataInformation::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mPointerClass.data();
}

bool PointerDataInformation::canHaveChildren() const
{
    return true;
}

bool PointerDataInformation::isPointer() const
{
    return true;
}

quint64 PointerDataInformation::interpret(Okteta::Address start) const
{
    Q_UNUSED(start) // TODO offsets
    const QScriptValue& interpretFunc = interpreterFunction();
    if (!interpretFunc.isValid()) {
        return mValue->value().value<quint64>() * (quint64)pointerScale();
    }

    Q_ASSERT(interpretFunc.isFunction());
    ScriptHandler* handler = topLevelDataInformation()->scriptHandler();
    QScriptValue result = handler->callFunction(interpretFunc,
                                                const_cast<DataInformation*>(asDataInformation()),
                                                ScriptHandlerInfo::Mode::InterpretingPointer);
    if (result.isError()) {
        logError() << "interpretFunc caused an error:" << result.toString();
        return 0;
    }

    ParsedNumber<quint64> value = ParserUtils::uint64FromScriptValue(result);
    if (!value.isValid) {
        logError() << "Pointer interpreter function did not return a valid number! Result was: " << result.toString();
        return 0;
    } else {
        return value.value;
    }
}
