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
    explicit
    TestPieceTableChange(int typeId = -1, const QString& description = QString(),
                         Address position = 0, Address storagePosition = 0, int storageId = Piece::ChangeStorage,
                         Address replacedStoragePosition = 0, int replacedStorageId = Piece::ChangeStorage);

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
    Address mPosition;
    Address mStoragePosition;
    int mStorageId;

    Address mReplacedStoragePosition;
    int mReplacedStorageId;
};

inline TestPieceTableChange::TestPieceTableChange(int typeId, const QString& description,
                                                  Address position, Address storagePosition, int storageId,
                                                  Address replacedStoragePosition, int replacedStorageId)
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
