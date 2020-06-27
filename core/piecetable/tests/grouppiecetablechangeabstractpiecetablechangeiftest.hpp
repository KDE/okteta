/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef GROUPPIECETABLECHANGEABSTRACTPIECETABLECHANGEIFTEST_HPP
#define GROUPPIECETABLECHANGEABSTRACTPIECETABLECHANGEIFTEST_HPP

// test
#include "abstractpiecetablechangeiftest.hpp"

namespace KPieceTable {

class GroupPieceTableChangeAbstractPieceTableChangeIfTest : public AbstractPieceTableChangeIfTest
{
protected: // AbstractPieceTableChangeIfTest API
    virtual AbstractPieceTableChange* createPieceTableChange();
    virtual void deletePieceTableChange(AbstractPieceTableChange* pieceTableChange);
    virtual void changePieceTable(PieceTable* pieceTable);
};

}

#endif
