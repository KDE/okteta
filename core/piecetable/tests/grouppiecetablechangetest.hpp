/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef GROUPPIECETABLECHANGETEST_HPP
#define GROUPPIECETABLECHANGETEST_HPP

// Qt
#include <QObject>

namespace KPieceTable {

class GroupPieceTableChangeTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testConstructor();
    void testAppendChange();
//     void testRevertBeforeChange();
};

}

#endif
