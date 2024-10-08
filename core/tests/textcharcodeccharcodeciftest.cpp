/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "textcharcodeccharcodeciftest.hpp"

// lib
#include <charcodecs/textcharcodec.hpp>
// Qt
#include <QTest>

namespace Okteta {

std::unique_ptr<CharCodec> TextCharCodecCharCodecIfTest::createCodec()
{
    return TextCharCodec::createCodec(QStringLiteral("ISO8859-1"));
    // TODO: test for some more, e.g. KOI8-R
}

void TextCharCodecCharCodecIfTest::deleteCodec(std::unique_ptr<CharCodec>&& codec)
{
    Q_UNUSED(codec)
}

}

QTEST_GUILESS_MAIN(Okteta::TextCharCodecCharCodecIfTest)

#include "moc_textcharcodeccharcodeciftest.cpp"
