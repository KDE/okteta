/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "primitivearraydata.hpp"

#include "arraydatainformation.hpp"
#include "../primitive/primitivedatainformation.hpp"
#include "../../script/scriptlogger.hpp"
#include "../../script/safereference.hpp"
#include "../../structlogging.hpp"

#include <Okteta/AbstractByteArrayModel>

#include <limits>
#include <KLocalizedString>

template <PrimitiveDataType type>
inline PrimitiveArrayData<type>::PrimitiveArrayData(unsigned int initialLength, PrimitiveDataInformation* childType,
                                                    ArrayDataInformation* parent)
    : AbstractArrayData(childType, parent)
    , mDummy(parent)
{
    Q_ASSERT(childType->type() == type);
    mData.reserve(initialLength);
    mData.resize(initialLength);
}

template <PrimitiveDataType type>
qint64 PrimitiveArrayData<type>::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                                          BitCount64 bitsRemaining)
{
    Q_ASSERT(bitsRemaining % 8 == 0);
    if (this->length() == 0) {
        return 0; // no need to read anything
    }
    // integer division -> gives us the desired result, limited by the number of items in this array
    const quint64 maxRemaining = (bitsRemaining / 8) / sizeof(T);
    // since its 64 bits may be larger than a 32 bit value and have all lower 32 bits as zero
    // therefore we use std::numeric_limits::max()
    quint32 maxRemaining32 = (maxRemaining > std::numeric_limits<quint32>::max()
                              ? std::numeric_limits<quint32>::max() : quint32(maxRemaining));
    const quint32 maxNumItems = qMin(this->length(), maxRemaining32);
    if (maxNumItems == 0) {
        return -1; // reached EOF
    }
    const QSysInfo::Endian byteOrder = mChildType->effectiveByteOrder();
    if (byteOrder == QSysInfo::ByteOrder) {
        this->readDataNativeOrder(maxNumItems, input, address);
    } else {
        this->readDataNonNativeOrder(maxNumItems, input, address);
    }
    this->mNumReadValues = maxNumItems;
    return maxNumItems * sizeof(T) * 8;
}

template <PrimitiveDataType type>
void PrimitiveArrayData<type>::readDataNativeOrder(uint numItems, Okteta::AbstractByteArrayModel* input,
                                                   Okteta::Address address)
{
    Q_ASSERT(numItems <= length());
    const Okteta::Size numBytes = numItems * sizeof(T);
    Q_ASSERT(input->size() >= numBytes + address);
    Q_UNUSED(numBytes);
    auto* vectorBytes = reinterpret_cast<Okteta::Byte*>(this->mData.data());
    const Okteta::Size numCopied = input->copyTo(vectorBytes, address, numItems * sizeof(T));
    Q_ASSERT(numCopied == numBytes);
    Q_UNUSED(numCopied);
}

template <PrimitiveDataType type>
void PrimitiveArrayData<type>::readDataNonNativeOrder(uint numItems, Okteta::AbstractByteArrayModel* input,
                                                      Okteta::Address address)
{
    Q_ASSERT(numItems <= length());
    const uint numBytes = numItems * sizeof(T);
    Q_ASSERT(uint(input->size()) >= numBytes + address);
    auto* vectorBytes = reinterpret_cast<Okteta::Byte*>(this->mData.data());
    for (uint itemOffs = 0; itemOffs < numBytes; itemOffs += sizeof(T)) {
        // the compiler should unroll this loop
        for (uint byte = 0; byte < sizeof(T); byte++) {
            vectorBytes[itemOffs + byte] = input->byte(address + itemOffs + (sizeof(T) - byte - 1));
        }
    }
}

template <PrimitiveDataType type>
bool PrimitiveArrayData<type>::setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                                            Okteta::Address address, BitCount64 bitsRemaining)
{
    Q_ASSERT(row < length());
    Q_ASSERT(value.isValid());
    Q_ASSERT(bitsRemaining % 8 == 0);
    if ((row + 1) * sizeof(T) * 8 >= bitsRemaining) {
        this->mParent->logInfo() << " not enough bits remaining ("
                                 << bitsRemaining << ") need " << ((row + 1) * sizeof(T) * 8);
        return false;
    }
    QSysInfo::Endian byteOrder = mChildType->effectiveByteOrder();
    bool littleEndian = byteOrder == QSysInfo::LittleEndian;
    bool ok = false;
    T convertedVal = DisplayClass::fromVariant(value, &ok);
    if (!ok) {
        this->mParent->logWarn() << "could not convert" << value << "to" << type;
        return false;
    }
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
        << AbstractArrayData::mParent->fullObjectPath() << "setting index" << row << "to"
        << value << "(= " << convertedVal << ")";
    this->mData[row] = convertedVal;
    this->writeOneItem(convertedVal, address + (row * sizeof(T)), out, littleEndian);
    return true;
}

template <PrimitiveDataType type>
void PrimitiveArrayData<type>::writeOneItem(T value, Okteta::Address addr,
                                            Okteta::AbstractByteArrayModel* out, bool littleEndian)
{
    if (littleEndian) {
        for (uint i = 0; i < sizeof(T); ++i) {
            // compiler should be smart enough not to create a loop
            quint8 val = (quint64(value) & (quint64(0xff) << (8 * i))) >> (8 * i);
            out->setByte(addr + i, val);
        }
    } else {
        for (uint i = 1; i <= sizeof(T); ++i) {
            // compiler should be smart enough not to create a loop
            quint8 val = (quint64(value) & (quint64(0xff) << (8 * (sizeof(T) - i)))) >> (8 * (sizeof(T) - i));
            out->setByte(addr + i, val);
        }
    }
}

template <>
void PrimitiveArrayData<PrimitiveDataType::Float>::writeOneItem(float value, Okteta::Address addr,
                                                                Okteta::AbstractByteArrayModel* out, bool littleEndian)
{
    Q_ASSERT(sizeof(float) == sizeof(quint32));
    union {
        quint32 intVal; float floatVal;
    } un;
    un.floatVal = value;
    PrimitiveArrayData<PrimitiveDataType::UInt32>::writeOneItem(un.intVal, addr, out, littleEndian);
}

template <>
void PrimitiveArrayData<PrimitiveDataType::Double>::writeOneItem(double value, Okteta::Address addr,
                                                                 Okteta::AbstractByteArrayModel* out, bool littleEndian)
{
    Q_ASSERT(sizeof(double) == sizeof(quint64));
    union {
        quint64 intVal; double doubleVal;
    } un;
    un.doubleVal = value;
    PrimitiveArrayData<PrimitiveDataType::UInt64>::writeOneItem(un.intVal, addr, out, littleEndian);
}

template <PrimitiveDataType type>
void PrimitiveArrayData<type>::activateIndex(uint index)
{
    Q_ASSERT(index < length());
    // invalidate all previous references
    SafeReferenceHolder::instance.invalidateAll(mChildType.data());
    mChildType->mWasAbleToRead = mNumReadValues > index;
    mChildType->asPrimitive()->setValue(mData.at(index));
    mChildType->setName(QString::number(index));
    mDummy.setDummyIndex(index);
}

template <PrimitiveDataType type>
QVariant PrimitiveArrayData<type>::dataAt(uint index, int column, int role)
{
    Q_ASSERT(index < length());
    if (role == Qt::DisplayRole) {
        if (column == DataInformation::ColumnName) {
            return QString(QLatin1Char('[') + QString::number(index) + QLatin1Char(']'));
        }
        if (column == DataInformation::ColumnType) {
            return mChildType->typeName();
        }
        if (column == DataInformation::ColumnValue) {
            // if we are outside the valid range
            if (uint(index) >= this->mNumReadValues) {
                return DataInformation::eofReachedData(Qt::DisplayRole);
            }
            if (Q_UNLIKELY(mChildType->toStringFunction().isValid())) {
                activateIndex(index);
                return mChildType->valueString();
            }
            return DisplayClass::staticValueString(mData.at(index));
        }
    }
    if (column == DataInformation::ColumnValue && uint(index) >= this->mNumReadValues) {
        return DataInformation::eofReachedData(role);
    }
    return {};
}

template <PrimitiveDataType type>
QString PrimitiveArrayData<type>::typeName() const
{
    return QString(mChildType->typeName() + QLatin1Char('[')
                   + QString::number(this->length()) + QLatin1Char(']'));
}

template <PrimitiveDataType type>
int PrimitiveArrayData<type>::indexOf(const DataInformation* data) const
{
    if (data == &mDummy) {
        return this->mDummy.dummyIndex();
    }
    if (data == mChildType.data()) {
        return this->mDummy.dummyIndex();
    }
    Q_ASSERT_X(false, "PrimitiveArrayData::indexOf", "Logic error, should never be reached");
    return -1;
}

template <PrimitiveDataType type>
QScriptValue PrimitiveArrayData<type>::toScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    activateIndex(index);
    return mChildType->toScriptValue(engine, handlerInfo);
}

template <PrimitiveDataType type>
QWidget* PrimitiveArrayData<type>::createChildEditWidget(uint index, QWidget* parent) const
{
    Q_ASSERT(index < length());
    Q_UNUSED(index)
    return DisplayClass::staticCreateEditWidget(parent);
}

template <PrimitiveDataType type>
QVariant PrimitiveArrayData<type>::dataFromChildWidget(uint index, const QWidget* w) const
{
    Q_ASSERT(index < length());
    Q_UNUSED(index)
    return DisplayClass::staticDataFromWidget(w);
}

template <PrimitiveDataType type>
void PrimitiveArrayData<type>::setChildWidgetData(uint index, QWidget* w) const
{
    Q_ASSERT(index < length());
    Q_UNUSED(index)
    DisplayClass::staticSetWidgetData(mData.at(index), w);
}

// now instantiate all the template instances
template class PrimitiveArrayData<PrimitiveDataType::Bool8>;
template class PrimitiveArrayData<PrimitiveDataType::Bool16>;
template class PrimitiveArrayData<PrimitiveDataType::Bool32>;
template class PrimitiveArrayData<PrimitiveDataType::Bool64>;
template class PrimitiveArrayData<PrimitiveDataType::Int8>;
template class PrimitiveArrayData<PrimitiveDataType::Int16>;
template class PrimitiveArrayData<PrimitiveDataType::Int32>;
template class PrimitiveArrayData<PrimitiveDataType::Int64>;
template class PrimitiveArrayData<PrimitiveDataType::UInt8>;
template class PrimitiveArrayData<PrimitiveDataType::UInt16>;
template class PrimitiveArrayData<PrimitiveDataType::UInt32>;
template class PrimitiveArrayData<PrimitiveDataType::UInt64>;
template class PrimitiveArrayData<PrimitiveDataType::Char>;
template class PrimitiveArrayData<PrimitiveDataType::Float>;
template class PrimitiveArrayData<PrimitiveDataType::Double>;
