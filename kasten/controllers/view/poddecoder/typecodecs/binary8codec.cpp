/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "binary8codec.hpp"

// tool
#include "../types/binary8.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

Binary8Codec::Binary8Codec()
    : AbstractTypeCodec(i18nc("@label:textbox encoding of one byte as value in the binary format", "Binary 8-bit"))
{}

Binary8Codec::~Binary8Codec() = default;

QVariant Binary8Codec::value(const PODData& data, int* byteCount) const
{
    const unsigned char* pointer = (unsigned char*)data.pointer(1);

    *byteCount = pointer ? 1 : 0;
    return pointer ? QVariant::fromValue<Binary8>(Binary8(*pointer)) : QVariant();
}

QByteArray Binary8Codec::valueToBytes(const QVariant& value) const
{
    const quint8 number = value.value<Binary8>().value;

    return QByteArray((const char*)&number, sizeof(quint8));
}

bool Binary8Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Binary8>().value == otherValue.value<Binary8>().value);
}

}
