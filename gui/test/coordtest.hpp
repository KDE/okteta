/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KBUFFERCOORDTEST_HPP
#define KBUFFERCOORDTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class CoordTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testConstructor();
    void testSetConstructor();
    void testFromIndexConstructor();
    void testCopyConstructor();
    void testEquals();
    void testAssign();
    void testSetPosLine();
    void testSet();
    void testIsAtStart();
    void testGotoStart();
    void testGo();
    void testGoN();
    void testGotoControlled();
    void testGoLineStart();
    void testGoLineEnd();
    void testCompareOperator();
    void testIsBelowIsAbove();
    void testIsLaterPriorInLineThan();
    void testIsBeforeLineStart();
    void testIsBeforeLineEnd();
    void testIndexNWidth();
};

}

#endif
