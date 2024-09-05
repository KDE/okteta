/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYCHAR8STRINGENCODERTEST_HPP
#define OKTETA_BYTEARRAYCHAR8STRINGENCODERTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class ByteArrayChar8StringEncoderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testEncode();
    void testEncode_data();
};

}

#endif
