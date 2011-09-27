/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
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



#include "complexarraydata.h"
#include "../datainformation.h"
#include "../topleveldatainformation.h"
#include "../../script/scripthandlerinfo.h"

ComplexArrayData::ComplexArrayData(unsigned int initialLength, DataInformation* data,
        DataInformation* parent): AbstractArrayData(parent), mChildType(data)
{
    mChildren.reserve(initialLength);
    mChildType->setParent(mParent);
    for (unsigned int i = 0; i < initialLength; ++i) {
        DataInformation* child = mChildType->clone();
        child->setParent(mParent);
        mChildren.append(child);    }
}

ComplexArrayData::ComplexArrayData(const ComplexArrayData& c): AbstractArrayData(c), mChildType(0)
{
    Q_CHECK_PTR(c.mChildType);
    mChildType = c.mChildType->clone();
    mChildType->setParent(mParent);
    uint initialLength = c.length();
    mChildren.reserve(initialLength);
    for (unsigned int i = 0; i < initialLength; ++i) {
        DataInformation* child = mChildType->clone();
        child->setParent(mParent);
        mChildren.append(child);
    }
}


ComplexArrayData::~ComplexArrayData()
{
    qDeleteAll(mChildren);
    delete mChildType;
}

void ComplexArrayData::setLength(int newLength)
{
    int oldLength = mChildren.count();
    kDebug() << "resizing" << mParent->name() << "from " << oldLength << " to " << newLength;
    if (newLength > oldLength)
    {
        mParent->topLevelDataInformation()->_childrenAboutToBeInserted(mParent, oldLength, newLength - 1);
        mChildren.reserve(newLength);
        for (int i = oldLength; i < newLength; ++i)
        {
            DataInformation* arrayElem = mChildType->clone();
            arrayElem->setParent(mParent);
            mChildren.append(arrayElem);
        }
        mParent->topLevelDataInformation()->_childrenInserted(mParent, oldLength, newLength - 1);
    }
    else if (newLength < oldLength) //XXX maybe keep some cached
    {
        mParent->topLevelDataInformation()->_childrenAboutToBeRemoved(mParent, newLength, oldLength - 1);

        qDeleteAll(mChildren.begin() + newLength, mChildren.end());
        mChildren.resize(newLength);

        mParent->topLevelDataInformation()->_childrenRemoved(mParent, newLength, oldLength - 1);
    }
    //else nothing to do, length stays the same
}

DataInformation* ComplexArrayData::childAt(unsigned int idx)
{
    Q_ASSERT(idx < uint(mChildren.size()));
    return mChildren.at(idx);
}

AbstractArrayData* ComplexArrayData::clone()
{
    return new ComplexArrayData(*this);
}

QVariant ComplexArrayData::dataAt(int index, int column, int role)
{
    Q_ASSERT(index < uint(mChildren.size()));
    return mChildren.at(index)->data(column, role);
}

unsigned int ComplexArrayData::length() const
{
    return mChildren.size();
}

unsigned int ComplexArrayData::size() const
{
    int max = mChildren.size();
    uint total = 0;
    for (int i = 0; i < max; ++i) {
        total += mChildren.at(i)->size();
    }
    return total;
}

QString ComplexArrayData::typeName() const
{
    QString type;
    if (mChildType->isStruct())
        type = QLatin1String("struct ") + mChildType->name();
    else if (mChildType->isUnion())
        type = QLatin1String("union ") + mChildType->name();
    else
        type = mChildType->typeName();
    return i18nc("type name, then array length", "%1[%2]", type, QString::number(length()));
}

void ComplexArrayData::setParent(DataInformation* parent)
{
    AbstractArrayData::setParent(parent);
    mChildType->setParent(parent);
    for (int i = 0; i < mChildren.size(); ++i)
        mChildren.at(i)->setParent(parent);
}

quint64 ComplexArrayData::offset(unsigned int index) const
{
    Q_ASSERT(index < length());
    quint64 offset = 0;
    //sum size of elements up to index
    for (unsigned int i = 0; i < index; ++i)
    {
        offset += mChildren.at(i)->size();
    }
    return offset;
}

int ComplexArrayData::indexOf(const DataInformation* const data) const
{
    const int size = mChildren.size();
    for (int i = 0; i < size; ++i)
    {
        if (mChildren.at(i) == data)
            return i;
    }
    kWarning() << data << "is not child of " << this;
    return -1;
}

void ComplexArrayData::setChildType(DataInformation* newChildType)
{
    uint len = mChildren.count();

    delete mChildType;
    mChildType = newChildType;
    mChildType->setParent(mParent);

    if (len == 0)
        return; //do nothing, prevent integer underflow when calling len - 1

    mParent->topLevelDataInformation()->_childrenAboutToBeRemoved(mParent, 0, len - 1);
    qDeleteAll(mChildren);
    //no need to clear, we overwrite all elements
    mParent->topLevelDataInformation()->_childrenRemoved(mParent, 0, len - 1);

    mParent->topLevelDataInformation()->_childrenAboutToBeInserted(mParent, 0, len - 1);
    for (uint i = 0; i < len; i++)
    {
        DataInformation* arrayElem = newChildType->clone();
        mChildren[i] = arrayElem;
    }
    mParent->topLevelDataInformation()->_childrenInserted(mParent, 0, len - 1);
}

QString ComplexArrayData::strictTypeName() const
{
    return mChildType->typeName();
}

QScriptValue ComplexArrayData::toScriptValue(uint index, QScriptEngine* engine,
        ScriptHandlerInfo* handlerInfo) const
{
    Q_ASSERT(index < length());
    return mChildren.at(index)->toScriptValue(engine, handlerInfo);
}

qint64 ComplexArrayData::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, quint64 bitsRemaining)
{
    uint readBytes = 0;
    qint64 readBits = 0;
    quint8 bitOffset = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        qint64 currentReadBits = mChildren[i]->readData(input, address + readBytes,
                bitsRemaining - readBits, &bitOffset);
        if (currentReadBits == -1)
        {
            //could not read one element -> whole structure could not be read
            return -1;
        }
        readBits += currentReadBits;
        readBytes = (readBits + bitOffset) / 8;
    }
    return readBits;
}

bool ComplexArrayData::setChildData(uint row, QVariant value, Okteta::AbstractByteArrayModel* out,
        Okteta::Address address, quint64 bitsRemaining)
{
    Q_ASSERT(row < unsigned(mChildren.size()));
    unsigned int bits = 0;
    for (uint i = 0; i < row; ++i)
    {
        bits += mChildren.at(i)->size();
    }
    return mChildren.at(row)->setData(value, out, address + (bits / 8), bitsRemaining - bits, bits % 8);
}

