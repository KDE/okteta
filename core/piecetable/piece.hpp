/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_PIECE_HPP
#define KPIECETABLE_PIECE_HPP

// lib
#include <addressrange.hpp>
// Qt
#include <QTypeInfo>

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
    Piece(const Piece&) = default; // trivial
    Piece(Piece&&) = default; // trivial

    ~Piece() = default; // trivial

    Piece& operator=(const Piece&) = default; // trivial
    Piece& operator=(Piece&&) = default; // trivial

public:
    [[nodiscard]]
    int storageId() const;

public:
    void setStorageId(int storageId);
    [[nodiscard]]
    Piece splitAt(Address storageOffset);
    [[nodiscard]]
    Piece splitAtLocal(Address localStorageOffset);
    Piece remove(const AddressRange& removeStorageRange);
    Piece removeLocal(const AddressRange& localRemoveStorageRange);
    Piece removeStartBeforeLocal(Address storageOffset);
    Piece removeEndBehindLocal(Address storageOffset);
    [[nodiscard]]
    bool prepend(const Piece& other);
    [[nodiscard]]
    bool append(const Piece& other);

public:
    [[nodiscard]]
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
    return {AddressRange::splitAt(storageOffset), mStorageId};
}
inline Piece Piece::splitAtLocal(Address localStorageOffset)
{
    return {AddressRange::splitAtLocal(localStorageOffset), mStorageId};
}
inline Piece Piece::remove(const AddressRange& removeStorageRange)
{
    return {AddressRange::remove(removeStorageRange), mStorageId};
}
inline Piece Piece::removeLocal(const AddressRange& localRemoveStorageRange)
{
    return {AddressRange::removeLocal(localRemoveStorageRange), mStorageId};
}
inline Piece Piece::removeStartBeforeLocal(Address storageOffset)
{
    const Address oldStart = start();
    moveStartBy(storageOffset);
    return {AddressRange(oldStart, nextBeforeStart()), mStorageId};
}
inline Piece Piece::removeEndBehindLocal(Address storageOffset)
{
    const Address oldEnd = end();
    setEndByWidth(storageOffset + 1);
    return {AddressRange(nextBehindEnd(), oldEnd), mStorageId};
}

inline Piece Piece::subPiece(const AddressRange& local) const
{
    return {AddressRange::subRange(local), mStorageId};
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

Q_DECLARE_TYPEINFO(KPieceTable::Piece, Q_RELOCATABLE_TYPE);

#endif
