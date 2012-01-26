/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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
#include "datainformationwithchildren.h"
#include "topleveldatainformation.h"
#include "../script/scriptvalueconverter.h"
#include "../script/scriptutils.h"
#include "../script/scripthandlerinfo.h"
#include "../script/classes/structunionscriptclass.h"

#include <KLineEdit>
#include <QtScript/QScriptEngine>

void DataInformationWithChildren::appendChild(DataInformation* child)
{
    if (child != NULL)
    {
        child->setParent(this);
        mChildren.append(child);
    }
}

DataInformation* DataInformationWithChildren::childAt(unsigned int idx) const
{
    if (idx >= (unsigned) mChildren.size())
    {
        return NULL;
    }
    return mChildren[idx];
}

bool DataInformationWithChildren::setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    kWarning() << "this should not be called";
    Q_ASSERT(row < childCount());
    int offs = offset(row);
    quint8 bitOffs = (offs + bitOffset) & 7; //mod 8
    return mChildren.at(row)->setData(value, out, address + (offs / 8), bitsRemaining - offs, bitOffs);
}

bool DataInformationWithChildren::setData(const QVariant& value, Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    Q_ASSERT_X(false, "DataInformationWithChildren::setData()", "this should never be called");
    return false;
}


qint64 DataInformationWithChildren::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    //first of all update the structure:
    topLevelDataInformation()->updateElement(this);

    uint readBytes = 0;
    qint64 readBits = 0;
    const quint8 origBitOffset = *bitOffset;
    for (int i = 0; i < mChildren.size(); i++)
    {
        qint64 currentReadBits = mChildren[i]->readData(input, address + readBytes,
                bitsRemaining - readBits, bitOffset);
        if (currentReadBits == -1)
        {
            mWasAbleToRead = false;
            //could not read one element -> whole structure could not be read
            return -1;
        }
        readBits += currentReadBits;
        readBytes = (readBits + origBitOffset) / 8;
    }
    mWasAbleToRead = true;
    return readBits;
}

DataInformationWithChildren::~DataInformationWithChildren()
{
    qDeleteAll(mChildren);
}

DataInformationWithChildren::DataInformationWithChildren(QString& name, DataInformation* parent)
    : DataInformation(name, parent)
{
}

DataInformationWithChildren::DataInformationWithChildren(const DataInformationWithChildren& d)
        : DataInformation(d)
{
    int count = d.mChildren.count();
    mChildren.reserve(count);
    for (int i = 0; i < count; ++i)
    {
        const DataInformation* dat = d.mChildren.at(i);
        DataInformation* child = dat->clone();
        child->setParent(this);
        mChildren.append(child);
    }
}

QWidget* DataInformationWithChildren::createEditWidget(QWidget* parent) const
{
    return new KLineEdit(parent);
}

QVariant DataInformationWithChildren::dataFromWidget(const QWidget* w) const
{
    Q_UNUSED(w);
    return QVariant();
}

void DataInformationWithChildren::setWidgetData(QWidget* w) const
{
    Q_UNUSED(w)
}

BitCount32 DataInformationWithChildren::size() const
{
    BitCount32 size = 0;
    for (unsigned int i = 0; i < childCount(); ++i)
    {
        size += childAt(i)->size();
    }
    return size;
}

BitCount32 DataInformationWithChildren::offset(unsigned int index) const
{
    if (index >= childCount())
        return 0;
    quint64 offset = 0;
    //sum size of elements up to index
    for (unsigned int i = 0; i < index; ++i)
    {
        offset += childAt(i)->size();
    }
    return offset;
}

void DataInformationWithChildren::resetValidationState()
{
    mHasBeenValidated = false;
    mValidationSuccessful = false;
    if (mAdditionalData)
        mAdditionalData->setValidationError(QString());
    foreach (DataInformation* child, mChildren)
        {
            child->resetValidationState();
        }
}

void DataInformationWithChildren::calculateValidationState()
{
    if (hasChildren())
    {
        bool hasValidatedChildren = false;
        bool allChildrenValid = true;
        for (uint i = 0; i < childCount(); ++i)
        {
            DataInformation* child = childAt(i);
            DataInformationWithChildren* childWithChildren =
                    dynamic_cast<DataInformationWithChildren*> (child);
            if (childWithChildren)
                childWithChildren->calculateValidationState();
            //first make sure the child item validation state has been set
            if (child->hasBeenValidated())
            {
                hasValidatedChildren = true;
                if (!child->validationSuccessful())
                {
                    allChildrenValid = false;
                    break; //one is invalid -> whole structure is invalid
                }
            }
        }
        if (hasValidatedChildren)
        {
            setValidationSuccessful(allChildrenValid);
        }
    }
}

void DataInformationWithChildren::setChildren(QScriptValue children)
{
    if (children.isNull() || children.isUndefined())
        ScriptUtils::object()->logScriptError(
            QLatin1String("attempting to set children of ")
            + name() + QLatin1String(" to null. This must be and error in the script."));
    ScriptValueConverter conv(children, name());
    QList<DataInformation*> convertedVals = conv.convertValues();

    //is valid now
    //childcount changed to 0
    topLevelDataInformation()->_childrenAboutToBeRemoved(this, 0, childCount() - 1);
    qDeleteAll(mChildren);
    mChildren.clear();
    topLevelDataInformation()->_childrenRemoved(this, 0, childCount() - 1);

    const uint count = convertedVals.size();
    topLevelDataInformation()->_childrenAboutToBeInserted(this, 0, count - 1);
    mChildren.reserve(count);
    for (uint i = 0; i < count; ++i)
    {
        DataInformation* child = convertedVals.at(i);
        appendChild(child);
    }
    topLevelDataInformation()->_childrenInserted(this, 0, count - 1);
}

int DataInformationWithChildren::indexOf(const DataInformation* const data) const
{
    const int size = mChildren.size();
    for (int i = 0; i < size; ++i)
    {
        if (mChildren.at(i) == data)
        {
            return i;
        }
    }
    return -1;
}

QVariant DataInformationWithChildren::childData(int row, int column, int role) const
{
    Q_ASSERT(row >= 0 && row < mChildren.size());
    //just delegate to child
    return mChildren.at(row)->data(column, role);
}

QScriptValue DataInformationWithChildren::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    QScriptValue ret = engine->newObject(handlerInfo->mStructUnionClass.data());
    ret.setData(engine->toScriptValue(static_cast<DataInformation*>(this)));
    return ret;
}

QString DataInformationWithChildren::tooltipString() const
{
    if (mHasBeenValidated && !mValidationSuccessful)
    {
        QString validationError;
        if (additionalData() && !additionalData()->validationError().isEmpty())
        {
            validationError = i18nc("not all values in this structure"
                    " are as they should be", "Validation failed: \"%1\"",
                    additionalData()->validationError());
        }
        else
        {
            validationError = i18nc("not all values in this structure"
                    " are as they should be", "Validation failed.");
        }
        return i18np("Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)\n\n %6",
                "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)\n\n %6",
                childCount(), name(), valueString(), typeName(), sizeString(), validationError);
    }
    else
    {
        return i18np("Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)",
                     "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)",
                     childCount(), name(), valueString(), typeName(), sizeString());
    }
}


BitCount32 DataInformationWithChildren::childSize(int index) const
{
    Q_ASSERT(index >= 0 && index < childCount());
    return mChildren.at(index)->size();
}

