/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYVALUESTRINGDECODERTEST_HPP
#define OKTETA_BYTEARRAYVALUESTRINGDECODERTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class ByteArrayValueStringDecoderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testDecode();
    void testDecode_data();
};

}

#endif
