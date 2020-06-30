/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "ebcdic1047charcodeccharcodeciftest.hpp"

// test object
#include <codecs/ebcdic1047charcodec.hpp>
// Qt
#include <QTest>

namespace Okteta {

CharCodec* EBCDIC1047CharCodecCharCodecIfTest::createCodec()
{
    return EBCDIC1047CharCodec::create();
}

void EBCDIC1047CharCodecCharCodecIfTest::deleteCodec(CharCodec* codec)
{
    delete codec;
}

}

QTEST_GUILESS_MAIN(Okteta::EBCDIC1047CharCodecCharCodecIfTest)
