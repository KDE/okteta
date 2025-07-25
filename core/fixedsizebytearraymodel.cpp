/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fixedsizebytearraymodel.hpp"

// lib
#include "arraychangemetricslist.hpp"
#include <logging.hpp>
// Std
#include <cstring>

namespace Okteta {

FixedSizeByteArrayModel::FixedSizeByteArrayModel(Byte* data, int size, Byte fillUpByte, QObject* parent)
    : AbstractByteArrayModel(parent)
    , mData(data)
    , mSize(size)
    , mFillUpByte(fillUpByte)
    , mReadOnly(true)
    , mModified(false)
    , mAutoDelete(false)
{
}

FixedSizeByteArrayModel::FixedSizeByteArrayModel(int size, Byte fillUpByte, QObject* parent)
    : AbstractByteArrayModel(parent)
    , mData(std::unique_ptr<Byte[]>(new Byte[size])) // no make_unique, no need for initialization
    , mSize(size)
    , mFillUpByte(fillUpByte)
    , mReadOnly(false)
    , mModified(false)
    , mAutoDelete(true)
{
    reset(0, size);
}

FixedSizeByteArrayModel::~FixedSizeByteArrayModel()
{
    if (!mAutoDelete) {
        mData.release();
    }
}

void FixedSizeByteArrayModel::setByte(Address offset, Byte byte)
{
    const bool wasModifiedBefore = mModified;

    mData[offset] = byte;
    mModified = true;

    Q_EMIT contentsChanged(ArrayChangeMetricsList::oneReplacement(offset, 1, 1));
    if (!wasModifiedBefore) {
        Q_EMIT modifiedChanged(true);
    }
}

Size FixedSizeByteArrayModel::insertBytes(Address offset, const Byte* insertData, int insertLength)
{
    // check all parameters
    if (offset >= mSize || insertLength == 0) {
        return 0;
    }

    const bool wasModifiedBefore = mModified;

    if (offset + insertLength > mSize) {
        insertLength = mSize - offset;
    }

    const Address behindInsertOffset = offset + insertLength;
    // fmove right data behind the input range
    memmove(&mData[behindInsertOffset], &mData[offset], mSize - behindInsertOffset);
    // insert input
    memcpy(&mData[offset], insertData, insertLength);

    mModified = true;

    Q_EMIT contentsChanged(ArrayChangeMetricsList::oneReplacement(offset, 0, insertLength));
    // Q_EMIT contentsReplaced( offset, , 0 ); TODO: how to signal the removed data?
    if (!wasModifiedBefore) {
        Q_EMIT modifiedChanged(true);
    }

    return insertLength;
}

Size FixedSizeByteArrayModel::removeBytes(const AddressRange& _removeRange)
{
    AddressRange removeRange(_removeRange);
    if (removeRange.start() >= mSize || removeRange.width() == 0) {
        return 0;
    }

    const bool wasModifiedBefore = mModified;

    removeRange.restrictEndTo(mSize - 1);

    const Size removeLength = removeRange.width();
    const Address behindRemoveOffset = removeRange.nextBehindEnd();
    // fmove right data behind the input range
    memmove(&mData[removeRange.start()], &mData[behindRemoveOffset], mSize - behindRemoveOffset);
    // clear freed space
    reset(mSize - removeLength, removeLength);

    mModified = true;

    Q_EMIT contentsChanged(ArrayChangeMetricsList::oneReplacement(removeRange.start(), removeRange.width(), 0));
    // Q_EMIT contentsReplaced( offset, 0,  ); TODO: how to signal the inserted data?
    if (!wasModifiedBefore) {
        Q_EMIT modifiedChanged(true);
    }

    return removeLength;
}

Size FixedSizeByteArrayModel::replaceBytes(const AddressRange& _removeRange, const Byte* insertData, int insertLength)
{
    AddressRange removeRange(_removeRange);
    // check all parameters
    if (removeRange.startsBehind(mSize - 1) || (removeRange.width() == 0 && insertLength == 0)) {
        return 0;
    }

    const bool wasModifiedBefore = mModified;

    removeRange.restrictEndTo(mSize - 1);
    if (removeRange.start() + insertLength > mSize) {
        insertLength = mSize - removeRange.start();
    }

    const Size sizeDiff = insertLength - removeRange.width();

    // is input longer than removed?
    if (sizeDiff > 0) {
        const Address behindInsertOffset = removeRange.start() + insertLength;
        // fmove right data behind the input range
        memmove(&mData[behindInsertOffset], &mData[removeRange.nextBehindEnd()], mSize - behindInsertOffset);
    }
    // is input smaller than removed?
    else if (sizeDiff < 0) {
        const Address behindRemoveOffset = removeRange.nextBehindEnd();
        // fmove right data behind the input range
        memmove(&mData[removeRange.start() + insertLength], &mData[behindRemoveOffset], mSize - behindRemoveOffset);
        // clear freed space
        reset(mSize + sizeDiff, -sizeDiff);
    }
    // insert input
    memcpy(&mData[removeRange.start()], insertData, insertLength);

    mModified = true;

    Q_EMIT contentsChanged(ArrayChangeMetricsList::oneReplacement(removeRange.start(), removeRange.width(), insertLength));
    // Q_EMIT contentsReplaced( offset, 0,  ); TODO: how to signal the changed data at the end?
    if (!wasModifiedBefore) {
        Q_EMIT modifiedChanged(true);
    }

    return insertLength;
}

bool FixedSizeByteArrayModel::swapBytes(Address firstStart, const AddressRange& _secondRange)
{
    AddressRange secondRange(_secondRange);
    // check all parameters
    if (secondRange.start() >= mSize || secondRange.width() == 0
        || firstStart > mSize || secondRange.start() == firstStart) {
        return false;
    }

    const bool wasModifiedBefore = mModified;

    secondRange.restrictEndTo(mSize - 1);
    const bool toRight = firstStart > secondRange.start();
    const Size movedLength = secondRange.width();
    const Size displacedLength = toRight ?  firstStart - secondRange.end() - 1 : secondRange.start() - firstStart;

    // find out section that is smaller
    Size smallPartLength, largePartLength, smallPartStart, largePartStart, smallPartDest, largePartDest;
    // moving part is smaller?
    if (movedLength < displacedLength) {
        smallPartStart = secondRange.start();
        smallPartLength = movedLength;
        largePartLength = displacedLength;
        // moving part moves right?
        if (toRight) {
            smallPartDest = firstStart - movedLength;
            largePartStart = secondRange.nextBehindEnd();
            largePartDest = secondRange.start();
        } else {
            smallPartDest = firstStart;
            largePartStart = firstStart;
            largePartDest = firstStart + movedLength;
        }
    } else {
        largePartStart = secondRange.start();
        largePartLength = movedLength;
        smallPartLength = displacedLength;
        // moving part moves right?
        if (toRight) {
            largePartDest = firstStart - movedLength;
            smallPartStart = secondRange.nextBehindEnd();
            smallPartDest = secondRange.start();
        } else {
            largePartDest = firstStart;
            smallPartStart = firstStart;
            smallPartDest = firstStart + movedLength;
        }
    }

    // copy smaller part to tempbuffer
    auto tempBuffer = std::make_unique<Byte[]>(smallPartLength);
    memcpy(tempBuffer.get(), &mData[smallPartStart], smallPartLength);

    // move the larger part
    memmove(&mData[largePartDest], &mData[largePartStart], largePartLength);

    // copy smaller part to its new dest
    memcpy(&mData[smallPartDest], tempBuffer.get(), smallPartLength);
    tempBuffer.reset();

    mModified = true;

    Q_EMIT contentsChanged(ArrayChangeMetricsList::oneSwapping(firstStart, secondRange.start(), secondRange.width()));
    if (!wasModifiedBefore) {
        Q_EMIT modifiedChanged(true);
    }

    return true;
}

Size FixedSizeByteArrayModel::fill(Byte fillByte, Address offset, Size fillLength)
{
    // nothing to fill
    if (offset >= mSize) {
        return 0;
    }

    const bool wasModifiedBefore = mModified;

    const Size lengthToEnd = mSize - offset;

    if (fillLength < 0 || fillLength > lengthToEnd) {
        fillLength = lengthToEnd;
    }

    if (fillLength == 0) {
        return 0;
    }

    memset(&mData[offset], fillByte, fillLength);
    mModified = true;

    Q_EMIT contentsChanged(ArrayChangeMetricsList::oneReplacement(offset, fillLength, fillLength));
    if (!wasModifiedBefore) {
        Q_EMIT modifiedChanged(true);
    }

    return fillLength;
}

int FixedSizeByteArrayModel::compare(const AbstractByteArrayModel& other, const AddressRange& _otherRange, Address offset) const
{
    AddressRange otherRange(_otherRange);
    // qCDebug(LOG_OKTETA_CORE) << QString("offset: %1, otherRange: (%3/%4)" ).arg(offset).arg(otherRange.start()).arg(otherRange.end())
    //    << endl;
    // test other values
    if (otherRange.startsBehind(other.size() - 1)) {
        return 1;
    }

    // check own values
    if (offset >= mSize) {
        return -1;
    }

    int valueByLength = 0; // default: equal

    AddressRange range = AddressRange::fromWidth(offset, otherRange.width());
    Address lastOffset = other.size() - 1;
    //
    if (otherRange.endsBehind(lastOffset)) {
        // make shorter
        otherRange.setEnd(lastOffset);
        if (otherRange.width() < range.width()) {
            valueByLength = 1;
        }
    }
    lastOffset = mSize - 1;
    if (range.endsBehind(lastOffset)) {
        // make shorter
        range.setEnd(lastOffset);
        if (otherRange.width() > range.width()) {
            valueByLength = -1;
        }
    }
    // qCDebug(LOG_OKTETA_CORE)
    //    << QString( "range: (%1/%2), otherRange: (%3/%4)" ).arg(range.start()).arg(range.end()).arg(otherRange.start()).arg(otherRange.end())
    //    << endl;
    const Address rangeEnd = range.end();
    Address oi = otherRange.start();
    for (Address i = range.start(); i <= rangeEnd; ++i, ++oi) {
        Byte OD = other.byte(oi);
        Byte data = mData[i];
        // qCDebug(LOG_OKTETA_CORE) << QString("%1==%2").arg((int)data).arg((int)OD) ;
        if (OD == data) {
            continue;
        }
        return (OD < data) ? 1 : -1;
    }

    return valueByLength;
}

void FixedSizeByteArrayModel::reset(unsigned int offset, unsigned int length)
{
    memset(&mData[offset], mFillUpByte, length);
}

}

#include "moc_fixedsizebytearraymodel.cpp"
