/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetable.hpp"

// Std
#include <algorithm>

namespace KPieceTable {

PieceTable::PieceTable(Size size)
{
    init(size);
}

void PieceTable::init(Size size)
{
    mList.clear();
    if (size > 0) {
        mList.append(Piece(0, size, Piece::OriginalStorage));
    }

    mSize = size;
}

bool PieceTable::getStorageData(Piece::StorageType* storageId, Address* storageOffset, Address dataOffset) const
{
    bool result = false;

    // TODO: use width or offset from current and next?
    AddressRange dataRange(0, -1);
    for (const Piece& piece : mList) {
        dataRange.setEndByWidth(piece.width());

        if (dataRange.includes(dataOffset)) {
            *storageId = piece.storageId();
// qCDebug(LOG_OKTETA_CORE) <<piece.start()<<"+"<<dataRange.localIndex( dataOffset );
            *storageOffset = piece.start() + dataRange.localIndex(dataOffset);
            result = true;
            break;
        }
        dataRange.setStart(dataRange.nextBehindEnd());
    }

    return result;
}

// TODO: optimize search from behind if dataOffset is large (perhaps by counting total size
// TODO: combine sucsequenting inserts, also on other changes if possible link neighbors
void PieceTable::insert(Address insertDataOffset, Size insertLength, Address storageOffset)
{
    // shortcut empty size
    if (insertLength < 1) {
        return;
    }

    const Piece insertPiece(storageOffset, insertLength, Piece::ChangeStorage);

    bool isInserted = false;
    auto it = mList.begin();

    // TODO: use width or offset from current and next?
    AddressRange dataRange(0, -1);
    while (it != mList.end()) {
        Piece& piece = *it;
        dataRange.setEndByWidth(piece.width());

        // current piece starts at insert offset?
        if (dataRange.start() == insertDataOffset) {
            // merge inserted piece with previous piece if possible
            if (it != mList.begin()) {
                Piece& previousPiece = *(it - 1);
                if (previousPiece.append(insertPiece)) {
                    // merge altered previous and current piece if possible
                    if (previousPiece.append(piece)) {
                        mList.erase(it);
                    }
                    // done
                    isInserted = true;
                    break;
                }
            }

            // merge to-insert piece with current piece if possible, otherwise insert
            if (!piece.prepend(insertPiece)) {
                mList.insert(it, insertPiece);
            }
            // done
            isInserted = true;
            break;
        }
        // current piece covers insert offset
        ++it;
        if (dataRange.includes(insertDataOffset)) {
            // split current piece at insertion offset, will be two valid pieces
            const Piece secondPiece = piece.splitAtLocal(dataRange.localIndex(insertDataOffset));
            // insert to-insert piece
            it = mList.insert(it, insertPiece);
            ++it;
            // at last insert split-off half piece
            mList.insert(it, secondPiece);
            // done
            isInserted = true;
            break;
        }

        // update loop state
        dataRange.setStart(dataRange.nextBehindEnd());
    }
    // end of pieces reached
    if (!isInserted && (dataRange.start() == insertDataOffset)) {
        // merge to-insert piece with last piece if possible
        if (it != mList.begin()) {
            Piece& previousPiece = *(it - 1);
            isInserted = previousPiece.append(insertPiece);
        }
        if (!isInserted) {
            // insert to-insert piece at end
            mList.insert(it, insertPiece);
        }
    }

    mSize += insertLength;
}

void PieceTable::insert(Address insertDataOffset, const PieceList& insertPieceList)
{
    // shortcut empty list
    if (insertPieceList.isEmpty()) {
        return;
    }

    bool isInserted = false;
    auto it = mList.begin();

    // TODO: use width or offset from current and next?
    AddressRange dataRange(0, -1);
    while (it != mList.end()) {
        Piece& piece = *it;
        dataRange.setEndByWidth(piece.width());

        // current piece starts at insert offset?
        if (dataRange.start() == insertDataOffset) {
            auto firstInsertIt = insertPieceList.begin();
            // merge first inserted piece with previous piece if possible
            if (it != mList.begin()) {
                Piece& previousPiece = *(it - 1);
                if (previousPiece.append(*firstInsertIt)) {
                    ++firstInsertIt;
                    // shortcut if no more pieces left to add
                    if (firstInsertIt == insertPieceList.end()) {
                        // merge altered previous and current piece if possible
                        if (previousPiece.append(piece)) {
                            mList.erase(it);
                        }
                        // done
                        isInserted = true;
                        break;
                    }
                }
            }

            // insert all-but-last to-insert pieces
            const auto lastInsertIt = insertPieceList.end() - 1;
            // there is at least 1 piece to handle, also given the shortcut above,
            // so firstInsertIt <= lastInsertIt can be relied on
            std::for_each(firstInsertIt, lastInsertIt, [this, &it](const Piece& piece) mutable {
                it = mList.insert(it, piece);
                ++it;
            });

            // merge last to-insert piece with current piece if possible, otherwise insert
            const Piece& lastInsertPiece = *lastInsertIt;
            if (!piece.prepend(lastInsertPiece)) {
                mList.insert(it, lastInsertPiece);
            }
            // done
            isInserted = true;
            break;
        }
        // current piece covers insert offset
        ++it;
        if (dataRange.includes(insertDataOffset)) {
            // split current piece at insertion offset, will be two valid pieces
            const Piece secondPiece = piece.splitAtLocal(dataRange.localIndex(insertDataOffset));
            // insert all to-insert pieces
            for (const auto& insertPiece : insertPieceList) {
                it = mList.insert(it, insertPiece);
                ++it;
            }
            // at last insert split-off half piece
            mList.insert(it, secondPiece);
            // done
            isInserted = true;
            break;
        }

        // update loop state
        dataRange.setStart(dataRange.nextBehindEnd());
    }
    // end of pieces reached
    if (!isInserted && (dataRange.start() == insertDataOffset)) {
        auto firstInsertIt = insertPieceList.begin();
        // merge first inserted piece with last piece if possible
        if (it != mList.begin()) {
            Piece& previousPiece = *(it - 1);
            if (previousPiece.append(*firstInsertIt)) {
                ++firstInsertIt;
            }
        }
        // insert all to-insert pieces at end
        std::for_each(firstInsertIt, insertPieceList.end(), [this, &it](const Piece& piece) mutable {
            it = mList.insert(it, piece);
            ++it;
        });
    }

    mSize += insertPieceList.totalLength();
}

// TODO: make algorithm simpler
PieceList PieceTable::remove(const AddressRange& removeRange)
{
    // shortcut empty range
    // TODO: empty range should be acceptable, warnn about anything?
    if (!removeRange.isValid()) {
        return {};
    }

    PieceList removedPieceList;

    AddressRange dataRange(0, -1);

    QLinkedList<Piece>::Iterator it = mList.begin();
    while (it != mList.end()) {
        Piece* piece = &*it;
        dataRange.setEndByWidth(piece->width());

        if (dataRange.includes(removeRange.start())) {
            QLinkedList<Piece>::Iterator firstRemoved = it;
            const Address firstDataRangeStart = dataRange.start();
// int sections = 1;

            if (dataRange.includesInside(removeRange)) {
                const AddressRange localRange = dataRange.localRange(removeRange);
                const Piece removedPiece = piece->subPiece(localRange);
                const Piece secondPiece = piece->removeLocal(localRange);

                mList.insert(++it, secondPiece);
                removedPieceList.append(removedPiece);
// qCDebug(LOG_OKTETA_CORE) << "removed intern";
                break;
            }
            do {
                if (dataRange.includes(removeRange.end())) {
                    QLinkedList<Piece>::Iterator lastRemoved = it;
// qCDebug(LOG_OKTETA_CORE) << removeRange.start() << removeRange.end() << firstDataRangeStart << dataRange.end();
                    // cut from first section if not all
                    bool onlyCompletePiecesRemoved = true;
                    if (firstDataRangeStart < removeRange.start()) {
                        const Address newLocalEnd = removeRange.start() - firstDataRangeStart - 1;
                        const Piece removedPiece = (*firstRemoved).removeEndBehindLocal(newLocalEnd);
                        removedPieceList.append(removedPiece);

                        ++firstRemoved;
                        onlyCompletePiecesRemoved = false;
// qCDebug(LOG_OKTETA_CORE) << "end of first removed"<<piece->start()<<piece->end()<<"->"<<removedPiece.start()<<removedPiece.end();
// --sections;
                    }

                    Piece removedPartialPieceFromLast;
                    // cut from last section if not all
                    if (removeRange.end() < dataRange.end()) {
                        const Address newLocalStart =  dataRange.localIndex(removeRange.end()) + 1;
                        removedPartialPieceFromLast = piece->removeStartBeforeLocal(newLocalStart);

                        onlyCompletePiecesRemoved = false;
// qCDebug(LOG_OKTETA_CORE) << "start of last removed"<<piece->start()<<piece->end()<<"->"<<removedPartialPieceFromLast.start()<<removedPartialPieceFromLast.end();
// --sections;
                    } else {
                        ++lastRemoved;
                    }

                    std::for_each(firstRemoved, lastRemoved, [&removedPieceList](const Piece& piece) mutable {
                        removedPieceList.append(piece);
                    });

                    if (removedPartialPieceFromLast.isValid()) {
                        removedPieceList.append(removedPartialPieceFromLast);
                    }

                    if (onlyCompletePiecesRemoved) {
                        if (firstRemoved != mList.begin() && lastRemoved != mList.end()) {
                            QLinkedList<Piece>::Iterator beforeFirstRemoved = firstRemoved - 1;
                            if ((*beforeFirstRemoved).append(*lastRemoved)) {
                                ++lastRemoved;
                            }
                        }
                    }

                    mList.erase(firstRemoved, lastRemoved);
// qCDebug(LOG_OKTETA_CORE) << "removed "<<sections;
                    break;
                }
                dataRange.setStart(dataRange.nextBehindEnd());
                ++it;
// ++sections;
                // removeRange is longer than content TODO: just quit or at least remove till the end?
                if (it == mList.end()) {
                    break;
                }
                piece = &*it;
                dataRange.setEndByWidth(piece->width());
            } while (it != mList.end());
            break;
        }

        dataRange.setStart(dataRange.nextBehindEnd());
        ++it;
    }

    mSize -= removeRange.width();

// qCDebug(LOG_OKTETA_CORE)<<"end:"<<asStringList(mList);
    return removedPieceList;
}

PieceList PieceTable::replace(const AddressRange& removeRange, Size insertLength, Address storageOffset)
{
    PieceList removedPieceList = remove(removeRange);
    insert(removeRange.start(), insertLength, storageOffset);
    return removedPieceList;
}
void PieceTable::replace(const AddressRange& removeRange, const PieceList& insertPieceList)
{
    remove(removeRange);
    insert(removeRange.start(), insertPieceList);
}

void PieceTable::swap(Address firstStart, const AddressRange& secondRange)
{
    // shortcut empty range
    // TODO: empty range should be acceptable, warnn about anything?
    if (!secondRange.isValid()) {
        return;
    }

    AddressRange dataRange(0, -1);

    QLinkedList<Piece>::Iterator it = mList.begin();
    while (it != mList.end()) {
        Piece* piece = &*it;
        dataRange.setEndByWidth(piece->width());

        if (dataRange.includes(firstStart)) {
            // piece does not start at offset?
            if (dataRange.start() < firstStart) {
                // well, split and insert second piece, even if we move it later, just make it work for now
                const Piece secondPiece = piece->splitAtLocal(dataRange.localIndex(firstStart));
                it = mList.insert(++it, secondPiece);
                piece = &*it;
                dataRange.setStart(firstStart);
            }
            QLinkedList<Piece>::Iterator firstIt = it;

            do {
                if (dataRange.includes(secondRange.start())) {
                    // piece does not start at source?
                    if (dataRange.start() < secondRange.start()) {
                        // well, split and insert second piece, even if we move it later, just make it work for now
                        const Piece secondPiece = piece->splitAtLocal(dataRange.localIndex(secondRange.start()));
                        it = mList.insert(++it, secondPiece);
                        piece = &*it;
                        dataRange.setStart(secondRange.start());
                    }
                    QLinkedList<Piece>::Iterator firstSecondIt = it;

                    do {
                        if (dataRange.includes(secondRange.end())) {
                            // piece does not start at source?
                            if (secondRange.end() < dataRange.end()) {
                                // well, split and insert second piece, even if we move it later, just make it work for now
                                const Piece secondPiece = piece->splitAtLocal(dataRange.localIndex(secondRange.nextBehindEnd()));
                                it = mList.insert(++it, secondPiece);
                            } else {
                                ++it;
                            }
                            QLinkedList<Piece>::Iterator behindLastSecondIt = it;

                            for (it = firstSecondIt; it != behindLastSecondIt; ++it) {
                                firstIt = mList.insert(firstIt, *it) + 1;
                            }

                            mList.erase(firstSecondIt, behindLastSecondIt);

                            // done, move out of here
                            it = mList.end();
                            break;
                        }
                        dataRange.setStart(dataRange.nextBehindEnd());
                        ++it;

                        // removeRange is longer than content TODO: just quit or at least remove till the end?
                        if (it == mList.end()) {
                            break;
                        }
                        piece = &*it;
                        dataRange.setEndByWidth(piece->width());
                    } while (it != mList.end());
                } else {
                    dataRange.setStart(dataRange.nextBehindEnd());
                    ++it;

                    // removeRange is longer than content TODO: just quit or at least remove till the end?
                    if (it == mList.end()) {
                        break;
                    }
                    piece = &*it;
                    dataRange.setEndByWidth(piece->width());
                }
            } while (it != mList.end());
            break;
        }

        dataRange.setStart(dataRange.nextBehindEnd());
        ++it;
    }
}

Piece PieceTable::replaceOne(Address dataOffset, Address storageOffset, Piece::StorageType storageId)
{
    Piece::StorageType replacedStorageId = Piece::OriginalStorage;
    Address replacedStorageOffset = -1;

    auto it = mList.begin();

    AddressRange dataRange(0, -1);
    while (it != mList.end()) {
        Piece* const piece = &(*it);
        dataRange.setEndByWidth(piece->width());
        if (dataRange.includes(dataOffset)) {
            // collect replaced stprage type
            replacedStorageId = piece->storageId();

            const Piece replacePiece(storageOffset, 1, storageId);

            // piece starts at offset?
            if (dataRange.start() == dataOffset) {
                // collect replaced stprage offset
                replacedStorageOffset = piece->start();

                bool isApppendedToPrevious = false;
                // merge inserted piece with previous piece if possible
                if (it != mList.begin()) {
                    Piece& previousPiece = *(it - 1);
                    isApppendedToPrevious = previousPiece.append(replacePiece);
                }

                // existing piece 1:1 replacement?
                if (dataRange.width() == 1) {
                    // merge inserted piece with next piece if possible
                    bool isPrependedToNext = false;
                    bool isPreviousNextMerged = false;
                    const auto nextIt = it + 1;
                    if (nextIt != mList.end()) {
                        Piece& nextPiece = *nextIt;
                        if (isApppendedToPrevious) {
                            Piece& previousPiece = *(it - 1);
                            isPreviousNextMerged = previousPiece.append(nextPiece);
                        } else {
                            isPrependedToNext = nextPiece.prepend(replacePiece);
                        }
                    }
                    // if merged, drop existing piece, otherwise reuse it
                    if (isApppendedToPrevious || isPrependedToNext) {
                        it = mList.erase(it);
                        if (isPreviousNextMerged) {
                            mList.erase(it);
                        }
                    } else {
                        piece->set(storageOffset, storageOffset);
                        piece->setStorageId(storageId);
                    }
                } else {
                    // insert before current piece
                    if (!isApppendedToPrevious) {
                        mList.insert(it, replacePiece);
                    }
                    // chop off leading 1 from that
                    piece->moveStartBy(1);
                }
            } else if (dataRange.end() == dataOffset) {
                // collect replaced stprage offset
                replacedStorageOffset = piece->end();
                // assuming size > 1, piece with size == 1 would be handled in check for start above
                // chop off trailing 1 from current piece
                piece->moveEndBy(-1);
                // merge inserted piece with next piece if possible
                bool isPrependedToNext = false;
                const auto nextIt = it + 1;
                if (nextIt != mList.end()) {
                    Piece& nextPiece = *nextIt;
                    isPrependedToNext = nextPiece.prepend(replacePiece);
                }
                // insert after current piece
                if (!isPrependedToNext) {
                    mList.insert(nextIt, replacePiece);
                }
            } else {
                const Address localIndex = dataRange.localIndex(dataOffset);

                // collect replaced stprage offset
                replacedStorageOffset = piece->start() + localIndex;

                // split off second piece and insert after adapted current
                const Piece secondPiece = piece->removeLocal(AddressRange::fromWidth(localIndex, 1));
                ++it;
                it = mList.insert(it, replacePiece);
                ++it;
                mList.insert(it, secondPiece);
            }

            // done
            break;
        }

        // update loop state
        ++it;
        dataRange.setStart(dataRange.nextBehindEnd());
    }

    return Piece(replacedStorageOffset, 1, replacedStorageId);
}

}
