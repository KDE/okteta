/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "staticlengtharraydatainformation.h"
#include "topleveldatainformation.h"
#include "../script/scriptvalueconverter.h"
#include "../script/scriptutils.h"
#include <KLineEdit>
#include <KIcon>
#include <QtScript/QScriptEngine>

void DataInformationWithChildren::appendChild(DataInformation* child)
{
    if (child != NULL)
    {
        child->setParent(this);
        mChildren.append(child);
        connect(child, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
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

bool DataInformationWithChildren::setData(const QVariant& value,
        DataInformation* inf, Okteta::AbstractByteArrayModel *out,
        Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    if (this == inf)
        return true; //do nothing since this is not editable
    quint64 readBits = 0;
    uint readBytes = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {

        if (mChildren[i]->setData(value, inf, out, address + readBytes,
                bitsRemaining - readBits, bitOffset))
            return true; //found -> done job
        readBits += mChildren[i]->size();
        readBytes = (readBits + *bitOffset) / 8;

    }
    return false;
}

qint64 DataInformationWithChildren::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    //first of all update the structure:
    topLevelDataInformation()->updateElement(this);

    uint readBytes = 0;
    qint64 readBits = 0;
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
        readBytes = (readBits + *bitOffset) / 8;
    }
    mWasAbleToRead = true;
    return readBits;
}

DataInformationWithChildren::~DataInformationWithChildren()
{
    //    qDeleteAll(mChildren); // no need since this is set to their parent
}

DataInformationWithChildren::DataInformationWithChildren(QString& name, DataInformation* parent) :
    DataInformation(name, parent)
{
}

DataInformationWithChildren::DataInformationWithChildren(
        const DataInformationWithChildren& d) :
    DataInformation(d)
{
    for (int i = 0; i < d.mChildren.size(); ++i)
    {
        const DataInformation* dat = d.mChildren.at(i);
        DataInformation* child = dat->clone();
        child->setParent(this);
        QObject::connect(child, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
        mChildren.append(child);
    }
}

QList<const DataInformation*> DataInformationWithChildren::findChildrenWithName(
        const QString& name, const DataInformation* const upTo) const
{
    QList<const DataInformation*> retList;
    if (parent() && !dynamic_cast<TopLevelDataInformation*> (parent()))
    {
        DataInformationWithChildren* par =
                dynamic_cast<DataInformationWithChildren*> (parent());
        if (par)
            retList.append(par->findChildrenWithName(name, this));
    }
    for (uint i = 0; i < childCount(); ++i)
    {
        DataInformation* data = childAt(i);
        if (data == upTo)
            break;
        if (data->name() == name)
            retList.append(data);
        DataInformationWithChildren* dataWithChildren =
                dynamic_cast<DataInformationWithChildren*> (data);
        if (dataWithChildren)
            retList.append(dataWithChildren->findChildrenWithName(name, NULL));
    }
    return retList;
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

QVariant DataInformationWithChildren::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == ColumnName)
        {
            if (dynamic_cast<AbstractArrayDataInformation*> (parent()))
            {
                return QString("[%1]").arg(mIndex);
            }
            return name();
        }
        else if (column == ColumnType)
            return typeName();
        else if (column == ColumnValue)
            return valueString(); //empty QString
        else
            return QVariant();
    }
    else if (role == Qt::ToolTipRole)
    {
        if (mHasBeenValidated && !mValidationSuccessful)
        {
            QString validationError;
            if (additionalData() && !additionalData()->validationError().isEmpty())
                validationError = i18nc("not all values in this structure"
                    " are as they should be", "Validation failed: \"%1\"",
                        additionalData()->validationError());
            else
                validationError = i18nc("not all values in this structure"
                    " are as they should be", "Validation failed.");

            return i18np(
                    "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)\n\n %6",
                    "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)\n\n %6",
                    childCount(), name(), valueString(), typeName(), sizeString(),
                    validationError);
        }
        else
            return i18np("Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)",
                    "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)",
                    childCount(), name(), valueString(), typeName(), sizeString());
    }
    else if (role == Qt::DecorationRole && column == ColumnName)
    {
        //XXX better icons?
        if (mHasBeenValidated)
            return mValidationSuccessful ? KIcon("task-complete") : KIcon(
                    "dialog-warning");
    }
    return QVariant();
}

int DataInformationWithChildren::size() const
{
    int size = 0;
    for (unsigned int i = 0; i < childCount(); ++i)
    {
        size += childAt(i)->size();
    }
    return size;
}

quint64 DataInformationWithChildren::offset(unsigned int index) const
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

QScriptValue DataInformationWithChildren::childrenAsScriptValue() const
{
    QScriptEngine* engine = topLevelDataInformation()->scriptEngine();
    QScriptValue childrenScriptVal = engine->newArray(childCount());
    for (uint i = 0; i < childCount(); ++i)
    {
        //just append all children as a property
        DataInformation* data = mChildren.at(i);
        QScriptValue childObj = engine->newQObject(data, QScriptEngine::QtOwnership,
                QScriptEngine::ExcludeDeleteLater);
        childrenScriptVal.setProperty(data->name(), childObj, QScriptValue::ReadOnly
                | QScriptValue::Undeletable);
        //allow array access
        childrenScriptVal.setProperty(i, childObj, QScriptValue::ReadOnly
                | QScriptValue::Undeletable);
    }
    return childrenScriptVal;
}

QScriptValue DataInformationWithChildren::child(QString name) const
{
    for (uint i = 0; i < childCount(); ++i)
    {
        //just append all children as a property
        DataInformation* data = mChildren.at(i);
        QScriptEngine* engine = topLevelDataInformation()->scriptEngine();
        if (data->name() == name)
        {
            QScriptValue childObj = engine->newQObject(data,
                    QScriptEngine::QtOwnership, QScriptEngine::ExcludeDeleteLater);
            return childObj;
        }
    }
    //not found -> return invalid value
    return QScriptValue(QScriptValue::NullValue);
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

QScriptValue DataInformationWithChildren::at(uint index) const
{
    if (index >= childCount())
        return QScriptValue();
    QScriptEngine* engine = topLevelDataInformation()->scriptEngine();
    QScriptValue childObj = engine->newQObject(childAt(index),
            QScriptEngine::QtOwnership, QScriptEngine::ExcludeDeleteLater);
    return childObj;
}

void DataInformationWithChildren::setChildren(QScriptValue children)
{
    if (children.isNull() || children.isUndefined())
        ScriptUtils::object()->logScriptError("attempting to set children of "
                + name() + "to null. This must be and error in the script.");
    ScriptValueConverter conv(children, name());
    DataInformation* convertedVal = conv.convert();

    if (!convertedVal)
    {
        ScriptUtils::object()->logScriptError("Parsing of children failed"
            " in setChildren(), please check script for errors");
        return;
    }
    //is valid now
    //childcount changed to 0
    emit childrenAboutToBeRemoved(this, 0, childCount() - 1);
    qDeleteAll(mChildren);
    mChildren.clear();
    emit childrenRemoved(this, 0, childCount() - 1);
    if (convertedVal->childCount() < 1)
        ScriptUtils::object()->logScriptError("Value from setChildren for " + name()
                + " has no children, please check script for errors");
    else
    {
        emit childrenAboutToBeInserted(this, 0, convertedVal->childCount() - 1);
        for (uint i = 0; i < convertedVal->childCount(); ++i)
        {
            DataInformation* child = convertedVal->childAt(i);
            appendChild(child);
        }
        emit childrenInserted(this, 0, convertedVal->childCount() - 1);
    }
    kDebug()
        << "setting children of " + name();
    delete convertedVal;
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

