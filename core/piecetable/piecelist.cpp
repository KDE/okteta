/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecelist.hpp"

namespace KPieceTable {

void PieceList::append(const Piece& piece)
{
    if (!piece.isValid()) {
        return;
    }

    bool isMerged = false;
    if (!mList.isEmpty()) {
        isMerged = mList.last().append(piece);
    }
    if (!isMerged) {
        mList.append(piece);
    }
    mTotalLength += piece.width();
}

void PieceList::append(const PieceList& other)
{
    if (other.isEmpty()) {
        return;
    }
    if (mList.isEmpty()) {
        *this = other;
        return;
    }

    QVector<Piece>::ConstIterator it = other.mList.begin();

    // see if the ones at the border can be merged
    const bool isMerged = mList.last().append(other.mList.first());
    if (isMerged) {
        ++it;
    }

    std::for_each(it, other.mList.end(), [this](const Piece& piece) {
        mList.append(piece);
    });
// was:     mList += other.mList;

    mTotalLength += other.mTotalLength;
}

void PieceList::prepend(const PieceList& other)
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
