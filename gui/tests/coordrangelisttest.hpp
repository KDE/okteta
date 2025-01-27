/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef COORDRANGELISTTEST_HPP
#define COORDRANGELISTTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class CoordRangeListTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testAddSingleSection();
    void testAddJoinableSections();
    void testAddNonJoinableSections();
};

}

#endif
