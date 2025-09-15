/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYVALIDATORTEST_HPP
#define OKTETA_BYTEARRAYVALIDATORTEST_HPP

// sut
#include <bytearrayvalidator.hpp>
// Qt
#include <QObject>

class QByteArray;
class QString;

namespace Okteta {

class ByteArrayValidatorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testKeepMinMaxBetweenCodecs();
    void testRoundtrip_data();
    void testRoundtrip();
    void testToByteArraay_data();
    void testToByteArraay();
    void testToString_data();
    void testToString();
    void testValidate_data();
    void testValidate();

private:
    static QByteArray createFullByteSequence();
    static QString createString(ByteArrayValidator::Coding coding, int size);
    static QString createBadString(ByteArrayValidator::Coding coding, int size, const QString& charCodecName);
};

}

#endif
