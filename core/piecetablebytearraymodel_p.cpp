/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetablebytearraymodel_p.hpp"
#include "piecetablebytearraymodel.hpp"

// lib
#include <addressrangelist.hpp>

namespace Okteta {

static constexpr int InvalidVersionIndex = -1;

PieceTableByteArrayModelPrivate::PieceTableByteArrayModelPrivate(PieceTableByteArrayModel* parent, const QByteArray& data)
    : AbstractByteArrayModelPrivate(parent)
    , mReadOnly(false)
    , mInitialData(data)
{
    mPieceTable.init(data.size());
}

PieceTableByteArrayModelPrivate::PieceTableByteArrayModelPrivate(PieceTableByteArrayModel* parent, int size, Byte fillByte)
    : AbstractByteArrayModelPrivate(parent)
    , mReadOnly(false)
    , mInitialData(size, fillByte)
{
    mPieceTable.init(size);
}

PieceTableByteArrayModelPrivate::~PieceTableByteArrayModelPrivate() = default;

// TODO: getStorageData needs some caching, optimize for successive access
Byte PieceTableByteArrayModelPrivate::byte(Address offset) const
{
    int storageId;
    Address storageOffset;
    mPieceTable.getStorageData(&storageId, &storageOffset, offset);

    const Byte result = (storageId == KPieceTable::Piece::OriginalStorage) ?
                        mInitialData[storageOffset] :
                        mChangesDataStorage[storageOffset];
    return result;
}

void PieceTableByteArrayModelPrivate::setData(const QByteArray& data)
{
    Q_Q(PieceTableByteArrayModel);

    const int oldSize = mPieceTable.size();
    const bool wasModifiedBefore = isModified();
    const QVector<Bookmark> bookmarks = mBookmarks.list();

    mInitialData = data;
    mPieceTable.init(data.size());
    mChangesDataStorage.clear();
    mBookmarks.clear();

    // TODO: how to tell this to the synchronizer?
    emit q->contentsChanged(ArrayChangeMetricsList::oneReplacement(0, oldSize, data.size()));
    if (wasModifiedBefore) {
        emit q->modifiedChanged(false);
    }
    if (!bookmarks.empty()) {
        emit q->bookmarksRemoved(bookmarks);
    }
    emit q->headVersionChanged(0);
    emit q->revertedToVersionIndex(0);
}

void PieceTableByteArrayModelPrivate::setByte(Address offset, Byte byte)
{
    Q_Q(PieceTableByteArrayModel);

    if (mReadOnly) {
        return;
    }

    const bool wasModifiedBefore = isModified();
    const int oldVersionIndex = versionIndex();

    Address storageOffset;
    const bool newChange = mPieceTable.replaceOne(offset, &storageOffset);
    mChangesDataStorage.append(storageOffset, byte);

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement(offset, 1, 1);
    const ByteArrayChange modification(metrics, mChangesDataStorage.data(storageOffset, 1));
    const QVector<Okteta::ByteArrayChange> modificationsList {
        modification
    };

    emit q->contentsChanged(ArrayChangeMetricsList(metrics));
    emit q->changesDone(modificationsList, oldVersionIndex, versionIndex());
    if (!wasModifiedBefore) {
        emit q->modifiedChanged(true);
    }
    if (newChange) {
        emit q->headVersionChanged(mPieceTable.changesCount());
    } else {
        emit q->headVersionDescriptionChanged(mPieceTable.headChangeDescription());
    }
}

Size PieceTableByteArrayModelPrivate::insert(Address offset, const Byte* insertData, int insertLength)
{
    // correct parameters
    const int oldSize = mPieceTable.size();
    if (offset > oldSize) {
        offset = oldSize;
    }
    // check all parameters
    if (mReadOnly || insertLength == 0) {
        return 0;
    }

    beginChanges();

    doInsertChange(offset, insertData, insertLength);

    endChanges();

    return insertLength;
}

// TODO: is anyone interested in the removed data? so we need a signal beforeRemoving(section)?
Size PieceTableByteArrayModelPrivate::remove(const AddressRange& _removeRange)
{
    AddressRange removeRange(_removeRange);
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeRange.restrictEndTo(oldSize - 1);
    // check parameters
    if (removeRange.start() >= oldSize || removeRange.width() == 0) {
        return 0;
    }

    beginChanges();

    doRemoveChange(removeRange);

    endChanges();

    return removeRange.width();
}

Size PieceTableByteArrayModelPrivate::replace(const AddressRange& _removeRange, const Byte* insertData, int insertLength)
{
    AddressRange removeRange(_removeRange);
    // correct parameters
    const int oldSize = mPieceTable.size();
    removeRange.restrictEndTo(oldSize - 1);
    // check parameters
    if ((removeRange.startsBehind(oldSize - 1) && removeRange.width() > 0)
        || (removeRange.width() == 0 && insertLength == 0)) {
        return 0;
    }

    beginChanges();

    doReplaceChange(removeRange, insertData, insertLength);

    endChanges();

    return insertLength;
}

bool PieceTableByteArrayModelPrivate::swap(Address firstStart, const AddressRange& _secondRange)
{
    AddressRange secondRange(_secondRange);
    // correct parameters
    secondRange.restrictEndTo(mPieceTable.size() - 1);
    // check parameters
    if (secondRange.start() >= mPieceTable.size() || secondRange.width() <= 0
        || firstStart > mPieceTable.size() || secondRange.start() == firstStart) {
        return false;
    }

    beginChanges();

    doSwapChange(firstStart, secondRange);

    endChanges();

    return true;
}

Size PieceTableByteArrayModelPrivate::fill(Byte fillByte, Address offset, Size fillLength)
{
    // correct parameters
    const int lengthToEnd = mPieceTable.size() - offset;
    if (fillLength < 0) {
        fillLength = lengthToEnd;
    }
    const int filledLength = (fillLength < lengthToEnd) ? fillLength : lengthToEnd;
    // check parameters
    const bool nothingToFill = (((int)offset >= mPieceTable.size()) || (fillLength == 0));
    if (nothingToFill) {
        return 0;
    }

    beginChanges();

    doFillChange(offset, filledLength, fillByte, fillLength);

    endChanges();

    return fillLength;
}

void PieceTableByteArrayModelPrivate::revertToVersionByIndex(int versionIndex)
{
    Q_Q(PieceTableByteArrayModel);

    ArrayChangeMetricsList changeList;
    AddressRangeList changedRanges;

    const bool oldModified = isModified();

    const bool anyChanges =
        mPieceTable.revertBeforeChange(versionIndex, &changedRanges, &changeList);   // TODO: changedRanges no longer used

    if (!anyChanges) {
        return;
    }

    const bool newModified = isModified();
    const bool isModificationChanged = (oldModified != newModified);

// TODO: what about the bookmarks? They need version support, too.
// Modell of the bookmarks. But shouldn't they be independent?

    emit q->contentsChanged(changeList);
    if (isModificationChanged) {
        emit q->modifiedChanged(newModified);
    }
    emit q->revertedToVersionIndex(versionIndex);
}

void PieceTableByteArrayModelPrivate::openGroupedChange(const QString& description)
{
    Q_Q(PieceTableByteArrayModel);

    const bool isModifiedBefore = isModified();
    mBeforeGroupedChangeVersionIndex = mPieceTable.appliedChangesCount();
    mPieceTable.openGroupedChange(description);

    if (!isModifiedBefore) {
        emit q->modifiedChanged(true);
    }
    emit q->headVersionChanged(mPieceTable.changesCount());
}

void PieceTableByteArrayModelPrivate::cancelGroupedChange()
{
    if (mBeforeGroupedChangeVersionIndex != InvalidVersionIndex) {
        revertToVersionByIndex(mBeforeGroupedChangeVersionIndex);
    }
}

void PieceTableByteArrayModelPrivate::closeGroupedChange(const QString& description)
{
    Q_Q(PieceTableByteArrayModel);

    mPieceTable.closeGroupedChange(description);
    mBeforeGroupedChangeVersionIndex = InvalidVersionIndex;

    emit q->headVersionDescriptionChanged(mPieceTable.headChangeDescription());
}

QVector<ByteArrayChange> PieceTableByteArrayModelPrivate::changes(int firstVersionIndex, int lastVersionIndex) const
{
    QVector<ByteArrayChange> result;

    result.reserve(lastVersionIndex - firstVersionIndex);
    for (int i = firstVersionIndex; i < lastVersionIndex; ++i) {
        ArrayChangeMetrics metrics;
        Address storageOffset;
        mPieceTable.getChangeData(&metrics, &storageOffset, i);

        QByteArray data;
        if (metrics.type() == ArrayChangeMetrics::Replacement) {
            data = mChangesDataStorage.data(storageOffset, metrics.insertLength());
        }
        result.append(ByteArrayChange(metrics, data));
    }

    return result;
}

void PieceTableByteArrayModelPrivate::doChanges(const QVector<ByteArrayChange>& changes,
                                                int oldVersionIndex, int newVersionIndex)
{
// qCDebug(LOG_OKTETA_CORE) << this<<" is at "<<versionIndex()<<", should from "<<oldVersionIndex<<" to "<<newVersionIndex;
    // changes already done? TODO: should this check be here?
    if (newVersionIndex == versionIndex()) {
        return;
    }

    // collision! TODO: what to do?
    if (oldVersionIndex != versionIndex()) {
        return;
    }

    beginChanges();

    for (const ByteArrayChange& change : changes) {
        const ArrayChangeMetrics& metrics = change.metrics();
        switch (metrics.type())
        {
        case ArrayChangeMetrics::Replacement:
        {
            const int oldSize = mPieceTable.size();
            const AddressRange removeRange = AddressRange::fromWidth(metrics.offset(), metrics.removeLength());
            // check parameters
            if (removeRange.startsBehind(oldSize - 1) && (removeRange.width() > 0)) {
                // something is not matching
                ; // TODO: set flag to undo all changes
            }

            const QByteArray& insertData = change.data();
            doReplaceChange(removeRange, reinterpret_cast<const Byte*>(insertData.constData()), insertData.size());
            break;
        }
        case ArrayChangeMetrics::Swapping:
        {
            const AddressRange secondRange(metrics.secondStart(), metrics.secondEnd());
            doSwapChange(metrics.offset(), secondRange);
            break;
        }
        default:
            ;
        }
    }

    // not the same versioning? TODO: where and how to define the version id?
//     if( newVersionIndex != versionIndex() )
//         return;
// qCDebug(LOG_OKTETA_CORE)<<this<<" is now at "<<versionIndex();

    endChanges();
}

void PieceTableByteArrayModelPrivate::beginChanges()
{
    mBeforeChangesVersionIndex = versionIndex();
    mBeforeChangesModified = isModified();
}

void PieceTableByteArrayModelPrivate::endChanges()
{
    Q_Q(PieceTableByteArrayModel);

    const int currentVersionIndex = versionIndex();
    const bool newChange = (mBeforeChangesVersionIndex != currentVersionIndex);
    const bool currentIsModified = isModified();
    const bool modifiedChanged = (mBeforeChangesModified != currentIsModified);

    emit q->contentsChanged(mChangeMetrics);
    emit q->changesDone(mChanges, mBeforeChangesVersionIndex, currentVersionIndex);
    if (mBookmarksModified) {
        emit q->bookmarksModified(true);
    }
    if (modifiedChanged) {
        emit q->modifiedChanged(currentIsModified);
    }
    if (newChange) {
        emit q->headVersionChanged(mPieceTable.changesCount());
    } else {
        emit q->headVersionDescriptionChanged(mPieceTable.headChangeDescription());
    }

    // clean
    mChangeMetrics.clear();
    mChanges.clear();
    mBookmarksModified = false;
}

void PieceTableByteArrayModelPrivate::doInsertChange(Address offset,
                                                     const Byte* insertData, int insertLength)
{
    Address storageOffset;
    mPieceTable.insert(offset, insertLength, &storageOffset);
    mChangesDataStorage.append(storageOffset, reinterpret_cast<const char*>(insertData), insertLength);

    mBookmarksModified |= mBookmarks.adjustToReplaced(offset, 0, insertLength);

    const ArrayChangeMetrics metrics = ArrayChangeMetrics::asReplacement(offset, 0, insertLength);
    const ByteArrayChange change(metrics, mChangesDataStorage.data(storageOffset, insertLength));

    mChangeMetrics.append(metrics);
    mChanges.append(change);
}

void PieceTableByteArrayModelPrivate::doRemoveChange(const AddressRange& removeRange)
{
    mPieceTable.remove(removeRange);

    mBookmarksModified |= mBookmarks.adjustToReplaced(removeRange.start(), removeRange.width(), 0);

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement(removeRange.start(), removeRange.width(), 0);
    const ByteArrayChange change(metrics);

    mChangeMetrics.append(metrics);
    mChanges.append(change);
}

void PieceTableByteArrayModelPrivate::doReplaceChange(const AddressRange& removeRange,
                                                      const Byte* insertData, int insertLength)
{
    Address storageOffset;
    mPieceTable.replace(removeRange, insertLength, &storageOffset);
    mChangesDataStorage.append(storageOffset, reinterpret_cast<const char*>(insertData), insertLength);

    mBookmarksModified |= mBookmarks.adjustToReplaced(removeRange.start(), removeRange.width(), insertLength);

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement(removeRange.start(), removeRange.width(), insertLength);
    const ByteArrayChange change(metrics, mChangesDataStorage.data(storageOffset, insertLength));

    mChangeMetrics.append(metrics);
    mChanges.append(change);
}

void PieceTableByteArrayModelPrivate::doSwapChange(Address firstStart, const AddressRange& secondRange)
{
    mPieceTable.swap(firstStart, secondRange);

    mBookmarksModified |= mBookmarks.adjustToSwapped(firstStart, secondRange.start(), secondRange.width());

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asSwapping(firstStart, secondRange.start(), secondRange.width());
    const ByteArrayChange change(metrics);

    mChangeMetrics.append(metrics);
    mChanges.append(change);
}

void PieceTableByteArrayModelPrivate::doFillChange(Address offset, Size filledLength,
                                                   Byte fillByte, int fillLength)
{
    Address storageOffset;
    mPieceTable.replace(offset, filledLength, fillLength, &storageOffset);

    mChangesDataStorage.appendFill(storageOffset, fillByte, fillLength);

    const ArrayChangeMetrics metrics =
        ArrayChangeMetrics::asReplacement(offset, filledLength, fillLength);
    const ByteArrayChange change(metrics);

    mChangeMetrics.append(metrics);
    mChanges.append(change);
}

}
