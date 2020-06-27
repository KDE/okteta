/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef TESTPIECETABLECHANGEABSTRACTPIECETABLECHANGEIFTEST_HPP
#define TESTPIECETABLECHANGEABSTRACTPIECETABLECHANGEIFTEST_HPP

// test
#include "abstractpiecetablechangeiftest.hpp"

namespace KPieceTable {

class TestPieceTableChangeAbstractPieceTableChangeIfTest : public AbstractPieceTableChangeIfTest
{
    Q_OBJECT

protected: // AbstractPieceTableChangeIfTest API
    AbstractPieceTableChange* createPieceTableChange() override;
    void deletePieceTableChange(AbstractPieceTableChange* pieceTableChange) override;
    void changePieceTable(PieceTable* pieceTable) override;
};

}

#endif
