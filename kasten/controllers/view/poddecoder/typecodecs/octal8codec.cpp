/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "octal8codec.hpp"

// tool
#include "../types/octal8.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

Octal8Codec::Octal8Codec()
    : AbstractTypeCodec(i18nc("@label:textbox encoding of one byte as value in the octal format", "Octal 8-bit"))
{}

Octal8Codec::~Octal8Codec() = default;

QVariant Octal8Codec::value(const PODData& data, int* byteCount) const
{
    const unsigned char* pointer = (unsigned char*)data.pointer(1);

    *byteCount = pointer ? 1 : 0;
    return pointer ? QVariant::fromValue<Octal8>(Octal8(*pointer)) : QVariant();
}

QByteArray Octal8Codec::valueToBytes(const QVariant& value) const
{
    const quint8 number = value.value<Octal8>().value;

    return QByteArray((const char*)&number, sizeof(quint8));
}

bool Octal8Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Octal8>().value == otherValue.value<Octal8>().value);
}

}
