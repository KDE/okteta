/*
    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PIECELISTTEST_HPP
#define PIECELISTTEST_HPP

// Qt
#include <QObject>

namespace KPieceTable {

class PieceListTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testPieceConstructor_data();
    void testPieceConstructor();
    void testAppendPiece_data();
    void testAppendPiece();
    void testAppendPieceList_data();
    void testAppendPieceList();
    void testPrependPieceList_data();
    void testPrependPieceList();
};

}

#endif
