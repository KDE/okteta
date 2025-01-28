/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef BYTEARRAYTABLELAYOUTTEST_HPP
#define BYTEARRAYTABLELAYOUTTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class ByteArrayTableLayoutTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testConstructor();
    void testSetStartOffset();
    void testSetFirstLineOffset();
    void testSetNoOfBytesPerLine();
    void testSetByteArrayOffset();
    void testSetLength();
    void testSetNoOfLinesPerPage();
};

}

#endif
