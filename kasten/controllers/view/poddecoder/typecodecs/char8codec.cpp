/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "char8codec.hpp"

// tool
#include "../types/char8.hpp"
#include "../poddata.hpp"
// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/Character>
// KF
#include <KLocalizedString>

namespace Okteta {

Char8Codec::Char8Codec(CharCodec* charCodec)
    : AbstractTypeCodec(i18nc("@label:textbox encoding of one byte as character", "Character 8-bit"))
    , mCharCodec(charCodec)
{}

Char8Codec::~Char8Codec() = default;

QVariant Char8Codec::value(const PODData& data, int* byteCount) const
{
    const unsigned char* pointer = (unsigned char*)data.pointer(1);

    *byteCount = pointer ? 1 : 0;
    QVariant result;
    if (pointer) {
        const Okteta::Character decodedChar = mCharCodec->decode(*pointer);

        result = QVariant::fromValue<Char8>(Char8(decodedChar));
    }

    return result;
}

QByteArray Char8Codec::valueToBytes(const QVariant& value) const
{
    const Okteta::Character character = value.value<Char8>().character;

    bool success = (!character.isUndefined());
    Okteta::Byte byte;

    if (success) {
        success = mCharCodec->encode(&byte, character);
    }

    return success ? QByteArray(1, byte) : QByteArray();
}

bool Char8Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Char8>().character == otherValue.value<Char8>().character);
}

}
