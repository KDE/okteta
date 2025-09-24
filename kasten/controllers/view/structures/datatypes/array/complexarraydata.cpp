/*
    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "complexarraydata.hpp"

// lib
#include "arraydatainformation.hpp"
#include <topleveldatainformation.hpp>
#include <datainformationwithchildren.hpp>
#include <scripthandlerinfo.hpp>
#include <scriptlogger.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>
#include <QVariant>
// Std
#include <utility>

ComplexArrayData::ComplexArrayData(unsigned int initialLength,
                                   std::unique_ptr<DataInformation>&& data,
                                   ArrayDataInformation* parent)
    : AbstractArrayData(std::move(data), parent)
{
    mChildren.reserve(initialLength);
    appendChildren(0, initialLength);
}

ComplexArrayData::~ComplexArrayData() = default;

void ComplexArrayData::appendChildren(uint from, uint to)
{
    mChildren.reserve(mChildren.size() + to - from);
    for (uint i = from; i < to; ++i) {
        std::unique_ptr<DataInformation> arrayElem = mChildType->clone();
        arrayElem->setName(QString::number(i));
        arrayElem->setParent(mParent);
        mChildren.emplace_back(std::move(arrayElem));
    }
}

void ComplexArrayData::setLength(uint newLength)
{
    uint oldLength = mChildren.size();
    if (newLength > oldLength) {
        mChildren.reserve(newLength);
        appendChildren(oldLength, newLength);
    } else if (newLength < oldLength) {   // XXX maybe keep some cached
        mChildren.resize(newLength);
    }
    // else nothing to do, length stays the same
}

DataInformation* ComplexArrayData::childAt(unsigned int idx)
{
    Q_ASSERT(idx < mChildren.size());
    return mChildren[idx].get();
}

QVariant ComplexArrayData::dataAt(uint index, int column, int role)
{
    Q_ASSERT(index < mChildren.size());
    // this is slightly more efficient that delegating to the child
    if (role == Qt::DisplayRole && column == DataInformation::ColumnName) {
        return QString(QLatin1Char('[') + QString::number(index) + QLatin1Char(']'));
    }
    return mChildren[index]->data(column, role);
}

unsigned int ComplexArrayData::length() const
{
    return mChildren.size();
}

bool ComplexArrayData::isComplex() const
{
    return true;
}

BitCount32 ComplexArrayData::size() const
{
    uint total = 0;
    for (const auto& child : mChildren) {
        total += child->size();
    }

    return total;
}

QString ComplexArrayData::typeName() const
{
    QString type = mChildType->typeName();
    return i18nc("type name, then array length", "%1[%2]", type, QString::number(length()));
}

QString ComplexArrayData::valueString() const
{
    return QString();
}

void ComplexArrayData::setNewParentForChildren()
{
    for (const auto& child : mChildren) {
        child->setParent(mParent);
    }
}

BitCount64 ComplexArrayData::offset(const DataInformation* child) const
{
    BitCount64 offset = 0;
    // sum size of elements up to index
    for (const auto& current : mChildren) {
        if (current.get() == child) {
            return offset;
        }
        offset += current->size();
    }

    Q_ASSERT(false); // should never be reached
    return offset;
}

int ComplexArrayData::indexOf(const DataInformation* const data) const
{
    const std::size_t size = mChildren.size();
    for (std::size_t i = 0; i < size; ++i) {
        if (mChildren[i].get() == data) {
            return i;
        }
    }

    mParent->logWarn() << data->fullObjectPath() << "is not a valid child!";
    Q_ASSERT(false); // should never land here
    return -1;
}

QScriptValue ComplexArrayData::toScriptValue(uint index, QScriptEngine* engine,
                                             ScriptHandlerInfo* handlerInfo)
{
    Q_ASSERT(index < length());
    return mChildren[index]->toScriptValue(engine, handlerInfo);
}

qint64 ComplexArrayData::readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining)
{
    qint64 readBits = 0;
    TopLevelDataInformation* top = mParent->topLevelDataInformation();
    quint8 bitOffset = 0; // FIXME no more padding before and after arrays!
    std::ignore = DataInformationWithChildren::readChildren(mChildren, input, address, bitsRemaining, &bitOffset, &readBits, top);
    return readBits;
}

bool ComplexArrayData::setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                                    Okteta::Address address, BitCount64 bitsRemaining)
{
    Q_ASSERT(row < mChildren.size());
    unsigned int bits = 0;
    for (uint i = 0; i < row; ++i) {
        bits += mChildren[i]->size();
    }

    return mChildren[row]->setData(value, out, address + (bits / 8), bitsRemaining - bits, bits % 8);
}

PrimitiveDataType ComplexArrayData::primitiveType() const
{
    if (mChildType->isBitfield()) {
        return PrimitiveDataType::Bitfield;
    } 

    return PrimitiveDataType::Invalid;
}

BitCount32 ComplexArrayData::sizeAt(uint index)
{
    Q_ASSERT(index < length());
    return mChildren[index]->size();
}

Qt::ItemFlags ComplexArrayData::childFlags(int index, int column, bool fileLoaded)
{
    Q_ASSERT(index >= 0 && uint(index) < length());
    return mChildren[index]->flags(column, fileLoaded);
}

QWidget* ComplexArrayData::createChildEditWidget(uint index, QWidget* parent) const
{
    Q_ASSERT(index < length());
    return mChildren[index]->createEditWidget(parent);
}

QVariant ComplexArrayData::dataFromChildWidget(uint index, const QWidget* w) const
{
    Q_ASSERT(index < length());
    return mChildren[index]->dataFromWidget(w);

}

void ComplexArrayData::setChildWidgetData(uint index, QWidget* w) const
{
    Q_ASSERT(index < length());
    mChildren[index]->setWidgetData(w);
}
