/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usasciicharcodec.hpp"

// lib
#include <character.hpp>
// Qt
#include <QString>

namespace Okteta {

bool USASCIICharCodec::encode(Byte* byte, const QChar& _char) const
{
    const int unicodeValue = _char.unicode();
    // not in range?
    if (0x007F < unicodeValue) {
        return false;
    }

    *byte = unicodeValue;

    return true;
}

Character USASCIICharCodec::decode(Byte byte) const
{
    return {QChar(ushort(byte)), (byte > 0x007F)};
}

bool USASCIICharCodec::canEncode(const QChar& _char) const
{
    return (_char.unicode() <= 0x007F);
}

const QString& USASCIICharCodec::name() const
{
    return codecName();
}

const QString& USASCIICharCodec::codecName()
{
    static const QString name = QStringLiteral("US-ASCII");
    return name;
}

}
