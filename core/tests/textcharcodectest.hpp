/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef TEXTCHARCODECTEST_HPP
#define TEXTCHARCODECTEST_HPP

// Qt
#include <QObject>

namespace Okteta {

class TextCharCodecTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testCreateCodec_data();
    void testCreateCodec();
    void testCreateLocalCodec();
    void testCreateNonexistingCodec();
};

}

#endif
