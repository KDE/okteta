/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SELECTIONTEST_HPP
#define SELECTIONTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class SelectionTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testPlainConstructor();
    void testAnchorConstructor();
    void testCopyConstructor();
    void testCompare();
    void testSetStart();
    void testSetStartEnd();
    void testCancel();
    void testSetForward();
    void testReverse();
};

}

#endif
