/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "hexadecimal8codec.hpp"

// tool
#include "../types/hexadecimal8.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

Hexadecimal8Codec::Hexadecimal8Codec()
    : AbstractTypeCodec(i18nc("@label:textbox encoding of one byte as value in the hexadecimal format", "Hexadecimal 8-bit"))
{}

Hexadecimal8Codec::~Hexadecimal8Codec() = default;

QVariant Hexadecimal8Codec::value(const PODData& data, int* byteCount) const
{
    const unsigned char* pointer = (unsigned char*)data.pointer(1);

    *byteCount = pointer ? 1 : 0;
    return pointer ? QVariant::fromValue<Hexadecimal8>(Hexadecimal8(*pointer)) : QVariant();
}

QByteArray Hexadecimal8Codec::valueToBytes(const QVariant& value) const
{
    const quint8 number = value.value<Hexadecimal8>().value;

    return QByteArray((const char*)&number, sizeof(quint8));
}

bool Hexadecimal8Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Hexadecimal8>().value == otherValue.value<Hexadecimal8>().value);
}

}
