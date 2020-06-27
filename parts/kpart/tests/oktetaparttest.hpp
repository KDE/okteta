/*
    This file is part of the Okteta KPart module, made within the KDE community.

    SPDX-FileCopyrightText: 2016 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETAPARTTEST_HPP
#define OKTETAPARTTEST_HPP

// Qt
#include <QObject>

class OktetaPartTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testPartReuseWithAnotherUrl(); // bug #365523
};

#endif
