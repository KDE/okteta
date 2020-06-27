/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef VALUECODECTEST_HPP
#define VALUECODECTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class ValueCodecTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testCreateCodec_data();
    void testCreateCodec();
    void testEncodeDecode_data();
    void testEncodeDecode();
    void testEncodeShortDecode_data();
    void testEncodeShortDecode();
    void testAppendDigit_data();
    void testAppendDigit();
    void testIsValidDigit_data();
    void testIsValidDigit();
    void testRemoveLastDigit_data();
    void testRemoveLastDigit();
};

}

#endif
