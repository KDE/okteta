/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011, 2013 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "datainformation.hpp"

#include "topleveldatainformation.hpp"
#include "additionaldata.hpp"
#include <structureviewpreferences.h>
#include "../script/scriptlogger.hpp"
#include "../script/safereference.hpp"

#include <QScriptValue>
#include <QScriptEngine>
#include <QIcon>

#include <KLocalizedString>
#include <KColorScheme>

DataInformation::DataInformation(const QString& name, DataInformationBase* parent)
    : mParent(parent)
    , mName(name)
    , mValidationSuccessful(false)
    , mHasBeenValidated(false)
    , mHasBeenUpdated(false)
    , mWasAbleToRead(false)
    , mLoggedData(ScriptLogger::LogInvalid)
{
}

DataInformation::DataInformation(const DataInformation& d)
    : mAdditionalData(d.mAdditionalData)
    , mParent(nullptr)
    , mName(d.mName)
    , mValidationSuccessful(false)
    , mHasBeenValidated(false)
    , mHasBeenUpdated(false)
    , mWasAbleToRead(false)
    , mByteOrder(d.mByteOrder)
    , mLoggedData(ScriptLogger::LogInvalid)
{
}

DataInformation::~DataInformation()
{
    // references to this are no longer valid
    SafeReferenceHolder::instance.invalidateAll(this);
}

QString DataInformation::sizeString() const
{
    if (size() % 8 == 0) { // no bits remaining
        return i18np("1 byte", "%1 bytes", size() / 8);
    }

    QString bytes = i18np("1 byte", "%1 bytes", size() / 8);
    QString bits = i18np("1 bit", "%1 bits", size() % 8);
    return i18nc("number of bytes, then number of bits", "%1 %2", bytes, bits);
}

QString DataInformation::valueStringImpl() const
{
    return {};
}

BitCount64 DataInformation::positionInFile(Okteta::Address start) const
{
    Q_CHECK_PTR(mParent);
    if (mParent->isTopLevel()) {
        return start * 8; // this is the root of the structure

    }
    const DataInformation* par = mParent->asDataInformation();
    return par->childPosition(this, start);
}

DataInformation* DataInformation::mainStructure()
{
    Q_CHECK_PTR(mParent);
    if (mParent->isTopLevel()) {
        return this;
    }

    return mParent->asDataInformation()->mainStructure();
}

void DataInformation::setValidationError(const QString& errorMessage)
{
    if (errorMessage.isEmpty()) {
        mAdditionalData.remove(AdditionalData::AdditionalDataType::ValidationError);
    } else {
        mAdditionalData.set(AdditionalData::AdditionalDataType::ValidationError, errorMessage);
    }
}

void DataInformation::setCustomTypeName(const QString& customTypeName)
{
    if (customTypeName.isEmpty()) {
        mAdditionalData.remove(AdditionalData::AdditionalDataType::CustomTypeName);
    } else {
        mAdditionalData.set(AdditionalData::AdditionalDataType::CustomTypeName, customTypeName);
    }
}

void DataInformation::resetValidationState()
{
    mAdditionalData.remove(AdditionalData::AdditionalDataType::ValidationError);
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
    if (numChildren > 0 && childAt(0)->isDummy()) {
        return; // first child is dummy -> all are
    }
    // otherwise start read for all children
    for (uint i = 0; i < childCount(); ++i) {
        childAt(i)->beginRead();
    }
}

ScriptLogger* DataInformation::logger() const
{
    return topLevelDataInformation()->logger();
}

void DataInformation::setAdditionalFunction(AdditionalData::AdditionalDataType entry, const QScriptValue& value,
                                            const char* name)
{
    if (!value.isValid() || value.isNull() || value.isUndefined()) {
        // removal requested
        mAdditionalData.remove(entry);
        return;
    }
    if (!value.isFunction()) {
        logError() << "cannot set" << name << "since" << value.toString() << "is not a function!";
        return;
    }
    mAdditionalData.set(entry, QVariant::fromValue(value));
}

QVariant DataInformation::data(int column, int role) const
{
    if (role == Qt::DisplayRole) {
        if (column == ColumnName) {
            if (mParent && mParent->isArray()) {
                return QString(QLatin1Char('[') + QString::number(row()) + QLatin1Char(']'));
            }

            return name();
        }
        if (column == ColumnType) {
            return typeName();
        }
        if (column == ColumnValue) {
            return mWasAbleToRead ? valueString() : eofReachedData(Qt::DisplayRole);
        }
    } else if (role == Qt::ToolTipRole) {
        return tooltipString();
    } else if (role == Qt::DecorationRole && column == ColumnName) {
        // XXX better icons?
        if (mHasBeenValidated) {
            return QIcon::fromTheme(mValidationSuccessful ? QStringLiteral("task-complete") : QStringLiteral("dialog-warning"));
        }
        if (mLoggedData != ScriptLogger::LogInvalid) {
            return ScriptLogger::iconForLevel(mLoggedData);
        }
    } else if (!mWasAbleToRead && column == ColumnValue) {
        return eofReachedData(role);
    }
    return {};
}

QVariant DataInformation::eofReachedData(int role)
{
    // whenever color scheme changes application must be restarted to have the correct color
    static KColorScheme scheme(QPalette::Active);

    if (role == Qt::DisplayRole) {
        return i18nc("invalid value (End of file reached)", "<EOF reached>");
    }
    if (role == Qt::ForegroundRole) {
        return scheme.foreground(KColorScheme::NegativeText);
    }
    return {};
}

QString DataInformation::tooltipString() const
{
    QString valueStr = mWasAbleToRead ? valueString() : eofReachedData(Qt::DisplayRole).toString();
    if (mHasBeenValidated && !mValidationSuccessful) {
        QString validationMsg = validationError();
        if (validationMsg.isEmpty()) {
            validationMsg = i18nc("not all values in this structure"
                                  " are as they should be", "Validation failed.");
        } else {
            validationMsg = i18nc("not all values in this structure are as they should be",
                                  "Validation failed: \"%1\"", validationMsg);
        }
        return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4\n\n%5", name(),
                    valueStr, typeName(), sizeString(), validationMsg);
    }

    return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4", name(),
                valueStr, typeName(), sizeString());
}

DataInformation* DataInformation::child(const QString& name) const
{
    int size = childCount();
    for (int i = 0; i < size; ++i) {
        DataInformation* child = childAt(i);
        if (child->name() == name) {
            return child;
        }
    }

    return nullptr;
}

TopLevelDataInformation* DataInformation::topLevelDataInformation() const
{
    Q_CHECK_PTR(mParent);
    if (mParent->isTopLevel()) {
        return mParent->asTopLevel();
    }

    return mParent->asDataInformation()->topLevelDataInformation();
}

int DataInformation::row() const
{
    Q_CHECK_PTR(mParent);
    if (mParent->isTopLevel()) {
        return mParent->asTopLevel()->indexOf(this);
    }

    return mParent->asDataInformation()->indexOf(this);
}

QString DataInformation::fullObjectPath() const
{
    DataInformationBase* par = parent();
    if (!par || par->isTopLevel()) {
        return name();
    }
    QString result;
    if (par->isArray()) {
        result = QLatin1Char('[') + QString::number(par->asDataInformation()->indexOf(this))
                 + QLatin1Char(']');
    } else {
        result = QLatin1Char('.') + name();
    }
    result.prepend(par->asDataInformation()->fullObjectPath());
    return result;
}

QScriptValue DataInformation::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    return engine->newObject(scriptClass(handlerInfo),
                             engine->newVariant(QVariant::fromValue(SafeReference(this))));
}

QSysInfo::Endian DataInformation::byteOrderFromSettings() const
{
    return Kasten::StructureViewPreferences::byteOrder();
}

QScriptValue DataInformation::toScriptValue(TopLevelDataInformation* top)
{
    return toScriptValue(top->scriptEngine(), top->scriptHandler()->handlerInfo());
}

QString DataInformation::customToString(const QScriptValue& func) const
{
    if (!wasAbleToRead()) {
        logError() << "Attempting to call custom to string function, but element could not be read";
        return valueStringImpl();
    }
    Q_ASSERT(func.isFunction());
    return topLevelDataInformation()->scriptHandler()->customToString(this, func);
}
