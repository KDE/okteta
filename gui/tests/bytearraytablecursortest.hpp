/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef BYTEARRAYTABLECURSORTEST_HPP
#define BYTEARRAYTABLECURSORTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class ByteArrayTableCursorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testConstructor();
    void testSetAppendPosEnabled();
    void testGotoIndex();
    void testGotoCIndex();
    void testGotoRealIndex();
    void testGotoStart();
    void testGotoEnd();
    void testGotoNextByte();
    void testGotoPreviousByte();
    void testGotoNextByteN();
    void testGotoPreviousByteN();
};

}

#endif
