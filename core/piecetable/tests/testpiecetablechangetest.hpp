/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef TESTPIECETABLECHANGETEST_HPP
#define TESTPIECETABLECHANGETEST_HPP

// Qt
#include <QObject>

namespace KPieceTable {

class TestPieceTableChangeTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testMerge();
};

}

#endif
