/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYCHAR8STRINGCODECROUNDTRIPTEST_HPP
#define OKTETA_BYTEARRAYCHAR8STRINGCODECROUNDTRIPTEST_HPP

// Qt
#include <QObject>

class QByteArray;

namespace Okteta {

class ByteArrayChar8StringCodecRoundtripTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testCodecRoundtrip();
    void testCodecRoundtrip_data();

private:
    static QByteArray createFullByteSequence();
};

}

#endif
