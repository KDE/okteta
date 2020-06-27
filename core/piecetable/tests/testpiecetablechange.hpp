/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_TESTPIECETABLECHANGE_HPP
#define KPIECETABLE_TESTPIECETABLECHANGE_HPP

// lib
#include <piecetable/abstractpiecetablechange.hpp>
#include <piecetable/piece.hpp>
// Qt
#include <QString>

namespace KPieceTable {

class TestPieceTableChange : public AbstractPieceTableChange
{
public:
    TestPieceTableChange(int typeId = -1, const QString& description = QString(),
                         int position = 0, int storagePosition = 0, int storageId = Piece::ChangeStorage,
                         int replacedStoragePosition = 0, int replacedStorageId = Piece::ChangeStorage);

    ~TestPieceTableChange() override;

public: // AbstractPieceTableChange API
    int type() const override;
    QString description() const override;
    bool merge(const AbstractPieceTableChange* other) override;
    AddressRange apply(PieceTable* pieceTable) const override;
    AddressRange revert(PieceTable* pieceTable) const override;
    ArrayChangeMetrics metrics() const override;
    int dataSize() const override;

private:
    int mTypeId;
    QString mDescription;
    int mPosition;
    int mStoragePosition;
    int mStorageId;

    int mReplacedStoragePosition;
    int mReplacedStorageId;
};

inline TestPieceTableChange::TestPieceTableChange(int typeId, const QString& description,
                                                  int position, int storagePosition, int storageId,
                                                  int replacedStoragePosition, int replacedStorageId)
    : mTypeId(typeId)
    , mDescription(description)
    , mPosition(position)
    , mStoragePosition(storagePosition)
    , mStorageId(storageId)
    , mReplacedStoragePosition(replacedStoragePosition)
    , mReplacedStorageId(replacedStorageId)
{}

}

#endif
