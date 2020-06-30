/*
    This file is part of the Okteta Core library, made within the KDE community.

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
    void testSplitAt();
    void testSplitAtLocal();
    void testRemove();
    void testRemoveLocal();
};

}

#endif
