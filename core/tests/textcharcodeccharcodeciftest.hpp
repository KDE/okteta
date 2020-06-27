/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef TEXTCHARCODECCHARCODECIFTEST_HPP
#define TEXTCHARCODECCHARCODECIFTEST_HPP

// test
#include "charcodeciftest.hpp"

namespace Okteta {

class TextCharCodecCharCodecIfTest : public CharCodecIfTest
{
    Q_OBJECT

protected: // CharCodecIfTest API
    CharCodec* createCodec() override;
    void deleteCodec(CharCodec* codec) override;
};

}

#endif
