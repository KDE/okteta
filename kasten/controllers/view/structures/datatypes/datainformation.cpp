/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011, 2013 Alex Richardson <alex.richardson@gmx.de>
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
#include "additionaldata.h"
#include "structviewpreferences.h"
#include "../script/scriptlogger.h"
#include "../script/safereference.h"

#include <QScriptValue>
#include <QScriptEngine>

#include <KLocale>
#include <KLineEdit>
#include <KIcon>
#include <KColorScheme>

DataInformation::DataInformation(const QString& name, DataInformationBase* parent)
        : mValidationSuccessful(false), mHasBeenValidated(false), mHasBeenUpdated(false),
          mWasAbleToRead(false), mByteOrder(EndianessInherit), mLoggedData(ScriptLogger::LogInvalid),
                mParent(parent), mName(name)
{
}

DataInformation::DataInformation(const DataInformation& d)
        : mValidationSuccessful(false), mHasBeenValidated(false), mHasBeenUpdated(false), mWasAbleToRead(false),
                mByteOrder(d.mByteOrder), mLoggedData(ScriptLogger::LogInvalid), mParent(0), mName(d.mName)
{
    if (d.mAdditionalData)
        mAdditionalData.reset(new AdditionalData(*(d.mAdditionalData)));
}

DataInformation::~DataInformation()
{
    //references to this are no longer valid
    SafeReferenceHolder::instance.invalidateAll(this);
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

BitCount64 DataInformation::positionInFile(Okteta::Address start) const
{
    Q_CHECK_PTR(mParent);
    if (mParent->isTopLevel())
        return start * 8; //this is the root of the structure

    const DataInformation* par = mParent->asDataInformation();
    return par->childPosition(this, start);
}

DataInformation* DataInformation::mainStructure()
{
    Q_CHECK_PTR(mParent);
    if (mParent->isTopLevel())
        return this;
    else
        return mParent->asDataInformation()->mainStructure();

}

QString DataInformation::validationError() const
{
    AdditionalData* data = additionalData();
    if (!data)
        return QString();
    return data->validationError();
}

void DataInformation::unsetValidationError()
{
    AdditionalData* data = additionalData();
    if (data)
    {
        data->setValidationError(QString());
        //check whether we still need the AdditionalData
        if (!additionalDataNeeded(data))
            setAdditionalData(0);
    }
}

void DataInformation::setValidationError(QString errorMessage)
{
    if (errorMessage.isEmpty())
    {
        unsetValidationError();
        return;
    }
    else
    {
        mValidationSuccessful = false;
        AdditionalData* data = additionalData();
        if (data)
            data->setValidationError(errorMessage);
        else
        {
            AdditionalData* newData = new AdditionalData();
            newData->setValidationError(errorMessage);
            setAdditionalData(newData);
        }
    }
}

void DataInformation::resetValidationState()
{
    unsetValidationError();
    mHasBeenValidated = false;
    mValidationSuccessful = false;
}

void DataInformation::beginRead()
{
    mHasBeenUpdated = false;
    mWasAbleToRead = false;
    mHasBeenValidated = false;
    mLoggedData = ScriptLogger::LogInvalid;
    const uint numChildren = childCount();
    if (numChildren > 0 && childAt(0)->isDummy())
        return; //first child is dummy -> all are
    //otherwise start read for all children
    for (uint i = 0; i < childCount(); ++i)
    {
        childAt(i)->beginRead();
    }
}

ScriptLogger* DataInformation::logger() const
{
    return topLevelDataInformation()->logger();
}

bool DataInformation::additionalDataNeeded(AdditionalData* data) const
{
    Q_CHECK_PTR(data);
    return data->updateFunction().isValid()
            || data->validationFunction().isValid()
            || !data->validationError().isEmpty();
}

QScriptValue DataInformation::updateFunc() const
{
    const AdditionalData* data = additionalData();
    if (data)
        return data->updateFunction();
    return QScriptValue();
}

void DataInformation::setUpdateFunc(const QScriptValue& func)
{
    AdditionalData* data = additionalData();
    if (!func.isValid() || func.isNull() || func.isUndefined())
    {
        //reset update func to QScriptValue() and delete additionalData() if not needed anymore
        if (data)
        {
            data->setUpdateFunction(QScriptValue());
            if (!additionalDataNeeded(data))
                setAdditionalData(0); //we don't need it anymore, just delete
        }
        //if additionalData() is already null no need to do anything
        return;
    }
    else
    {
        if (!func.isFunction())
        {
            logWarn() << "cannot set update function since "
                    << func.toString() << "is not a function!";
            return;
        }
        if (data)
        {
            data->setUpdateFunction(func);
        }
        else
        {
            AdditionalData* newData = new AdditionalData();
            newData->setUpdateFunction(func);
            setAdditionalData(newData);
        }
    }
}

QScriptValue DataInformation::validationFunc() const
{
    const AdditionalData* data = additionalData();
    if (data)
        return data->validationFunction();
    return QScriptValue();
}

void DataInformation::setValidationFunc(const QScriptValue& func)
{
    AdditionalData* data = additionalData();
    if (!func.isValid() || func.isNull() || func.isUndefined())
    {
        if (data)
        {
            //reset validationfunc func to QScriptValue() and delete additionalData() if not needed anymore
            data->setValidationFunction(QScriptValue());
            if (!additionalDataNeeded(data))
                setAdditionalData(0);
        }
        //if additionalData() is already null no need to do anything
        return;
    }
    else
    {
        if (!func.isFunction())
        {
            logWarn() << "cannot set validation function since "
                    << func.toString() << "is not a function!";
            return;
        }
        if (data)
        {
            data->setValidationFunction(func);
        }
        else
        {
            AdditionalData* newData = new AdditionalData();
            newData->setValidationFunction(func);
            setAdditionalData(newData);
        }
    }
}

QVariant DataInformation::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == ColumnName)
        {
            if (mParent && mParent->isArray())
                return QString(QLatin1Char('[') + QString::number(row()) + QLatin1Char(']'));
            else
                return name();
        }
        if (column == ColumnType)
            return typeName();
        if (column == ColumnValue)
            return mWasAbleToRead ? valueString() : eofReachedData(Qt::DisplayRole);
    }
    else if (role == Qt::ToolTipRole)
    {
        return tooltipString();
    }
    else if (role == Qt::DecorationRole && column == ColumnName)
    {
        //XXX better icons?
        if (mHasBeenValidated)
            return KIcon(QLatin1String(mValidationSuccessful ? "task-complete" : "dialog-warning"));
        if (mLoggedData != ScriptLogger::LogInvalid)
            return ScriptLogger::iconForLevel(mLoggedData);
    }
    else if (!mWasAbleToRead && column == ColumnValue)
        return eofReachedData(role);
    return QVariant();
}

QVariant DataInformation::eofReachedData(int role)
{
    //whenever color scheme changes application must be restarted to have the correct color
    static KColorScheme scheme(QPalette::Active);

    if (role == Qt::DisplayRole)
        return i18nc("invalid value (End of file reached)", "<EOF reached>");
    else if (role == Qt::ForegroundRole)
        return scheme.foreground(KColorScheme::NegativeText);
    return QVariant();
}


QString DataInformation::tooltipString() const
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
            validationMsg = i18nc("not all values in this structure are as they should be",
                    "Validation failed: \"%1\"", validationMsg);
        }
        return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4\n\n%5", name(),
                valueStr, typeName(), sizeString(), validationMsg);
    }
    else
    {
        return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4", name(),
                valueStr, typeName(), sizeString());
    }
}

DataInformation* DataInformation::child(const QString& name) const
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

QString DataInformation::fullObjectPath() const
{
    DataInformationBase* par = parent();
    if (!par || par->isTopLevel())
    {
        return name();
    }
    QString result;
    if (par->isArray())
        result = QLatin1Char('[') + QString::number(par->asDataInformation()->indexOf(this))
                + QLatin1Char(']');
    else
        result = QLatin1Char('.') + name();
    result.prepend(par->asDataInformation()->fullObjectPath());
    return result;
}

void DataInformation::setAdditionalData(AdditionalData* data)
{
    mAdditionalData.reset(data);
}

QSysInfo::Endian DataInformation::effectiveByteOrder() const
{
    switch (mByteOrder)
    {
    case EndiannessBig:
        return QSysInfo::BigEndian;
    case EndianessLittle:
        return QSysInfo::LittleEndian;
    case EndianessFromSettings:
        return static_cast<QSysInfo::Endian>(Kasten2::StructViewPreferences::byteOrder());
    default: //inherit
        return (mParent && !mParent->isTopLevel()) ?
                mParent->asDataInformation()->effectiveByteOrder() :
                static_cast<QSysInfo::Endian>(Kasten2::StructViewPreferences::byteOrder());
    }
}

QScriptValue DataInformation::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    return engine->newObject(scriptClass(handlerInfo),
                             engine->newVariant(QVariant::fromValue(SafeReference(this))));
}

QScriptValue DataInformation::toScriptValue(TopLevelDataInformation* top)
{
    return toScriptValue(top->scriptEngine(), top->scriptHandler()->handlerInfo());
}
