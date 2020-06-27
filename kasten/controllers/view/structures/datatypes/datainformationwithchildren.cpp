/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "datainformationwithchildren.hpp"
#include "topleveldatainformation.hpp"
#include "../parsers/scriptvalueconverter.hpp"
#include "../script/scriptutils.hpp"
#include "../script/scripthandlerinfo.hpp"
#include "../script/classes/structunionscriptclass.hpp"
#include "../script/scriptlogger.hpp"

#include <KLocalizedString>
#include <QScriptEngine>
#include <QLineEdit>

DataInformationWithChildren::DataInformationWithChildren(const QString& name,
                                                         const QVector<DataInformation*>& children, DataInformation* parent)
    : DataInformation(name, parent)
    , mChildren(children)
{
    for (auto* child : qAsConst(mChildren)) {
        child->setParent(this);
    }
}

DataInformationWithChildren::DataInformationWithChildren(const DataInformationWithChildren& d)
    : DataInformation(d)
    , mChildren(cloneList(d.mChildren, this))
{
}

DataInformationWithChildren::~DataInformationWithChildren()
{
    qDeleteAll(mChildren);
}

DataInformation* DataInformationWithChildren::childAt(unsigned int idx) const
{
    if (idx >= (unsigned) mChildren.size()) {
        return nullptr;
    }
    return mChildren[idx];
}

bool DataInformationWithChildren::setData(const QVariant&, Okteta::AbstractByteArrayModel*,
                                          Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT_X(false, "DataInformationWithChildren::setData()", "this should never be called");
    return false;
}

QWidget* DataInformationWithChildren::createEditWidget(QWidget* parent) const
{
    Q_ASSERT(false);
    auto* editWidget = new QLineEdit(parent);
    editWidget->setClearButtonEnabled(true);
    return editWidget;
}

QVariant DataInformationWithChildren::dataFromWidget(const QWidget* w) const
{
    Q_UNUSED(w);
    Q_ASSERT(false);
    return {};
}

void DataInformationWithChildren::setWidgetData(QWidget* w) const
{
    Q_ASSERT(false);
    Q_UNUSED(w)
}

BitCount32 DataInformationWithChildren::size() const
{
    BitCount32 size = 0;
    for (unsigned int i = 0; i < childCount(); ++i) {
        size += childAt(i)->size();
    }

    return size;
}

void DataInformationWithChildren::resetValidationState()
{
    DataInformation::resetValidationState();
    for (auto* child : qAsConst(mChildren)) {
        child->resetValidationState();
    }
}

void DataInformationWithChildren::calculateValidationState()
{
    if (childCount() > 0) {
        bool hasValidatedChildren = false;
        bool allChildrenValid = true;
        for (uint i = 0; i < childCount(); ++i) {
            DataInformation* child = childAt(i);
            DataInformationWithChildren* childWithChildren = child->asDataInformationWithChildren();
            if (childWithChildren) {
                childWithChildren->calculateValidationState();
            }
            // first make sure the child item validation state has been set
            if (child->hasBeenValidated()) {
                hasValidatedChildren = true;
                if (!child->validationSuccessful()) {
                    allChildrenValid = false;
                    break; // one is invalid -> whole structure is invalid
                }
            }
        }

        if (hasValidatedChildren) {
            mValidationSuccessful = allChildrenValid;
        }
    }
}

void DataInformationWithChildren::setChildren(const QVector<DataInformation*>& newChildren)
{
    // since we are replacing the children and the first few may be different emit
    // change to length zero and then to new length so that model gets updated correctly
    uint numChildren = childCount();
    topLevelDataInformation()->_childCountAboutToChange(this, numChildren, 0);
    qDeleteAll(mChildren);
    mChildren.clear();
    topLevelDataInformation()->_childCountChanged(this, numChildren, 0);

    const uint count = newChildren.size();
    topLevelDataInformation()->_childCountAboutToChange(this, 0, count);
    mChildren = newChildren;
    for (auto* child : qAsConst(mChildren)) {
        child->setParent(this);
    }

    topLevelDataInformation()->_childCountChanged(this, 0, count);
}

void DataInformationWithChildren::setChildren(const QScriptValue& children)
{
    if (children.isNull() || children.isUndefined()) {
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
    for (int i = 0; i < size; ++i) {
        if (mChildren.at(i) == data) {
            return i;
        }
    }

    Q_ASSERT(false); // should never reach this
    return -1;
}

QVariant DataInformationWithChildren::childData(int row, int column, int role) const
{
    Q_ASSERT(row >= 0 && row < mChildren.size());
    // just delegate to child
    return mChildren.at(row)->data(column, role);
}

void DataInformationWithChildren::appendChild(DataInformation* newChild, bool emitSignal)
{
    if (emitSignal) {
        topLevelDataInformation()->_childCountAboutToChange(this, mChildren.size(), mChildren.size() + 1);
    }
    newChild->setParent(this);
    mChildren.append(newChild);
    if (emitSignal) {
        topLevelDataInformation()->_childCountChanged(this, mChildren.size(), mChildren.size() + 1);
    }
}

void DataInformationWithChildren::appendChildren(const QVector<DataInformation*>& newChildren, bool emitSignal)
{
    if (newChildren.isEmpty()) {
        return;
    }
    const int added = newChildren.size();
    if (emitSignal) {
        topLevelDataInformation()->_childCountAboutToChange(this, mChildren.size(), mChildren.size() + added);
    }
    for (auto* child : newChildren) {
        child->setParent(this);
    }

    mChildren << newChildren;
    if (emitSignal) {
        topLevelDataInformation()->_childCountChanged(this, mChildren.size(), mChildren.size() + added);
    }
}

bool DataInformationWithChildren::replaceChildAt(unsigned int index, DataInformation* newChild)
{
    Q_ASSERT(index < uint(mChildren.size()));
    Q_CHECK_PTR(newChild);
    if (index >= uint(mChildren.size())) {
        return false;
    }

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
    if (mHasBeenValidated && !mValidationSuccessful) {
        QString validationMsg = validationError();
        if (validationMsg.isEmpty()) {
            validationMsg = i18nc("not all values in this structure"
                                  " are as they should be", "Validation failed.");
        } else {
            validationMsg = i18nc("not all values in this structure"
                                  " are as they should be", "Validation failed: \"%1\"", validationMsg);
        }
        return i18np("Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)\n\n %6",
                     "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)\n\n %6",
                     childCount(), name(), valueStr, typeName(), sizeString(), validationMsg);
    }

    return i18np("Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 child)",
                 "Name: %2\nValue: %3\n\nType: %4\nSize: %5 (%1 children)",
                 childCount(), name(), valueStr, typeName(), sizeString());
}

QVector<DataInformation*> DataInformationWithChildren::cloneList(const QVector<DataInformation*>& other,
                                                                 DataInformation* parent)
{
    int count = other.count();
    QVector<DataInformation*> ret;
    ret.reserve(count);
    for (int i = 0; i < count; ++i) {
        DataInformation* dat = other.at(i);
        DataInformation* newChild = dat->clone();
        newChild->setParent(parent);
        ret.append(newChild);
    }

    return ret;
}

unsigned int DataInformationWithChildren::childCount() const
{
    return mChildren.size();
}

bool DataInformationWithChildren::canHaveChildren() const
{
    return true;
}

bool DataInformationWithChildren::isDataInformationWithChildren() const
{
    return true;
}
