/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef REPLACEPIECETABLECHANGEABSTRACTPIECETABLECHANGEIFTEST_HPP
#define REPLACEPIECETABLECHANGEABSTRACTPIECETABLECHANGEIFTEST_HPP

// test
#include "abstractpiecetablechangeiftest.hpp"

namespace KPieceTable {

class ReplacePieceTableChangeAbstractPieceTableChangeIfTest : public AbstractPieceTableChangeIfTest
{
    Q_OBJECT

protected: // AbstractPieceTableChangeIfTest API
    std::unique_ptr<AbstractPieceTableChange> createPieceTableChange() override;
    void deletePieceTableChange(std::unique_ptr<AbstractPieceTableChange>&& pieceTableChange) override;
    void changePieceTable(PieceTable* pieceTable) override;
};

}

#endif
