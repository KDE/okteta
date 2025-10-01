/*
    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "datainformationwithchildren.hpp"

// lib
#include "topleveldatainformation.hpp"
#include <scriptvalueconverter.hpp>
#include <scripthandlerinfo.hpp>
#include <structunionscriptclass.hpp>
#include <scriptlogger.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QLineEdit>
// Std
#include <utility>

DataInformationWithChildren::DataInformationWithChildren(const QString& name,
                                                         std::vector<std::unique_ptr<DataInformation>>&& children, DataInformation* parent)
    : DataInformation(name, parent)
    , mChildren(std::move(children))
{
    for (const auto& child : mChildren) {
        child->setParent(this);
    }
}

DataInformationWithChildren::DataInformationWithChildren(const DataInformationWithChildren& d)
    : DataInformation(d)
    , mChildren(cloneList(d.mChildren, this))
{
}

DataInformationWithChildren::~DataInformationWithChildren() = default;

DataInformation* DataInformationWithChildren::childAt(unsigned int index) const
{
    if (index >= mChildren.size()) {
        return nullptr;
    }
    return mChildren[index].get();
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
    auto* const editWidget = new QLineEdit(parent);
    editWidget->setClearButtonEnabled(true);
    return editWidget;
}

QVariant DataInformationWithChildren::dataFromWidget(const QWidget* w) const
{
    Q_UNUSED(w)
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
    for (const auto& child : mChildren) {
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

void DataInformationWithChildren::setChildren(std::vector<std::unique_ptr<DataInformation>>&& newChildren)
{
    // since we are replacing the children and the first few may be different emit
    // change to length zero and then to new length so that model gets updated correctly
    uint numChildren = childCount();
    topLevelDataInformation()->_childCountAboutToChange(this, numChildren, 0);
    mChildren.clear();
    topLevelDataInformation()->_childCountChanged(this, numChildren, 0);

    const uint count = newChildren.size();
    topLevelDataInformation()->_childCountAboutToChange(this, 0, count);
    mChildren = std::move(newChildren);
    for (const auto& child : mChildren) {
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
    std::vector<std::unique_ptr<DataInformation>> convertedVals =
        ScriptValueConverter::convertValues(children, topLevelDataInformation()->logger());
    setChildren(std::move(convertedVals));
}

int DataInformationWithChildren::indexOf(const DataInformation* const data) const
{
    const std::size_t size = mChildren.size();
    for (std::size_t i = 0; i < size; ++i) {
        if (mChildren[i].get() == data) {
            return i;
        }
    }

    Q_ASSERT(false); // should never reach this
    return -1;
}

QVariant DataInformationWithChildren::childData(int row, int column, int role) const
{
    Q_ASSERT(0 <= row && row < static_cast<int>(mChildren.size()));
    // just delegate to child
    return mChildren[row]->data(column, role);
}

void DataInformationWithChildren::appendChild(std::unique_ptr<DataInformation>&& newChild, bool emitSignal)
{
    if (emitSignal) {
        topLevelDataInformation()->_childCountAboutToChange(this, mChildren.size(), mChildren.size() + 1);
    }
    newChild->setParent(this);
    mChildren.emplace_back(std::move(newChild));
    if (emitSignal) {
        topLevelDataInformation()->_childCountChanged(this, mChildren.size(), mChildren.size() + 1);
    }
}

bool DataInformationWithChildren::replaceChildAt(unsigned int index, std::unique_ptr<DataInformation>&& newChild)
{
    Q_ASSERT(index < mChildren.size());
    Q_CHECK_PTR(newChild);
    if (index >= mChildren.size()) {
        return false;
    }

    mChildren[index] = std::move(newChild);
    return true;
}

QScriptClass* DataInformationWithChildren::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->structUnionClass();
}

QString DataInformationWithChildren::tooltipString() const
{
    QString valueStr = mWasAbleToRead ? valueString() : eofReachedData(Qt::DisplayRole).toString();
    QString validationMsg;
    if (mHasBeenValidated && !mValidationSuccessful) {
        validationMsg = validationError();
        if (validationMsg.isEmpty()) {
            validationMsg = i18nc("not all values in this structure"
                                  " are as they should be", "Validation failed.");
        } else {
            validationMsg = i18nc("not all values in this structure"
                                  " are as they should be", "Validation failed: \"%1\"", validationMsg);
        }
    }

    return DataInformation::tooltipString(name(), valueStr, typeName(), sizeString(), childCount(),
                                          validationMsg);
}

std::vector<std::unique_ptr<DataInformation>> DataInformationWithChildren::cloneList(const std::vector<std::unique_ptr<DataInformation>>& other,
                                                                                     DataInformation* parent)
{
    std::vector<std::unique_ptr<DataInformation>> ret;
    ret.reserve(other.size());
    for (const auto& info : other) {
        std::unique_ptr<DataInformation> newChild = info->clone();
        newChild->setParent(parent);
        ret.emplace_back(std::move(newChild));
    }

    return ret;
}

unsigned int DataInformationWithChildren::childCount() const
{
    return mChildren.size();
}

bool DataInformationWithChildren::isDataInformationWithChildren() const
{
    return true;
}

bool DataInformationWithChildren::readChildren(const std::vector<std::unique_ptr<DataInformation>>& children,
                                               const Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining,
                                               quint8* bitOffset, qint64* readBitsPtr, TopLevelDataInformation* top)
{
    Q_CHECK_PTR(top);
    Q_CHECK_PTR(readBitsPtr);
    Q_ASSERT(*readBitsPtr >= 0); // otherwise we failed before

    qint64 readBits = *readBitsPtr;
    // prevent overflow
    Q_ASSERT(sizeof(qint64) == sizeof(Okteta::Address) || readBits < (qint64(std::numeric_limits<qint32>::max()) * 8));

    const quint8 origBitOffset = *bitOffset;
    Okteta::Address readBytes = (readBits + origBitOffset) / 8;
    for (std::size_t i = 0; i < children.size(); ++i) {
        DataInformation* const child = children[i].get();
        top->scriptHandler()->updateDataInformation(child);

        // next may be a dangling pointer now, reset it
        DataInformation* const newChild = children[i].get();
        if (child != newChild) {
            // logInfo() << "Child at index " << i << " was replaced.";
            top->setChildDataChanged();
        }

        const qint64 currentReadBits = newChild->readData(input, address + readBytes,
                                                          bitsRemaining - readBits, bitOffset);
        if (currentReadBits == -1) {
            *readBitsPtr = -1;
            return false; // could not read one element -> whole structure could not be read
        }

        readBits += currentReadBits;
        readBytes = (readBits + origBitOffset) / 8;
    }

    *readBitsPtr = readBits;

    return true;
}
