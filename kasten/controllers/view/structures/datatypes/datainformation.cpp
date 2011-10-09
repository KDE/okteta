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
#include "datainformation.h"
#include "topleveldatainformation.h"

#include <KLocale>
#include <KLineEdit>
#include <KIcon>


DataInformation::DataInformation(const QString& name, DataInformationBase* parent) :
    mValidationSuccessful(false), mHasBeenValidated(false), mWasAbleToRead(false),
    mByteOrder(EndianessInherit), mAdditionalData(0), mParent(parent), mName(name)
{
}

DataInformation::DataInformation(const DataInformation& d) :
            mValidationSuccessful(d.mValidationSuccessful),
            mHasBeenValidated(d.mHasBeenValidated), mWasAbleToRead(d.mWasAbleToRead),
            mByteOrder(d.mByteOrder), mAdditionalData(0), mParent(0), mName(d.mName)
{
    if (d.mAdditionalData)
        mAdditionalData = new AdditionalData(*(d.mAdditionalData));
}

DataInformation::~DataInformation()
{
    delete mAdditionalData;
}

QString DataInformation::valueString() const
{
    return QString();
}

QString DataInformation::sizeString() const
{
    if (size() % 8 == 0) //no bits remaining
    {
        return i18np("1 byte", "%1 bytes", size() / 8);
    }
    else
    {
        QString bytes = i18np("1 byte", "%1 bytes", size() / 8);
        QString bits = i18np("1 bit", "%1 bits", size() % 8);
        return i18nc("number of bytes, then number of bits", "%1 %2", bytes, bits);
    }
}

BitCount32 DataInformation::positionRelativeToRoot(int index) const
{
    Q_CHECK_PTR(mParent);
    //FIXME this needs updating to support bitfield marking
    if (mParent->isTopLevel())
        return 0;

    //TODO add a method offset(const DataInformation* const) for efficiency
    DataInformation* par = mParent->asDataInformation();
    //row defaults to -1
    return par->offset(index < 0 ? row() : index) + par->positionRelativeToRoot();
}

DataInformation* DataInformation::mainStructure()
{
    Q_CHECK_PTR(mParent);

    if (mParent->isTopLevel())
        return this;
    else
        return mParent->asDataInformation()->mainStructure();

}

void DataInformation::setAdditionalData(AdditionalData* data)
{
    mAdditionalData = data;
}

QString DataInformation::validationError() const
{
    if (!mAdditionalData)
        return QString();
    return mAdditionalData->validationError();
}

void DataInformation::setValidationError(QString errorMessage)
{
    setValidationSuccessful(false);
    if (!mAdditionalData)
        mAdditionalData = new AdditionalData();
    mAdditionalData->setValidationError(errorMessage);
}

bool DataInformation::validationSuccessful() const
{
    return mValidationSuccessful;
}

void DataInformation::setValidationSuccessful(bool successful)
{
    mValidationSuccessful = successful;
    setHasBeenValidated(true);
}

bool DataInformation::hasBeenValidated() const
{
    return mHasBeenValidated;
}

void DataInformation::setHasBeenValidated(bool hasBeen)
{
    mHasBeenValidated = hasBeen;
}

void DataInformation::resetValidationState()
{
    mHasBeenValidated = false;
    mValidationSuccessful = false;
    if (mAdditionalData)
        mAdditionalData->setValidationError(QString());
}
void DataInformation::beginRead()
{
    for (uint i = 0; i < childCount(); ++i)
    {
        childAt(i)->beginRead();
    }
    mWasAbleToRead = false;
}

int DataInformation::indexOf(const DataInformation* const data) const
{
    Q_UNUSED(data)
    Q_ASSERT_X(false, "DataInformation::indexOf", "this should never happen!");
    return 0;
}

QVariant DataInformation::childData(int row, int column, int role) const
{
    Q_ASSERT_X(false, "DataInformation::childData", "this should never happen!");
    Q_UNUSED(row)
    Q_UNUSED(column)
    Q_UNUSED(role)
    return QVariant(); //no children -> no child data
}

QVariant DataInformation::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == ColumnName)
            return name();
        if (column == ColumnType)
            return typeName();
        if (column == ColumnValue)
            return valueString();
    }
    else if (role == Qt::ToolTipRole)
    {
        return tooltipString();
    }
    else if (role == Qt::DecorationRole && column == ColumnName)
    {
        //XXX better icons?
        if (mHasBeenValidated)
            return KIcon(QLatin1String(mValidationSuccessful ? "task-complete":"dialog-warning"));
    }
    return QVariant();
}


QString DataInformation::tooltipString() const
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
        return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4\n\n%5", name(),
                valueString(), typeName(), sizeString(), validationError);
    }
    else
    {
        return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4", name(),
                    valueString(), typeName(), sizeString());
    }
}

Qt::ItemFlags DataInformation::childFlags(int row, int column, bool fileLoaded) const
{
    Q_ASSERT_X(false, "DataInformation::childFlags()", "Only subclass versions of this should be called");
    return childAt(row)->flags(column, fileLoaded);
}

DataInformation* DataInformation::child(QString name) const
{
    int size = childCount();
    for (int i = 0; i < size; ++i)
    {
        DataInformation* child = childAt(i);
        if (child->name() == name)
            return child;
    }
    return 0;
}

QPair<DataInformation*, QString> DataInformation::findChildForDynamicArrayLength(const QString& name, uint upTo) const
{
    Q_ASSERT(upTo <= childCount());
    for (int i = upTo - 1; i >= 0; --i) {
        DataInformation* current = childAt(i);
        QString start = name;
        if (current->canHaveChildren()) {
            QPair<DataInformation*, QString> tmp = findChildForDynamicArrayLength(name, current->childCount());
            current = tmp.first;
            if (current)
                start = start + tmp.second;
        }
        if (current && current->name() == name) {
            return qMakePair(current, QString(start + QLatin1String(".value")));
        }
    }
    if (!parent() || parent()->isTopLevel())
        return QPair<DataInformation*, QString>(0, QString());

    QPair<DataInformation*, QString> ret =
            parent()->asDataInformation()->findChildForDynamicArrayLength(name, row());
    if (ret.first)
    {
        //found one
        return qMakePair(ret.first, QString(QLatin1String("parent.") + ret.second));
    }
    else
        return QPair<DataInformation*, QString>(0, QString());
}

TopLevelDataInformation* DataInformation::topLevelDataInformation() const
{
    Q_CHECK_PTR(mParent);
    if (mParent->isTopLevel())
        return mParent->asTopLevel();

    return mParent->asDataInformation()->topLevelDataInformation();
}

int DataInformation::row() const
{
    Q_CHECK_PTR(mParent);
    if (mParent->isTopLevel())
        return mParent->asTopLevel()->indexOf(this);
    else
        return mParent->asDataInformation()->indexOf(this);
}

QWidget* DataInformation::createChildEditWidget(uint index, QWidget* parent) const
{
    Q_ASSERT_X(false, "DataInformation::createChildEditWidget",
            "only implemented so not all subclasses have to, since it will never be called there. Error!");
    return 0;

}

void DataInformation::setChildWidgetData(uint index, QWidget* w) const
{
    Q_ASSERT_X(false, "DataInformation::setChildWidgetData",
            "only implemented so not all subclasses have to, since it will never be called there. Error!");
}

QVariant DataInformation::dataFromChildWidget(uint index, const QWidget* w) const
{
    Q_ASSERT_X(false, "DataInformation::dataFromChildWidget",
            "only implemented so not all subclasses have to, since it will never be called there. Error!");
    return QVariant();
}
