/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PIECETABLETEST_HPP
#define PIECETABLETEST_HPP

// Qt
#include <QObject>

namespace KPieceTable {

class PieceTableTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testInit();
    void testInsert_data();
    void testInsert();
    void testInsertMulti();
    void testRemove_data();
    void testRemove();
    void testSwap_data();
    void testSwap();
};

}

#endif
