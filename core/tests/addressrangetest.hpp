/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef ADDRESSRANGETEST_HPP
#define ADDRESSRANGETEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class AddressRangeTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testConstructorByEnd();
    void testConstructorByWidth();
    void testSimpleConstructor();
    void testCopyConstructor();
    void testSetGetStart();
    void testSetGetEnd();
    void testSetGetWidth();
    void testIsEmpty();
    void testCompare();
    void testIncludes();
    void testLocalIndex();
    void testLocalRange();
    void testSubRange();
    void testAdaptToReplacement();
    void testSplitAt();
    void testSplitAtLocal();
    void testRemove();
    void testRemoveLocal();
    void testStartForInclude();
    void testIsJoinable();
};

}

#endif
