/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_PIECELIST_HPP
#define KPIECETABLE_PIECELIST_HPP

// lib
#include "piece.hpp"
// Qt
#include <QList>

namespace KPieceTable {

class PieceList
{
public:
    PieceList();
    explicit PieceList(const Piece& piece);
    PieceList(const PieceList&);
    PieceList(PieceList&&);

    ~PieceList();

    PieceList& operator=(const PieceList&) = default;
    PieceList& operator=(PieceList&&) = default;

public:
    [[nodiscard]]
    int size() const;
    [[nodiscard]]
    bool isEmpty() const;
    [[nodiscard]]
    Size totalLength() const;
    [[nodiscard]]
    const Piece& at(int i) const;

public:
    void append(const PieceList& other);
    void append(const Piece& piece);
    void prepend(const PieceList& other);

public: // introspection API, used for tests
    using ConstIterator = QList<Piece>::ConstIterator;

    [[nodiscard]]
    ConstIterator begin() const;
    [[nodiscard]]
    ConstIterator end() const;

private:
    QList<Piece> mList;
    Size mTotalLength = 0;
};

inline PieceList::PieceList() = default;
inline PieceList::PieceList(const PieceList&) = default;
inline PieceList::PieceList(const Piece& piece)
{
    append(piece);
}
inline PieceList::PieceList(PieceList&&) = default;

inline PieceList::~PieceList() = default;

inline int PieceList::size()             const { return mList.size(); }
inline bool PieceList::isEmpty()         const { return mList.isEmpty(); }
inline Size PieceList::totalLength()     const { return mTotalLength; }
inline const Piece& PieceList::at(int i) const { return mList.at(i); }

inline PieceList::ConstIterator PieceList::begin() const { return mList.begin(); }
inline PieceList::ConstIterator PieceList::end()   const { return mList.end(); }

}

#endif
