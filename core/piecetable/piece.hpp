/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_PIECE_HPP
#define KPIECETABLE_PIECE_HPP

// lib
#include <addressrange.hpp>

namespace KPieceTable {

using Size = Okteta::Size;
using Address = Okteta::Address;
using AddressRange = Okteta::AddressRange;

class Piece : public AddressRange
{
public:
    enum {
        OriginalStorage,
        ChangeStorage
    };

public:
    Piece(Address storageOffset, Size size, int storageId);
    Piece(const AddressRange& storageRange, int storageId);
    Piece();

public:
    int storageId() const;

public:
    void setStorageId(int storageId);
    Piece splitAt(Address storageOffset);
    Piece splitAtLocal(Address localStorageOffset);
    Piece remove(const AddressRange& removeStorageRange);
    Piece removeLocal(const AddressRange& localRemoveStorageRange);
    Piece removeStartBeforeLocal(Address storageOffset);
    Piece removeEndBehindLocal(Address storageOffset);
    bool prepend(const Piece& other);
    bool append(const Piece& other);

public:
    Piece subPiece(const AddressRange& local) const;

private:
    int mStorageId = OriginalStorage;
};

inline Piece::Piece(Address storageOffset, Size size, int storageId)
    : AddressRange(AddressRange::fromWidth(storageOffset, size))
    , mStorageId(storageId)
{}
inline Piece::Piece(const AddressRange& storageRange, int storageId)
    : AddressRange(storageRange)
    , mStorageId(storageId)
{}
inline Piece::Piece() = default;

inline int Piece::storageId() const { return mStorageId; }

inline void Piece::setStorageId(int storageId) { mStorageId = storageId; }

inline Piece Piece::splitAt(Address storageOffset)
{
    return Piece(AddressRange::splitAt(storageOffset), mStorageId);
}
inline Piece Piece::splitAtLocal(Address localStorageOffset)
{
    return Piece(AddressRange::splitAtLocal(localStorageOffset), mStorageId);
}
inline Piece Piece::remove(const AddressRange& removeStorageRange)
{
    return Piece(AddressRange::remove(removeStorageRange), mStorageId);
}
inline Piece Piece::removeLocal(const AddressRange& localRemoveStorageRange)
{
    return Piece(AddressRange::removeLocal(localRemoveStorageRange), mStorageId);
}
inline Piece Piece::removeStartBeforeLocal(Address storageOffset)
{
    const Address oldStart = start();
    moveStartBy(storageOffset);
    return Piece(AddressRange(oldStart, nextBeforeStart()), mStorageId);
}
inline Piece Piece::removeEndBehindLocal(Address storageOffset)
{
    const Address oldEnd = end();
    setEndByWidth(storageOffset + 1);
    return Piece(AddressRange(nextBehindEnd(), oldEnd), mStorageId);
}

inline Piece Piece::subPiece(const AddressRange& local) const
{
    return Piece(AddressRange::subRange(local), mStorageId);
}

inline bool Piece::prepend(const Piece& other)
{
    const bool result = (mStorageId == other.mStorageId && AddressRange::prepend(other));
    return result;
}
inline bool Piece::append(const Piece& other)
{
    const bool result = (mStorageId == other.mStorageId && AddressRange::append(other));
    return result;
}

}

Q_DECLARE_TYPEINFO(KPieceTable::Piece, Q_MOVABLE_TYPE);

#endif
