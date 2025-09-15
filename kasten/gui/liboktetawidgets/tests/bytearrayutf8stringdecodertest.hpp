/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYUTF8STRINGDECODERTEST_HPP
#define OKTETA_BYTEARRAYUTF8STRINGDECODERTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class ByteArrayUtf8StringDecoderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testDecode();
    void testDecode_data();
    void testDecodeMaxSize();
    void testDecodeMaxSize_data();
};

}

#endif
