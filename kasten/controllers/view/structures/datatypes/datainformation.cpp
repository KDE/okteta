/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
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
#include "datainformation.h"

#include <KLocale>
#include <KLineEdit>


DataInformation::DataInformation(const QString& name, DataInformation* parent) :
    QObject(parent), mValidationSuccessful(false), mHasBeenValidated(
            false), mWasAbleToRead(false), mByteOrder(EndianessInherit), mAdditionalData(NULL)
{
    setObjectName(name);
}

DataInformation::DataInformation(const DataInformation& d) :
    QObject(NULL), QScriptable(), mValidationSuccessful(d.mValidationSuccessful),
            mHasBeenValidated(d.mHasBeenValidated), mWasAbleToRead(d.mWasAbleToRead), 
            mByteOrder(d.mByteOrder), mAdditionalData(NULL)            
{
    setObjectName(d.objectName());
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

quint64 DataInformation::positionRelativeToParent() const
{
    //FIXME this needs updating to support bitfield marking
    DataInformation* par = dynamic_cast<DataInformation*> (parent());
    if (!par)
    {
        return 0;
    }
    //TODO add a method offset(const DataInformation* const) for efficiency
    return par->offset(row()) + par->positionRelativeToParent();
}

TopLevelDataInformation* DataInformation::topLevelDataInformation() const
{
    DataInformation* par = dynamic_cast<DataInformation*> (parent());
    if (par)
        return par->topLevelDataInformation();
    else
    {
        TopLevelDataInformation* top =
                dynamic_cast<TopLevelDataInformation*> (parent());
        if (top)
            return top;
    }
    return NULL;
}

DataInformation* DataInformation::mainStructure()
{
    DataInformation* par = dynamic_cast<DataInformation*> (parent());
    //the cast fails if the parent is a TopLevelDataInformation
    //, so we can tell when the top has been reached
    if (par)
        return par->mainStructure();
    else
        return this;

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
    Q_ASSERT(false); //this should never happen!
    return 0;
}

QVariant DataInformation::childData(int row, int column, int role) const
{
    kDebug() << "invalid call!";
    Q_UNUSED(row)
    Q_UNUSED(column)
    Q_UNUSED(role)
    return QVariant(); //no children -> no child data
}
