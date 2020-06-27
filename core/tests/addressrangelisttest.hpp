/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef ADDRESSRANGELISTTEST_HPP
#define ADDRESSRANGELISTTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class AddressRangeListTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testSimpleConstructor();
    void testAddSingleRange();
    void testAddJoinableRanges();
    void testAddNonJoinableRanges();
};

}

#endif
