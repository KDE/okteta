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
//QtScript
#include <QScriptEngine>

#include <KDebug>

TopLevelDataInformation::TopLevelDataInformation(DataInformation* data, ScriptLogger* logger,
        QScriptEngine* engine, const QFileInfo& structureFile)
        : QObject(0), mData(data), mLogger(logger), mStructureFile(structureFile),
                mIndex(-1), mValid(!data->isDummy()), mChildDataChanged(false)
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
    //first of all check if start offset is locked
    if (mLockedPositions.contains(input))
    {
        //use the saved offset
        address = mLockedPositions.value(input);
        //we read from the locked position, so now check whether it is necessary to update
        bool updateNeccessary = forceRead || isReadingNecessary(changesList, address);

        if (!updateNeccessary)
            return;
    }
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
    // We do this because the pointed data is indipendent from the
    // structure containing the pointer and so we can use fields
    // which come after the pointer itself in the structure
    while (!mDelayedRead.isEmpty())
        mDelayedRead.dequeue()->delayedReadData(input, address);

    if (mChildDataChanged)
    {
        emit dataChanged();
        mChildDataChanged = false;
    }
}

void TopLevelDataInformation::enqueueReadData(PointerDataInformation *toRead)
{
    mDelayedRead.append(toRead);
}

bool TopLevelDataInformation::isReadingNecessary(const Okteta::ArrayChangeMetricsList& changesList,
        Okteta::Address address)
{
    if (changesList.isEmpty())
        return false; //no changes
    const Okteta::Address structureSizeInBytes = (mData->size() / 8) + (mData->size() % 8 == 0 ? 0 : 1);
    const Okteta::Address end = address + structureSizeInBytes;
    for (int i = 0; i < changesList.length(); ++i)
    {
        const Okteta::ArrayChangeMetrics& change = changesList.at(i);
        //this is valid for all types
        if (change.offset() > end)
        {
            //insertion/deletion/swapping is after end of structure, it doesn't interest us
            continue;
        }
        //now handle special cases
        else if (change.type() == Okteta::ArrayChangeMetrics::Replacement)
        {
            //handle it for replacements
            if (change.lengthChange() == 0 && change.offset() + change.removeLength() < address) {
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

void TopLevelDataInformation::lockPositionToOffset(Okteta::Address offset,
        const Okteta::AbstractByteArrayModel* model)
{
    mLockedPositions.insert(model, quint64(offset));
    kDebug()
    << mData->name() << ": Locking start offset in model "
            << model << "to position " << offset;
    //remove when deleted
    connect(model, SIGNAL(destroyed(QObject*)),
            SLOT(removeByteArrayModelFromList(QObject*)));
}

void TopLevelDataInformation::unlockPosition(
        const Okteta::AbstractByteArrayModel* model)
{
    kDebug()
    << "removing lock at position " << mLockedPositions.value(model) << ", model ="
            << (void*) model;
    //just remove from map
    mLockedPositions.remove(model);
}

void TopLevelDataInformation::removeByteArrayModelFromList(QObject* obj)
{
    const Okteta::AbstractByteArrayModel* model =
            static_cast<Okteta::AbstractByteArrayModel*>(obj);
    mLockedPositions.remove(model);
}

bool TopLevelDataInformation::isLockedFor(const Okteta::AbstractByteArrayModel* model) const
{
    return mLockedPositions.contains(model);
}

quint64 TopLevelDataInformation::lockPositionFor(const Okteta::AbstractByteArrayModel* model) const
{
    Q_ASSERT(mLockedPositions.contains(model));
    return mLockedPositions.value(model);
}

int TopLevelDataInformation::indexOf(const DataInformation* const data) const
{
    Q_ASSERT(data == mData.data());
    Q_UNUSED(data)
    return mIndex;
}

void TopLevelDataInformation::setActualDataInformation(DataInformation* newData)
{
    Q_CHECK_PTR(newData);
    Q_ASSERT(!mData->hasBeenUpdated());
    if (mData->hasBeenUpdated())
        mLogger->error() << "Cannot replace top level element since it was already updated!";
    else
        mData.reset(newData);
}
