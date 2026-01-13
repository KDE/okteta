/*
    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "arraydatainformation.hpp"

// lib
#include "complexarraydata.hpp"
#include "primitivearraydata.hpp"
#include <primitivefactory.hpp>
#include <dummydatainformation.hpp>
#include <scriptvalueconverter.hpp>
#include <arrayscriptclass.hpp>
#include <scripthandlerinfo.hpp>
#include <scriptlogger.hpp>
// Std
#include <utility>

ArrayDataInformation::ArrayDataInformation(const QString& name, uint length,
                                           std::unique_ptr<DataInformation>&& childType,
                                           const LoggerWithContext& logger,
                                           DataInformation* parent, const QScriptValue& lengthFunction)
    : DataInformationWithDummyChildren(name, parent)
{
    if (lengthFunction.isValid()) {
        Q_ASSERT(lengthFunction.isFunction());
        setLengthFunction(lengthFunction);
    }
    const bool isLengthNotSupported = (length > MAX_LEN);
    const uint supportedLength = isLengthNotSupported ? MAX_LEN : length;

    childType->setParent(this);

    mData = AbstractArrayData::newArrayData(supportedLength, length, std::move(childType), this);

    if (isLengthNotSupported) {
        if (mData->isComplex()) {
            logger.warn(this).nospace() << "Array length " << length << " is larger than the maximal supported, limiting to " << MAX_LEN << ".";
        } else {
            logger.warn(this).nospace() << "Array length " << length << " is larger than the maximal supported, limiting displayed elements to " << MAX_LEN << ".";
        }
    }
}

ArrayDataInformation::ArrayDataInformation(const ArrayDataInformation& d)
    : DataInformationWithDummyChildren(d)
    , mData(nullptr)
{
    const uint supportedLength = d.mData->supportedLength();
    const uint length = d.mData->length();
    DataInformation* const childType = d.mData->childType();
    mData = AbstractArrayData::newArrayData(supportedLength, length, childType->clone(), this);
}

ArrayDataInformation::~ArrayDataInformation() = default;

void ArrayDataInformation::setArrayLength(uint newLength)
{
    uint newSupportedLength = newLength;
    const uint oldLength = mData->length();
    if (newSupportedLength > MAX_LEN) {
        if (oldLength == newLength) {
            // do not repeat warning, but keep warning state set
            const ScriptLogger::LogLevel warningLevel = ScriptLogger::LogWarning;
            if (loggedData() < warningLevel) {
                setLoggedData(warningLevel);
            }
            return;
        }

        if (mData->isComplex()) {
            logWarn().nospace() << "New array length " << newLength << " is larger than the maximal supported, limiting to " << MAX_LEN << ".";
        } else {
            logWarn().nospace() << "New array length " << newLength << " is larger than the maximal supported, limiting displayed elements to " << MAX_LEN << ".";
        }

        newSupportedLength = MAX_LEN;
    } else if (oldLength == newLength) {
        return;
    }

    const uint oldSupportedLength = mData->supportedLength();
    topLevelDataInformation()->_childCountAboutToChange(this, oldSupportedLength, newSupportedLength);
    mData->setLength(newSupportedLength, newLength);
    topLevelDataInformation()->_childCountChanged(this, oldSupportedLength, newSupportedLength);
}

void ArrayDataInformation::setArrayType(std::unique_ptr<DataInformation>&& newChildType)
{
    if (newChildType->isPrimitive() && newChildType->asPrimitive()->type() == mData->primitiveType()) {
        // there is no need to change the type
        logInfo() << "New and old child type are identical, skipping:" << mData->primitiveType();
        return;
    }
    newChildType->setParent(this);
    const uint supportedLength = mData->supportedLength();
    const uint length = mData->length();
    TopLevelDataInformation* const topLevel = topLevelDataInformation();
    if (supportedLength > 0) {
        // first create with length of 0, then change length to actual length (to ensure model is correct)
        topLevel->_childCountAboutToChange(this, supportedLength, 0);
        mData = AbstractArrayData::newArrayData(0, 0, std::move(newChildType), this);
        topLevel->_childCountChanged(this, supportedLength, 0);

        topLevel->_childCountAboutToChange(this, 0, supportedLength);
        mData->setLength(supportedLength, length);
        topLevel->_childCountChanged(this, 0, supportedLength);
    } else {
        // no need to emit the signals, which cause expensive model update
        mData = AbstractArrayData::newArrayData(0, 0, std::move(newChildType), this);
        // only the type of the array changed -> emit that this has changed data
        topLevel->setChildDataChanged();
    }
}

QScriptValue ArrayDataInformation::childType() const
{
    return mData->typeName().toLower();
}

QVariant ArrayDataInformation::childData(int row, int column, int role) const
{
    Q_ASSERT(uint(row) < mData->supportedLength());
    return mData->dataAt(row, column, role);
}

QWidget* ArrayDataInformation::createEditWidget(QWidget*) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::createEditWidget", "this should never happen!");
    return nullptr;
}

QVariant ArrayDataInformation::dataFromWidget(const QWidget*) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::dataFromWidget", "this should never happen!");
    return {};
}

void ArrayDataInformation::setWidgetData(QWidget*) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::setWidgetData", "this should never happen!");
}

BitCount64 ArrayDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    if (mParent->isTopLevel()) {
        return start * 8 + mData->offset(child);
    } 

    return mParent->asDataInformation()->childPosition(this, start) + mData->offset(child);
}

qint64 ArrayDataInformation::readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                                      BitCount64 bitsRemaining, quint8* bitOffset)
{
    if (*bitOffset != 0) {
        // TODO remove this, it will probably cause issues
        logWarn().nospace() << "Bit offset != 0 (" << *bitOffset << "), adding padding,"
            " arrays always start at full bytes.";
        bitsRemaining &= BitCount64(~7); // unset lower 3 bits to make it divisible by 8
        address++;
    }

    // update the length of the array
    if (!topLevelDataInformation()->scriptHandler()->updateLength(this)) {
        // fixed length, update state for any previous log warning about non-supported length
        if (mData->supportedLength() < mData->length()) {
            const ScriptLogger::LogLevel warningLevel = ScriptLogger::LogWarning;
            if (loggedData() < warningLevel) {
                setLoggedData(warningLevel);
            }
        }
    }

    // FIXME do not add this padding
    qint64 ret = mData->readData(input, address, bitsRemaining);
    mWasAbleToRead = ret >= 0; // if ret is -1 reading failed
    return ret;
}

bool ArrayDataInformation::setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                                        Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    if (bitOffset != 0) {
        logWarn().nospace() << "Bit offset != 0 (" << bitOffset << "), adding padding,"
            " arrays always start at full bytes.";
        bitsRemaining -= bitOffset;
        address++;
    }
    return mData->setChildData(row, value, out, address, bitsRemaining);
}

bool ArrayDataInformation::setData(const QVariant&, Okteta::AbstractByteArrayModel*,
                                   Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT_X(false, "ArrayDataInformation::setData()", "this should never be called");
    return false;
}

QScriptClass* ArrayDataInformation::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->arrayClass();
}

QScriptValue ArrayDataInformation::childToScriptValue(uint index, QScriptEngine* engine,
                                                      ScriptHandlerInfo* handlerInfo) const
{
    return mData->toScriptValue(index, engine, handlerInfo);
}

QString ArrayDataInformation::typeNameImpl() const
{
    return mData->typeName();
}

QString ArrayDataInformation::valueStringImpl() const
{
    return mData->valueString();
}

BitCount32 ArrayDataInformation::size() const
{
    return mData->size();
}

bool ArrayDataInformation::isArray() const
{
    return true;
}

DataInformation* ArrayDataInformation::childAt(unsigned int idx) const
{
    return mData->childAt(idx);
}

unsigned int ArrayDataInformation::childCount() const
{
    return mData->supportedLength();
}

QString ArrayDataInformation::childTypeName(uint index) const
{
    return mData->dataAt(index, DataInformation::ColumnType, Qt::DisplayRole).toString();
}

QString ArrayDataInformation::childString(uint index) const
{
    return mData->dataAt(index, DataInformation::ColumnValue, Qt::DisplayRole).toString();
}

int ArrayDataInformation::indexOf(const DataInformation* const data) const
{
    return mData->indexOf(data);
}

BitCount32 ArrayDataInformation::childSize(uint index) const
{
    return mData->sizeAt(index);
}

Qt::ItemFlags ArrayDataInformation::childFlags(int row, int column, bool fileLoaded) const
{
    return mData->childFlags(row, column, fileLoaded);
}

QWidget* ArrayDataInformation::createChildEditWidget(uint index, QWidget* parent) const
{
    return mData->createChildEditWidget(index, parent);
}

QVariant ArrayDataInformation::dataFromChildWidget(uint index, const QWidget* w) const
{
    return mData->dataFromChildWidget(index, w);
}

void ArrayDataInformation::setChildWidgetData(uint index, QWidget* w) const
{
    mData->setChildWidgetData(index, w);
}
