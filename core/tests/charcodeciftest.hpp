/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef CHARCODECIFTEST_HPP
#define CHARCODECIFTEST_HPP

// Qt
#include <QObject>

namespace Okteta {
class CharCodec;

class CharCodecIfTest : public QObject
{
    Q_OBJECT

protected:
    CharCodecIfTest();

protected: // API to be implemented
    virtual CharCodec* createCodec() = 0;
    virtual void deleteCodec(CharCodec* codec) = 0;

private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testEncodeDecode_data();
    void testEncodeDecode();

private:
    /** pointer to the buffer to test */
    CharCodec* mCharCodec = nullptr;
};

inline CharCodecIfTest::CharCodecIfTest() = default;

}

#endif
