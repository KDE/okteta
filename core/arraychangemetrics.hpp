/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ARRAYCHANGEMETRICS_HPP
#define OKTETA_ARRAYCHANGEMETRICS_HPP

// lib
#include "oktetacore_export.hpp"
#include "size.hpp"
#include "address.hpp"
// Qt
#include <QDataStream>

namespace Okteta {

// TODO: do we need the invalid status?
class OKTETACORE_EXPORT ArrayChangeMetrics
{
    friend QDataStream& operator<<(QDataStream& outStream, const ArrayChangeMetrics& metrics);
    friend QDataStream& operator>>(QDataStream& inStream, ArrayChangeMetrics& metrics);

private:
    static constexpr Address InvalidAddress = -1;

public:
    enum Type
    {
        // Insertion,
        // Removal,
        Replacement,
        Swapping,
        // Filling,
        // Setting,
        Invalid
    };

public:
    static constexpr ArrayChangeMetrics asReplacement(Address offset, Size removeLength, Size insertLength);
    static constexpr ArrayChangeMetrics asSwapping(Address firstOffset, Address secondOffset, Size secondLength);

public:
    ArrayChangeMetrics() = default;
    constexpr ArrayChangeMetrics(Type type, Address offset, qint32 secondArgument, qint32 thirdArgument);

public:
    constexpr bool operator==(const ArrayChangeMetrics& other) const;

public:
    void revert();

public:
    constexpr int type() const;
    constexpr Address offset() const;
    constexpr bool isValid() const;

public: // Replacement properties
    constexpr Size removeLength() const;
    constexpr Size insertLength() const;
    constexpr Size lengthChange() const;

public: // Swapping properties
    constexpr Size firstLength() const;
    constexpr Address secondStart() const;
    constexpr Address secondEnd() const;
    constexpr Size secondLength() const;

private:
    Type mType = Invalid;
    Address mOffset = InvalidAddress;
    // TODO: how to make the arguments of the size of the largest union member?
    union
    {
        qint32 mSecondArgument = 0;
        Size mRemoveLength;
        Address mSecondStart;
    };
    union
    {
        qint32 mThirdArgument = 0;
        Size mInsertLength;
        Size mSecondLength;
    };
};

inline constexpr ArrayChangeMetrics ArrayChangeMetrics::asReplacement(Address offset, Size removeLength, Size insertLength)
{
    return {Replacement, offset, removeLength, insertLength};
}

inline constexpr ArrayChangeMetrics ArrayChangeMetrics::asSwapping(Address firstOffset, Address secondOffset, Size secondLength)
{
    return
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        Q_ASSERT(firstOffset < secondOffset),
#endif
        ArrayChangeMetrics{Swapping, firstOffset, secondOffset, secondLength};
}

inline constexpr ArrayChangeMetrics::ArrayChangeMetrics(Type type, Address offset, qint32 secondArgument, qint32 thirdArgument)
    : mType(type)
    , mOffset(offset)
    , mSecondArgument(secondArgument)
    , mThirdArgument(thirdArgument)
{}
inline constexpr bool ArrayChangeMetrics::operator==(const ArrayChangeMetrics& other) const
{
    return mType == other.mType
           && mOffset == other.mOffset
           && mSecondArgument == other.mSecondArgument
           && mThirdArgument == other.mThirdArgument;
}
inline void ArrayChangeMetrics::revert()
{
    if (mType == Replacement) {
        const Size helper = mInsertLength;
        mInsertLength = mRemoveLength;
        mRemoveLength = helper;
    } else if (mType == Swapping) {
        const Size oldSecondLength = mSecondLength;
        mSecondLength = firstLength();
        mSecondStart = mOffset + oldSecondLength;
    }
}

inline constexpr bool ArrayChangeMetrics::isValid()        const { return mOffset != InvalidAddress; }
inline constexpr int ArrayChangeMetrics::type()            const { return mType; }
inline constexpr Address ArrayChangeMetrics::offset()      const { return mOffset; }
inline constexpr Size ArrayChangeMetrics::removeLength()   const { return mRemoveLength; }
inline constexpr Size ArrayChangeMetrics::insertLength()   const { return mInsertLength; }
inline constexpr Size ArrayChangeMetrics::lengthChange()   const { return mInsertLength - mRemoveLength; }
inline constexpr Address ArrayChangeMetrics::secondStart() const { return mSecondStart; }
inline constexpr Address ArrayChangeMetrics::secondEnd()   const { return mSecondStart + mSecondLength - 1; }
inline constexpr Size ArrayChangeMetrics::firstLength()    const { return mSecondStart - mOffset; }
inline constexpr Size ArrayChangeMetrics::secondLength()   const { return mSecondLength; }

QDataStream& operator<<(QDataStream& outStream, const ArrayChangeMetrics& metrics);
QDataStream& operator>>(QDataStream& inStream, ArrayChangeMetrics& metrics);

inline QDataStream& operator<<(QDataStream& outStream, const ArrayChangeMetrics& metrics)
{
    outStream << metrics.mType << metrics.mOffset << metrics.mSecondArgument << metrics.mThirdArgument;
    return outStream;
}

inline QDataStream& operator>>(QDataStream& inStream, ArrayChangeMetrics& metrics)
{
    int type;
    inStream >> type >> metrics.mOffset >> metrics.mSecondArgument >> metrics.mThirdArgument;
    metrics.mType = (ArrayChangeMetrics::Type)type; // TODO: find out how to stream to enum directly
    return inStream;
}

}

Q_DECLARE_TYPEINFO(Okteta::ArrayChangeMetrics, Q_MOVABLE_TYPE);

#endif
