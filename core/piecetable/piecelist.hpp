/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_PIECELIST_HPP
#define KPIECETABLE_PIECELIST_HPP

// lib
#include "piece.hpp"
// Qt
#include <QVector>

namespace KPieceTable {

class PieceList
{
public:
    PieceList();
    explicit PieceList(const Piece& piece);

    ~PieceList();

public:
    int size() const;
    bool isEmpty() const;
    Size totalLength() const;
    const Piece& at(int i) const;

public:
    void append(const PieceList& other);
    void append(const Piece& piece);
    void prepend(const PieceList& other);

private:
    QVector<Piece> mList;
    Size mTotalLength = 0;
};

inline PieceList::PieceList() = default;
inline PieceList::PieceList(const Piece& piece)
{
    append(piece);
}

inline PieceList::~PieceList() = default;

inline int PieceList::size()             const { return mList.size(); }
inline bool PieceList::isEmpty()         const { return mList.isEmpty(); }
inline Size PieceList::totalLength()     const { return mTotalLength; }
inline const Piece& PieceList::at(int i) const { return mList.at(i); }

inline void PieceList::append(const Piece& piece)
{
    bool isMerged = false;
    if (!mList.isEmpty()) {
        isMerged = mList.last().append(piece);
    }
    if (!isMerged) {
        mList.append(piece);
    }
    mTotalLength += piece.width();
}
inline void PieceList::append(const PieceList& other)
{
    QVector<Piece>::ConstIterator it = other.mList.begin();

    // see if the ones at the border can be merged
    bool isMerged = false;
    if (!mList.isEmpty() && !other.mList.isEmpty()) {
        isMerged = mList.last().append(other.mList.first());
    }
    if (isMerged) {
        ++it;
    }

    std::for_each(it, other.mList.end(), [this](const Piece& piece) {
        mList.append(piece);
    });
// was:     mList += other.mList;

    mTotalLength += other.mTotalLength;
}
inline void PieceList::prepend(const PieceList& other)
{
    QVector<Piece> otherCopy = other.mList;
    QVector<Piece>::Iterator it = mList.begin();

    // see if the ones at the border can be merged
    bool isMerged = false;
    if (!otherCopy.isEmpty() && !mList.isEmpty()) {
        isMerged = otherCopy.last().append(mList.first());
    }
    if (isMerged) {
        ++it;
    }

    std::for_each(it, mList.end(), [&otherCopy](const Piece& piece) mutable {
        otherCopy.append(piece);
    });

    mList = otherCopy;
// was:     mList = other.mList + mList;

    mTotalLength += other.mTotalLength;
}

}

#endif
