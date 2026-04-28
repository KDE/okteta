/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PIECETEST_HPP
#define PIECETEST_HPP

// Qt
#include <QObject>

namespace KPieceTable {

class PieceTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testFullConstructor();
    void testEqualOperator_data();
    void testEqualOperator();
    void testSetStorageId();
    void testSplitAt_data();
    void testSplitAt();
    void testSplitAtLocal_data();
    void testSplitAtLocal();
    void testRemove_data();
    void testRemove();
    void testRemoveLocal_data();
    void testRemoveLocal();
    void testRemoveStartBeforeLocal_data();
    void testRemoveStartBeforeLocal();
    void testRemoveEndBehindLocal_data();
    void testRemoveEndBehindLocal();
    void testPrepend_data();
    void testPrepend();
    void testAppend_data();
    void testAppend();
    void testSubPiece_data();
    void testSubPiece();
};

}

#endif
