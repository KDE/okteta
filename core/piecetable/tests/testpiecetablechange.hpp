/*
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
                         int position = 0, int storagePosition = 0, int storageId = Piece::ChangeStorage,
                         int replacedStoragePosition = 0, int replacedStorageId = Piece::ChangeStorage);

    ~TestPieceTableChange() override;

public: // AbstractPieceTableChange API
    [[nodiscard]]
    int type() const override;
    [[nodiscard]]
    QString description() const override;
    [[nodiscard]]
    bool merge(const AbstractPieceTableChange* other) override;
    [[nodiscard]]
    AddressRange apply(PieceTable* pieceTable) const override;
    [[nodiscard]]
    AddressRange revert(PieceTable* pieceTable) const override;
    [[nodiscard]]
    ArrayChangeMetrics metrics() const override;
    [[nodiscard]]
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
