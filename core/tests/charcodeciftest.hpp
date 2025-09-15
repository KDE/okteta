/*
    SPDX-FileCopyrightText: 2006, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef CHARCODECIFTEST_HPP
#define CHARCODECIFTEST_HPP

// Qt
#include <QObject>
// Std
#include <memory>

namespace Okteta {
class CharCodec;

class CharCodecIfTest : public QObject
{
    Q_OBJECT

protected:
    CharCodecIfTest();

protected: // API to be implemented
    virtual std::unique_ptr<CharCodec> createCodec() = 0;
    virtual void deleteCodec(std::unique_ptr<CharCodec>&& codec) = 0;

private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testEncodeDecode_data();
    void testEncodeDecode();

private:
    /** the codec to test */
    std::unique_ptr<CharCodec> mCharCodec;
};

inline CharCodecIfTest::CharCodecIfTest() = default;

}

#endif
