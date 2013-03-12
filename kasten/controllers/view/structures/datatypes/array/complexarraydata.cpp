/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include <QScriptValue>
#include <QVariant>
#include <KLocalizedString>
#include <KDebug>
#include "arraydatainformation.h"
#include "../topleveldatainformation.h"
#include "../structuredatainformation.h"
#include "../../script/scripthandlerinfo.h"
#include "../../script/scriptlogger.h"

ComplexArrayData::ComplexArrayData(unsigned int initialLength, DataInformation* data,
        ArrayDataInformation* parent): AbstractArrayData(data, parent)
{
    mChildren.reserve(initialLength);
    appendChildren(0, initialLength);
}

ComplexArrayData::~ComplexArrayData()
{
    qDeleteAll(mChildren);
}

void ComplexArrayData::appendChildren(uint from, uint to)
{
    for (uint i = from; i < to; ++i)
    {
        DataInformation* arrayElem = mChildType->clone();
        arrayElem->setName(QString::number(i));
        arrayElem->setParent(mParent);
        mChildren.append(arrayElem);
    }
}

void ComplexArrayData::setLength(uint newLength)
{
    uint oldLength = mChildren.count();
    if (newLength > oldLength)
    {
        mChildren.reserve(newLength);
        appendChildren(oldLength, newLength);
    }
    else if (newLength < oldLength) //XXX maybe keep some cached
    {
        qDeleteAll(mChildren.begin() + newLength, mChildren.end());
        mChildren.resize(newLength);
    }
    //else nothing to do, length stays the same
}

DataInformation* ComplexArrayData::childAt(unsigned int idx)
{
    Q_ASSERT(idx < uint(mChildren.size()));
    return mChildren.at(idx);
}

QVariant ComplexArrayData::dataAt(uint index, int column, int role)
{
    Q_ASSERT(index < uint(mChildren.size()));
    //this is slightly more efficient that delegating to the child
    if (role == Qt::DisplayRole && column == DataInformation::ColumnName)
        return QString(QLatin1Char('[') + QString::number(index) + QLatin1Char(']'));
    return mChildren.at(index)->data(column, role);
}

unsigned int ComplexArrayData::length() const
{
    return mChildren.size();
}

BitCount32 ComplexArrayData::size() const
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
    QString type = mChildType->typeName();
    return i18nc("type name, then array length", "%1[%2]", type, QString::number(length()));
}

void ComplexArrayData::setNewParentForChildren() {
    for (int i = 0; i < mChildren.size(); ++i)
        mChildren.at(i)->setParent(mParent);
}

BitCount64 ComplexArrayData::offset(const DataInformation* child) const
{
    BitCount64 offset = 0;
    //sum size of elements up to index
    for (int i = 0; i < mChildren.size(); ++i)
    {
        DataInformation* current = mChildren.at(i);
        if (current == child)
            return offset;
        offset += current->size();
    }
    Q_ASSERT(false); //should never be reached
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
    mParent->logWarn() << data->fullObjectPath() << "is not a valid child!";
    Q_ASSERT(false); //should never land here
    return -1;
}

QScriptValue ComplexArrayData::toScriptValue(uint index, QScriptEngine* engine,
        ScriptHandlerInfo* handlerInfo)
{
    Q_ASSERT(index < length());
    return mChildren.at(index)->toScriptValue(engine, handlerInfo);
}

qint64 ComplexArrayData::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining)
{
    qint64 readBits = 0;
    TopLevelDataInformation* top = mParent->topLevelDataInformation();
    quint8 bitOffset = 0; //FIXME no more padding before and after arrays!
    StructureDataInformation::readChildren(mChildren, input, address, bitsRemaining, &bitOffset, &readBits, top);
    return readBits;
}

bool ComplexArrayData::setChildData(uint row, QVariant value, Okteta::AbstractByteArrayModel* out,
        Okteta::Address address, BitCount64 bitsRemaining)
{
    Q_ASSERT(row < unsigned(mChildren.size()));
    unsigned int bits = 0;
    for (uint i = 0; i < row; ++i)
    {
        bits += mChildren.at(i)->size();
    }
    return mChildren.at(row)->setData(value, out, address + (bits / 8), bitsRemaining - bits, bits % 8);
}

PrimitiveDataType ComplexArrayData::primitiveType() const
{
    if (mChildType->isBitfield())
        return Type_Bitfield;
    else
        return Type_Invalid;
}

BitCount32 ComplexArrayData::sizeAt(uint index)
{
    Q_ASSERT(index < length());
    return mChildren.at(index)->size();
}

Qt::ItemFlags ComplexArrayData::childFlags(int index, int column, bool fileLoaded)
{
    Q_ASSERT(index >= 0 && uint(index) < length());
    return mChildren.at(index)->flags(column, fileLoaded);
}

QWidget* ComplexArrayData::createChildEditWidget(uint index, QWidget* parent) const
{
    Q_ASSERT(index < length());
    return mChildren.at(index)->createEditWidget(parent);
}

QVariant ComplexArrayData::dataFromChildWidget(uint index, const QWidget* w) const
{
    Q_ASSERT(index < length());
    return mChildren.at(index)->dataFromWidget(w);

}

void ComplexArrayData::setChildWidgetData(uint index, QWidget* w) const
{
    Q_ASSERT(index < length());
    mChildren.at(index)->setWidgetData(w);
}
