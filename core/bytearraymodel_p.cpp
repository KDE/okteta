/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraymodel_p.hpp"

// Std
#include <cstring>

static constexpr int minChunkSize = 512;
static constexpr int maxChunkSize = 1024 * 10; // TODO: get max. memory page size

// TODO: think about realloc & Co.

namespace Okteta {

ByteArrayModelPrivate::ByteArrayModelPrivate(ByteArrayModel* parent,
                                             Byte* data, int size, int rawSize, bool keepsMemory)
    : AbstractByteArrayModelPrivate(parent)
    , mData(data)
    , mSize(size)
    , mRawSize(rawSize < size ? size : rawSize)
    , mKeepsMemory(keepsMemory)
    , mAutoDelete(false)
    , mReadOnly(true)
    , mModified(false)
{
}

ByteArrayModelPrivate::ByteArrayModelPrivate(ByteArrayModel* parent,
                                             const Byte* data, int size)
    : AbstractByteArrayModelPrivate(parent)
    , mData(const_cast<Byte*>(data))
    , mSize(size)
    , mRawSize(size)
    , mKeepsMemory(true)
    , mAutoDelete(false)
    , mReadOnly(true)
    , mModified(false)
{
}

ByteArrayModelPrivate::ByteArrayModelPrivate(ByteArrayModel* parent,
                                             int size, int maxSize)
    : AbstractByteArrayModelPrivate(parent)
    , mData((size > 0) ? new Byte[size] : nullptr)
    , mSize(size)
    , mRawSize(size)
    , mMaxSize(maxSize)
    , mKeepsMemory(false)
    , mAutoDelete(true)
    , mReadOnly(false)
    , mModified(false)
{
}

void ByteArrayModelPrivate::setData(Byte* data, int size, int rawSize, bool keepMemory)
{
    Q_Q(ByteArrayModel);

    if (!mAutoDelete) {
        mData.release();
    }
    const int oldSize = mSize;

    mData.reset(data);
    mSize = size;
    mRawSize = (rawSize < size) ? size : rawSize;
    if (mMaxSize != -1 && mMaxSize < size) {
        mMaxSize = size;
    }
    mKeepsMemory = keepMemory;

    mModified = false;
    Q_EMIT q->contentsChanged(ArrayChangeMetricsList::oneReplacement(0, oldSize, size));
    Q_EMIT q->modifiedChanged(false);
}

Size ByteArrayModelPrivate::insertBytes(Address offset, const Byte* insertData, int insertLength)
{
    Q_Q(ByteArrayModel);

    if (mReadOnly) {
        return 0;
    }
    // check all parameters
    if (insertLength == 0) {
        return 0;
    }

    const bool wasModifiedBefore = mModified;

    // correct for appending
    if (offset > mSize) {
        offset = mSize;
    }

    insertLength = addSize(insertLength, offset, true);

    // copy new data to its place
    memcpy(&mData[offset], insertData, insertLength);

    const bool bookmarksModified = m_bookmarks.adjustToReplaced(offset, 0, insertLength);
    mModified = true;

    Q_EMIT q->contentsChanged(ArrayChangeMetricsList::oneReplacement(offset, 0, insertLength));
    if (bookmarksModified) {
        Q_EMIT q->bookmarksModified(true);
    }
    if (!wasModifiedBefore) {
        Q_EMIT q->modifiedChanged(true);
    }

    return insertLength;
}

Size ByteArrayModelPrivate::removeBytes(const AddressRange& _removeRange)
{
    Q_Q(ByteArrayModel);

    if (mReadOnly) {
        return 0;
    }

    AddressRange removeRange(_removeRange);
    if (removeRange.startsBehind(mSize - 1) || removeRange.width() == 0) {
        return 0;
    }

    const bool wasModifiedBefore = mModified;

    removeRange.restrictEndTo(mSize - 1);

    const Address behindRemovePos = removeRange.nextBehindEnd();
    // move right data behind the input range
    memmove(&mData[removeRange.start()], &mData[behindRemovePos], mSize - behindRemovePos);

    // set new values
    mSize -= removeRange.width();

    const bool bookmarksModified = m_bookmarks.adjustToReplaced(removeRange.start(), removeRange.width(), 0);
    mModified = true;

    Q_EMIT q->contentsChanged(ArrayChangeMetricsList::oneReplacement(removeRange.start(), removeRange.width(), 0));
    if (bookmarksModified) {
        Q_EMIT q->bookmarksModified(true);
    }
    if (!wasModifiedBefore) {
        Q_EMIT q->modifiedChanged(true);
    }

    return removeRange.width();
}

Size ByteArrayModelPrivate::replaceBytes(const AddressRange& _removeRange, const Byte* insertData, int insertLength)
{
    Q_Q(ByteArrayModel);

    if (mReadOnly) {
        return 0;
    }

    AddressRange removeRange(_removeRange);
    // check all parameters
    if (removeRange.start() >= mSize || (removeRange.width() == 0 && insertLength == 0)) {
        return 0;
    }

    const bool wasModifiedBefore = mModified;

    removeRange.restrictEndTo(mSize - 1);

    const Size sizeDiff = insertLength - removeRange.width();
    int newSize = mSize + sizeDiff;
    // check if buffer does not get to big TODO: make algo simplier and less if else
    if (mMaxSize != -1 && newSize > mMaxSize) {
        if (mSize == mMaxSize) {
            return 0;
        }
        insertLength -= newSize - mMaxSize;
        newSize = mMaxSize;
    } else if (mKeepsMemory && newSize > mRawSize) {
        if (mSize == mRawSize) {
            return 0;
        }
        insertLength -= newSize - mRawSize;
        newSize = mRawSize;
    }

    const Address behindInsertPos = removeRange.start() + insertLength;
    const Address behindRemovePos = removeRange.nextBehindEnd();

    // raw array not big enough?
    if (mRawSize < newSize) {
        // create new buffer
        auto newData = std::unique_ptr<Byte[]>(new Byte[newSize]); // no make_unique, no need for initialization
        if (!newData) {
            return 0;
        }

        // move old data to its (new) places
        memcpy(newData.get(), mData.get(), removeRange.start());
        memcpy(&newData[behindInsertPos], &mData[behindRemovePos], mSize - behindRemovePos);

        // remove old & set new values
        mData = std::move(newData);
        mRawSize = newSize;
    } else {
        // move old data to its (new) places
        memmove(&mData[behindInsertPos], &mData[behindRemovePos], mSize - behindRemovePos);
    }

    // copy new data to its place
    memcpy(&mData[removeRange.start()], insertData, insertLength);

    // set new values
    mSize = newSize;

    const bool bookmarksModified = m_bookmarks.adjustToReplaced(removeRange.start(), removeRange.width(), insertLength);
    mModified = true;

    Q_EMIT q->contentsChanged(
        ArrayChangeMetricsList::oneReplacement(removeRange.start(), removeRange.width(), insertLength));
    if (bookmarksModified) {
        Q_EMIT q->bookmarksModified(true);
    }
    if (!wasModifiedBefore) {
        Q_EMIT q->modifiedChanged(true);
    }
    return insertLength;
}

bool ByteArrayModelPrivate::swapBytes(Address firstStart, const AddressRange& _secondRange)
{
    Q_Q(ByteArrayModel);

    if (mReadOnly) {
        return false;
    }

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
    auto temp = std::make_unique<Byte[]>(smallPartLength);
    memcpy(temp.get(), &mData[smallPartStart], smallPartLength);

    // move the larger part
    memmove(&mData[largePartDest], &mData[largePartStart], largePartLength);

    // copy smaller part to its new dest
    memcpy(&mData[smallPartDest], temp.get(), smallPartLength);
    temp.reset();

    const bool bookmarksModified = toRight ?
                                   m_bookmarks.adjustToSwapped(secondRange.start(), secondRange.nextBehindEnd(), firstStart - secondRange.end() - 1) :
                                   m_bookmarks.adjustToSwapped(firstStart, secondRange.start(), secondRange.width());
    mModified = true;

    Q_EMIT q->contentsChanged(
        ArrayChangeMetricsList::oneSwapping(firstStart, secondRange.start(), secondRange.width()));
    if (bookmarksModified) {
        Q_EMIT q->bookmarksModified(true);
    }
    if (!wasModifiedBefore) {
        Q_EMIT q->modifiedChanged(true);
    }
    return true;
}

Size ByteArrayModelPrivate::fill(Byte fillByte, Address offset, Size fillLength)
{
    Q_Q(ByteArrayModel);

    if (mReadOnly) {
        return 0;
    }

    // nothing to fill
    if ((offset >= mSize) || (fillLength == 0)) {
        return 0;
    }

    const bool wasModifiedBefore = mModified;

    const Size lengthToEnd = mSize - offset;

    if (fillLength < 0) {
        fillLength = lengthToEnd;
    }

    const bool isToFillBehindEnd = (fillLength > lengthToEnd);
    const Size replacedLength = (isToFillBehindEnd ? lengthToEnd : fillLength);

    if (isToFillBehindEnd) {
        Size sizeToAdd = fillLength - lengthToEnd;
        sizeToAdd = addSize(sizeToAdd, offset, false);
        fillLength = lengthToEnd + sizeToAdd;
    }
    // nothing to fill
    if (fillLength == 0) {
        return 0;
    }

    memset(&mData[offset], fillByte, fillLength);
    mModified = true;

    Q_EMIT q->contentsChanged(ArrayChangeMetricsList::oneReplacement(offset, replacedLength, fillLength));
    if (!wasModifiedBefore) {
        Q_EMIT q->modifiedChanged(true);
    }
    return fillLength;
}

int ByteArrayModelPrivate::addSize(int addSize, int splitPosition, bool saveUpperPart)
{
    if (mReadOnly) {
        return 0;
    }

    int newSize = mSize + addSize;
    // check if buffer does not get too big
    if (mMaxSize != -1 && newSize > mMaxSize) {
        if (mSize == mMaxSize) {
            return 0;
        }
        newSize = mMaxSize;
        addSize = newSize - mSize;
    } else if (mKeepsMemory && newSize > mRawSize) {
        if (mSize == mRawSize) {
            return 0;
        }
        newSize = mRawSize;
        addSize = newSize - mSize;
    }

    const int BehindSplitPos = splitPosition + addSize;
    // raw array not big enough?
    if (mRawSize < newSize) {
        // get new raw size
        int chunkSize = minChunkSize;
        // find chunk size where newsize fits into
        while (chunkSize < newSize) {
            chunkSize <<= 1;
        }
        // limit to max size
        if (chunkSize > maxChunkSize) {
            chunkSize = maxChunkSize;
        }
        // find add size
        int NewRawSize = chunkSize;
        while (NewRawSize < newSize) {
            NewRawSize += chunkSize;
        }
        // create new buffer
        auto newData = std::unique_ptr<Byte[]>(new Byte[NewRawSize]); // no make_unique, no need for initialization

        // move old data to its (new) places
        memcpy(newData.get(), mData.get(), splitPosition);
        if (saveUpperPart) {
            memcpy(&newData[BehindSplitPos], &mData[splitPosition], mSize - splitPosition);
        }

        // remove old & set new values
        mData = std::move(newData);
        mRawSize = NewRawSize;
    }
    // old buffer kept
    else {
        if (saveUpperPart) {
            // move old data to its (new) places
            memmove(&mData[BehindSplitPos], &mData[splitPosition], mSize - splitPosition);
        }
    }

    // set new values
    mSize = newSize;

    return addSize;
}

}
