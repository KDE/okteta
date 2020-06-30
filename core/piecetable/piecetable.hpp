/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_PIECETABLE_HPP
#define KPIECETABLE_PIECETABLE_HPP

// lib
#include "piecelist.hpp"
// Qt
#include <QLinkedList>

namespace KPieceTable {

class PieceTable
{
public:
    explicit PieceTable(Size size = 0);

public:
    void init(Size size);
    void insert(Address insertDataOffset, Size insertLength, Address storageOffset);
    /// for use to reapply
    void insert(Address insertDataOffset, const PieceList& insertPieceList);
    PieceList remove(const AddressRange& removeRange);
    PieceList replace(const AddressRange& removeRange,
                      Size insertLength, Address storageOffset);
    /// for use to reapply
    void replace(const AddressRange& removeRange, const PieceList& insertPieceList);
    void swap(Address firstStart, const AddressRange& secondRange);
    Piece replaceOne(Address dataOffset, Address storageOffset, int storageId = Piece::ChangeStorage);

//     int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 ); TODO: filter change, calculated

public:
    bool getStorageData(int* storageId, Address* storageOffset, Address dataOffset) const;
    Size size() const;

private:
    QLinkedList<Piece> mList;
    Size mSize;
};

inline Size PieceTable::size() const { return mSize; }

}

#endif
