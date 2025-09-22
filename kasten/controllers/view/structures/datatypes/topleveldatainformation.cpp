/*
    SPDX-FileCopyrightText: 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "topleveldatainformation.hpp"

// lib
#include "datainformation.hpp"
#include "datainformationwithchildren.hpp"
#include <pointerdatainformation.hpp>
#include <scripthandler.hpp>
#include <scriptlogger.hpp>
#include <scriptengineinitializer.hpp>
#include <primitivefactory.hpp>
#include <structureslogging.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QScriptEngine>
// Std
#include <limits>

const Okteta::Address TopLevelDataInformation::INVALID_OFFSET = std::numeric_limits<Okteta::Address>::max();

TopLevelDataInformation::TopLevelDataInformation(std::unique_ptr<DataInformation>&& data,
                                                 ScriptLogger* logger,
                                                 std::unique_ptr<QScriptEngine>&& engine, const QFileInfo& structureFile)
    : QObject(nullptr)
    , mData(std::move(data))
    , mLogger(logger)
    , mStructureFile(structureFile)
    , mValid(!mData->isDummy())
    , mChildDataChanged(false)
{
    Q_CHECK_PTR(mData);
    mData->setParent(this);
    setObjectName(mData->name());

    if (!mLogger) {
        mLogger = std::make_unique<ScriptLogger>();
    }

    std::unique_ptr<QScriptEngine> eng = std::move(engine);
    if (!eng) {
        eng = ScriptEngineInitializer::newEngine();
    }
    mScriptHandler = std::make_unique<ScriptHandler>(std::move(eng), this);
}

TopLevelDataInformation::~TopLevelDataInformation() = default;

void TopLevelDataInformation::validate()
{
    logger()->info(mData.get()) << "Validation requested.";
    mScriptHandler->validateData(mData.get());
}

QScriptEngine* TopLevelDataInformation::scriptEngine() const
{
    return mScriptHandler->engine();
}

void TopLevelDataInformation::resetValidationState()
{
    mData->resetValidationState();
}

void TopLevelDataInformation::read(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                                   const Okteta::ArrayChangeMetricsList& changesList, bool forceRead)
{
    mChildDataChanged = false;

    if (isLockedFor(input)) {
        address = lockPositionFor(input);
    }

    const bool updateNeccessary = forceRead || isReadingNecessary(input, address, changesList);
    if (!updateNeccessary) {
        return;
    }

    quint64 remainingBits = BitCount64(input->size() - address) * 8;
    quint8 bitOffset = 0;
    mData->beginRead(); // before reading set wasAbleToRead to false
    mData->resetValidationState(); // reading new data -> validation state is old
    const DataInformation* oldData = mData.get();
    mScriptHandler->updateDataInformation(mData.get()); // unlikely that this is useful, but maybe someone needs it
    if (mData.get() != oldData) {
        mLogger->info() << "Main element was replaced!";
    }
    mData->readData(input, address, remainingBits, &bitOffset);

    // Read all the delayed PointerDataInformation
    // We do this because the pointed data is independent from the
    // structure containing the pointer and so we can use fields
    // which come after the pointer itself in the structure
    while (!mDelayedRead.isEmpty()) {
        mDelayedRead.dequeue()->delayedReadData(input, address);
    }

    if (mChildDataChanged) {
        Q_EMIT dataChanged();
        mChildDataChanged = false;
    }
    mLastModel = input;
    mLastReadOffset = address;
}

void TopLevelDataInformation::enqueueReadData(PointerDataInformation* toRead)
{
    mDelayedRead.append(toRead);
}

bool TopLevelDataInformation::isReadingNecessary(const Okteta::AbstractByteArrayModel* model,
                                                 Okteta::Address address, const Okteta::ArrayChangeMetricsList& changesList)
{
    if (model != mLastModel) {
        return true; // whenever we have a new model we have to read

    }

    if (address != mLastReadOffset) {
        return true; // address as changed, we have to read again

    }
    // address has not changed, check whether the changes affect us

    // TODO always return true if structure contains pointers
    if (changesList.isEmpty()) {
        return false; // no changes
    }
    const Okteta::Address structureSizeInBytes = (mData->size() / 8) + (mData->size() % 8 == 0 ? 0 : 1);
    const Okteta::Address end = address + structureSizeInBytes;
    for (int i = 0; i < changesList.length(); ++i) {
        const Okteta::ArrayChangeMetrics& change = changesList.at(i);
        // this is valid for all types
        if (change.offset() >= end) {
            // insertion/deletion/swapping is after end of structure, it doesn't interest us
            continue;
        }
        // now handle special cases
        if (change.type() == Okteta::ArrayChangeMetrics::Replacement) {
            // handle it for replacements
            if (change.lengthChange() == 0 && change.offset() + change.removeLength() <= address) {
                // no length change and it doesn't affect structure since it is before
                // could use insertLength() instead of removeLength() since they are the same
                continue;
            }
            // something was removed/inserted before start of structure
            // which means all bytes have moved forwards/backwards: reread all
            return true;
        }
        if (change.type() == Okteta::ArrayChangeMetrics::Swapping) {
            if (change.secondEnd() < address) {
                // swapped ranges end before start, i.e. the range of interest does not change
                continue;
            }
            // Not sure what other possibilities there are, but rather waste CPU or I/O rereading
            // than showing outdated values
            return true;
        }

        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Invalid change";
        continue;
    }

    return false; // nothing affected us
}

void TopLevelDataInformation::lockPositionToOffset(Okteta::Address offset, const Okteta::AbstractByteArrayModel* model)
{
    if (offset == INVALID_OFFSET) {
        // we use Okteta::Address max to indicate not locked -> error out
        mLogger->error() << "Attempting to lock at max address value, this is forbidden.";
        return;
    }
    auto it = mLockedPositions.find(model);
    if (it != mLockedPositions.end()) {
        it.value() = offset;
    } else {
        mLockedPositions.insert(model, offset);
        // remove when deleted
        connect(model, &Okteta::AbstractByteArrayModel::destroyed,
                this, &TopLevelDataInformation::removeByteArrayModelFromList);
    }
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
        << mData->name() << ": Locking start offset in model" << model << "to position" << Qt::hex << offset;
}

void TopLevelDataInformation::unlockPosition(const Okteta::AbstractByteArrayModel* model)
{
    Q_ASSERT(mLockedPositions.contains(model) && mLockedPositions.value(model) != INVALID_OFFSET);
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
        << "removing lock at position" << mLockedPositions.value(model) << ", model=" << model;
    mLockedPositions.remove(model);
    disconnect(model, &Okteta::AbstractByteArrayModel::destroyed,
               this, &TopLevelDataInformation::removeByteArrayModelFromList);
}

void TopLevelDataInformation::removeByteArrayModelFromList(QObject* obj)
{
    const Okteta::AbstractByteArrayModel* model = static_cast<Okteta::AbstractByteArrayModel*>(obj);
    mLockedPositions.remove(model);
}

bool TopLevelDataInformation::isLockedByDefault() const
{
    return mDefaultLockOffset != INVALID_OFFSET;
}

void TopLevelDataInformation::setDefaultLockOffset(Okteta::Address offset)
{
    if (offset == INVALID_OFFSET) {
        // we use max Okteta::Address to indicate not locked -> error out
        mLogger->error() << "Attempting to lock by default at max Okteta::Address, this is forbidden.";
        return;
    }
    mDefaultLockOffset = offset;
}

void TopLevelDataInformation::newModelActivated(Okteta::AbstractByteArrayModel* model)
{
    if (!model || mLockedPositions.contains(model)) {
        return;
    }

    if (mDefaultLockOffset != INVALID_OFFSET) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
            << "new model activated:" << model << ", locked at 0x" << QString::number(mDefaultLockOffset, 16);
        lockPositionToOffset(mDefaultLockOffset, model);
    } else {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES)
            << "new model activated:" << model << ", not locked.";
    }
}

bool TopLevelDataInformation::isLockedFor(const Okteta::AbstractByteArrayModel* model) const
{
    return mLockedPositions.contains(model);
}

Okteta::Address TopLevelDataInformation::lockPositionFor(const Okteta::AbstractByteArrayModel* model) const
{
    Q_ASSERT(mLockedPositions.contains(model) && mLockedPositions.value(model) != INVALID_OFFSET);
    return mLockedPositions.value(model, INVALID_OFFSET);
}

int TopLevelDataInformation::indexOf(const DataInformation* const data) const
{
    Q_ASSERT(data == mData.get());
    Q_UNUSED(data)
    return mIndex;
}

void TopLevelDataInformation::setActualDataInformation(DataInformation* newData)
{
    Q_ASSERT(newData);
    mData.reset(newData);
}

bool TopLevelDataInformation::isTopLevel() const
{
    return true;
}

#include "moc_topleveldatainformation.cpp"
