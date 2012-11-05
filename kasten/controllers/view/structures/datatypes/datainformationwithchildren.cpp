/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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
#include "../parsers/scriptvalueconverter.h"
#include "../script/scriptutils.h"
#include "../script/scripthandlerinfo.h"
#include "../script/classes/structunionscriptclass.h"
#include "../script/scriptlogger.h"

#include <KLineEdit>
#include <KLocalizedString>
#include <QScriptEngine>

DataInformation* DataInformationWithChildren::childAt(unsigned int idx) const
{
    if (idx >= (unsigned) mChildren.size())
        return 0;
    return mChildren[idx];
}

bool DataInformationWithChildren::setData(const QVariant&, Okteta::AbstractByteArrayModel*,
        Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT_X(false, "DataInformationWithChildren::setData()", "this should never be called");
    return false;
}

DataInformationWithChildren::~DataInformationWithChildren()
{
    qDeleteAll(mChildren);
}

DataInformationWithChildren::DataInformationWithChildren(const QString& name,
        const QVector<DataInformation*>& children, DataInformation* parent)
        : DataInformation(name, parent), mChildren(children)
{
    for (int i = 0; i < mChildren.size(); ++i)
        mChildren.at(i)->setParent(this);
}

DataInformationWithChildren::DataInformationWithChildren(const DataInformationWithChildren& d)
        : DataInformation(d), mChildren(cloneList(d.mChildren, this))
{
}

QWidget* DataInformationWithChildren::createEditWidget(QWidget* parent) const
{
    Q_ASSERT(false);
    return new KLineEdit(parent);
}

QVariant DataInformationWithChildren::dataFromWidget(const QWidget* w) const
{
    Q_UNUSED(w);
    Q_ASSERT(false);
    return QVariant();
}

void DataInformationWithChildren::setWidgetData(QWidget* w) const
{
    Q_ASSERT(false);
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

void DataInformationWithChildren::resetValidationState()
{
    unsetValidationError();
    mHasBeenValidated = false;
    mValidationSuccessful = false;
    for (int i = 0; i < mChildren.size(); ++i)
    {
        mChildren.at(i)->resetValidationState();
    }
}

void DataInformationWithChildren::calculateValidationState()
{
    if (childCount() > 0)
    {
        bool hasValidatedChildren = false;
        bool allChildrenValid = true;
        for (uint i = 0; i < childCount(); ++i)
        {
            DataInformation* child = childAt(i);
            DataInformationWithChildren* childWithChildren =
                    dynamic_cast<DataInformationWithChildren*>(child);
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
            mValidationSuccessful = allChildrenValid;
        }
    }
}

void DataInformationWithChildren::setChildren(const QVector<DataInformation*>& newChildren)
{
    //since we are replacing the children and the first few may be different emit
    //change to length zero and then to new length so that model gets updated correctly
    uint numChildren = childCount();
    topLevelDataInformation()->_childCountAboutToChange(this, numChildren, 0);
    qDeleteAll(mChildren);
    mChildren.clear();
    topLevelDataInformation()->_childCountChanged(this, numChildren, 0);

    const uint count = newChildren.size();
    topLevelDataInformation()->_childCountAboutToChange(this, 0, count);
    mChildren = newChildren;
    for (int i = 0; i < mChildren.size(); ++i)
        mChildren.at(i)->setParent(this);
    topLevelDataInformation()->_childCountChanged(this, 0, count);
}

void DataInformationWithChildren::setChildren(QScriptValue children)
{
    if (children.isNull() || children.isUndefined())
    {
        logError() << "attempting to set children to null/undefined.";
        return;
    }
    QVector<DataInformation*> convertedVals =
            ScriptValueConverter::convertValues(children, topLevelDataInformation()->logger());
    setChildren(convertedVals);
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
    Q_ASSERT(false); //should never reach this
    return -1;
}

QVariant DataInformationWithChildren::childData(int row, int column, int role) const
{
    Q_ASSERT(row >= 0 && row < mChildren.size());
    //just delegate to child
    return mChildren.at(row)->data(column, role);
}

void DataInformationWithChildren::appendChild(DataInformation* newChild, bool emitSignal)
{
    if (emitSignal)
        topLevelDataInformation()->_childCountAboutToChange(this, mChildren.size(), mChildren.size() + 1);
    newChild->setParent(this);
    mChildren.append(newChild);
    if (emitSignal)
        topLevelDataInformation()->_childCountChanged(this, mChildren.size(), mChildren.size() + 1);
}

void DataInformationWithChildren::appendChildren(const QVector<DataInformation*>& newChildren, bool emitSignal)
{
    if (newChildren.isEmpty())
        return;
    const int added = newChildren.size();
    if (emitSignal)
        topLevelDataInformation()->_childCountAboutToChange(this, mChildren.size(), mChildren.size() + added);
    for (int i = 0; i < newChildren.size(); ++i)
        newChildren.at(i)->setParent(this);
    mChildren << newChildren;
    if (emitSignal)
        topLevelDataInformation()->_childCountChanged(this, mChildren.size(), mChildren.size() + added);
}

bool DataInformationWithChildren::replaceChildAt(unsigned int index, DataInformation* newChild)
{
    Q_ASSERT(index < uint(mChildren.size()));
    Q_CHECK_PTR(newChild);
    if (index >= uint(mChildren.size()))
        return false;

    delete mChildren.at(index);
    mChildren[index] = newChild;
    return true;
}

QScriptClass* DataInformationWithChildren::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mStructUnionClass.data();
}

QString DataInformationWithChildren::tooltipString() const
{
    QString valueStr = mWasAbleToRead ? valueString() : eofReachedData(Qt::DisplayRole).toString();
    if (mHasBeenValidated && !mValidationSuccessful)
    {
        QString validationMsg = validationError();
        if (validationMsg.isEmpty())
        {
            validationMsg = i18nc("not all values in this structure"
                    " are as they should be", "Validation failed.");
        }
        else
        {
            validationMsg = i18nc("not all values in this structure"
                    " are as they should be", "Validation failed: \"%1\"", validationMsg);
        }
        return i18np("Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)\n\n %6",
                "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)\n\n %6",
                childCount(), name(), valueStr, typeName(), sizeString(), validationMsg);
    }
    else
    {
        return i18np("Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)",
                "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)",
                childCount(), name(), valueStr, typeName(), sizeString());
    }
}

QVector<DataInformation*> DataInformationWithChildren::cloneList(const QVector<DataInformation*>& other,
        DataInformation* parent)
{
    int count = other.count();
    QVector<DataInformation*> ret;
    ret.reserve(count);
    for (int i = 0; i < count; ++i)
    {
        DataInformation* dat = other.at(i);
        DataInformation* newChild = dat->clone();
        newChild->setParent(parent);
        ret.append(newChild);
    }
    return ret;
}
