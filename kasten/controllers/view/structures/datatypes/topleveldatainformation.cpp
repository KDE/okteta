/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

#include "topleveldatainformation.h"
#include "datainformation.h"
#include "primitive/pointerdatainformation.h"
#include "datainformationwithchildren.h"

#include "../script/scripthandler.h"
#include "../script/scriptlogger.h"
#include "../script/scriptengineinitializer.h"
#include "primitivefactory.h"

#include <abstractbytearraymodel.h>

#include <QScriptEngine>

#include <KDebug>

#include <limits>

const quint64 TopLevelDataInformation::INVALID_OFFSET = std::numeric_limits<quint64>::max();

TopLevelDataInformation::TopLevelDataInformation(DataInformation* data, ScriptLogger* logger,
        QScriptEngine* engine, const QFileInfo& structureFile)
        : QObject(0), mData(data), mLogger(logger), mStructureFile(structureFile),
          mIndex(-1), mValid(!data->isDummy()), mChildDataChanged(false),
          mDefaultLockOffset(INVALID_OFFSET), mLastReadOffset(INVALID_OFFSET), mLastModel(0)
{
    Q_CHECK_PTR(mData);
    mData->setParent(this);
    setObjectName(mData->name());

    if (!mLogger)
        mLogger.reset(new ScriptLogger());

    if (!engine)
        engine = ScriptEngineInitializer::newEngine();
    mScriptHandler.reset(new ScriptHandler(engine, this));
}

TopLevelDataInformation::~TopLevelDataInformation()
{
}

void TopLevelDataInformation::validate()
{
    logger()->info(mData.data()) << "Validation requested.";
    mScriptHandler->validateData(mData.data());
}

QScriptEngine* TopLevelDataInformation::scriptEngine() const
{
    return mScriptHandler->engine();
}

void TopLevelDataInformation::resetValidationState()
{
    mData->resetValidationState();
}

void TopLevelDataInformation::read(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        const Okteta::ArrayChangeMetricsList& changesList, bool forceRead)
{
    mChildDataChanged = false;
    const bool updateNeccessary = forceRead || isReadingNecessary(input, address, changesList);
    if (!updateNeccessary)
        return;

    quint64 remainingBits = (input->size() - address) * 8;
    quint8 bitOffset = 0;
    mData->beginRead(); //before reading set wasAbleToRead to false
    mData->resetValidationState(); //reading new data -> validation state is old
    const DataInformation* oldData = mData.data();
    mScriptHandler->updateDataInformation(mData.data()); //unlikely that this is useful, but maybe someone needs it
    if (mData.data() != oldData)
        mLogger->info() << "Main element was replaced!";
    mData->readData(input, address, remainingBits, &bitOffset);

    // Read all the delayed PointerDataInformation
    // We do this because the pointed data is independant from the
    // structure containing the pointer and so we can use fields
    // which come after the pointer itself in the structure
    while (!mDelayedRead.isEmpty())
        mDelayedRead.dequeue()->delayedReadData(input, address);

    if (mChildDataChanged)
    {
        emit dataChanged();
        mChildDataChanged = false;
    }
    mLastModel = input;
    mLastReadOffset = address;
}

void TopLevelDataInformation::enqueueReadData(PointerDataInformation* toRead)
{
    mDelayedRead.append(toRead);
}

bool TopLevelDataInformation::isReadingNecessary(Okteta::AbstractByteArrayModel* model,
        Okteta::Address address, const Okteta::ArrayChangeMetricsList& changesList)
{
    if (model != mLastModel)
        return true; //whenever we have a new model we have to read

    if (isLockedFor(model))
        address = lockPositionFor(model);

    if (address != mLastReadOffset)
        return true; //address as changed, we have to read again

    //address has not changed, check whether the changes affect us

    //TODO always return true if structure contains pointers
    if (changesList.isEmpty())
        return false; //no changes
    const Okteta::Address structureSizeInBytes = (mData->size() / 8) + (mData->size() % 8 == 0 ? 0 : 1);
    const Okteta::Address end = address + structureSizeInBytes;
    for (int i = 0; i < changesList.length(); ++i)
    {
        const Okteta::ArrayChangeMetrics& change = changesList.at(i);
        //this is valid for all types
        if (change.offset() >= end)
        {
            //insertion/deletion/swapping is after end of structure, it doesn't interest us
            continue;
        }
        //now handle special cases
        else if (change.type() == Okteta::ArrayChangeMetrics::Replacement)
        {
            //handle it for replacements
            if (change.lengthChange() == 0 && change.offset() + change.removeLength() <= address) {
                //no length change and it doesn't affect structure since it is before
                //could use insertLength() instead of removeLength() since they are the same
                continue;
            }
            else {
                //something was removed/inserted before start of structure
                //which means all bytes have moved forwards/backwards: reread all
                return true;
            }
        }
        else if (change.type() == Okteta::ArrayChangeMetrics::Swapping)
        {
            if (change.secondEnd() < address) {
                //swapped ranges end before start, i.e. the range of interest does not change
                continue;
            }
            else {
                //Not sure what other possibilities there are, but rather waste CPU or I/O rereading
                //than showing outdated values
                return true;
            }
        }
        else
        {
            kDebug() << "Invalid change";
            continue;
        }
    }
    return false; //nothing affected us
}

void TopLevelDataInformation::lockPositionToOffset(Okteta::Address offset, const Okteta::AbstractByteArrayModel* model)
{
    if (offset == INVALID_OFFSET)
    {
        //we use quint64 max to indicate not locked -> error out
        mLogger->error() << "Attempting to lock at uint64_max, this is forbidden.";
        return;
    }
    mLockedPositions.insert(model, quint64(offset));
    kDebug() << mData->name() << ": Locking start offset in model" << model << "to position" << hex << offset;
    //remove when deleted
    connect(model, SIGNAL(destroyed(QObject*)), this, SLOT(removeByteArrayModelFromList(QObject*)));
}

void TopLevelDataInformation::unlockPosition(const Okteta::AbstractByteArrayModel* model)
{
    Q_ASSERT(mLockedPositions.contains(model) && mLockedPositions.value(model) != INVALID_OFFSET);
    kDebug() << "removing lock at position" << mLockedPositions.value(model) << ", model=" << model;
    mLockedPositions.insert(model, INVALID_OFFSET);
}

void TopLevelDataInformation::removeByteArrayModelFromList(QObject* obj)
{
    Q_ASSERT(qobject_cast<Okteta::AbstractByteArrayModel*>(obj));
    const Okteta::AbstractByteArrayModel* model = static_cast<Okteta::AbstractByteArrayModel*>(obj);
    mLockedPositions.remove(model);
}

bool TopLevelDataInformation::isLockedByDefault() const
{
    return mDefaultLockOffset != INVALID_OFFSET;
}

void TopLevelDataInformation::setDefaultLockOffset(Okteta::Address offset)
{
    if (offset == INVALID_OFFSET)
    {
        //we use quint64 max to indicate not locked -> error out
        mLogger->error() << "Attempting to lock by default at uint64_max, this is forbidden.";
        return;
    }
    mDefaultLockOffset = offset;
}

void TopLevelDataInformation::newModelActivated(Okteta::AbstractByteArrayModel* model)
{
    //don't add null pointers to map
    if (model && !mLockedPositions.contains(model))
    {
        //if this structure has no default lock offset, mDefaultLockOfsset will contain NOT_LOCKED
        mLockedPositions.insert(model, mDefaultLockOffset);
        if (mDefaultLockOffset == INVALID_OFFSET)
            kDebug() << "new model activated:" << model << ", not locked.";
        else
            kDebug() << "new model activated:" << model << ", locked at 0x" << QString::number(mDefaultLockOffset, 16);
    }
}

bool TopLevelDataInformation::isLockedFor(const Okteta::AbstractByteArrayModel* model) const
{
    Q_ASSERT(mLockedPositions.contains(model));
    return mLockedPositions.value(model, 0) != INVALID_OFFSET;
}

quint64 TopLevelDataInformation::lockPositionFor(const Okteta::AbstractByteArrayModel* model) const
{
    Q_ASSERT(mLockedPositions.contains(model) && mLockedPositions.value(model) != INVALID_OFFSET);
    return mLockedPositions.value(model);
}

int TopLevelDataInformation::indexOf(const DataInformation* const data) const
{
    Q_ASSERT(data == mData.data());
    return mIndex;
}

void TopLevelDataInformation::setActualDataInformation(DataInformation* newData)
{
    Q_ASSERT(newData);
    mData.reset(newData);
}
