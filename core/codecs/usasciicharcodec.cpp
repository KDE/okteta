/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "usasciicharcodec.hpp"

// lib
#include <character.hpp>
// Qt
#include <QString>

namespace Okteta {

bool USASCIICharCodec::encode(Byte* byte, QChar _char) const
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

bool USASCIICharCodec::canEncode(QChar _char) const
{
    return (_char.unicode() <= 0x007F);
}

QString USASCIICharCodec::name() const
{
    return codecName();
}

const QString& USASCIICharCodec::codecName()
{
    static const QString name = QStringLiteral("US-ASCII");
    return name;
}

}
