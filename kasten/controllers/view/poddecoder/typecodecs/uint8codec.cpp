/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uint8codec.hpp"

// tool
#include "../types/uint8.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

UInt8Codec::UInt8Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Unsigned 8-bit"))
{}

UInt8Codec::~UInt8Codec() = default;

QVariant UInt8Codec::value(const PODData& data, int* byteCount) const
{
    const quint8* pointer = (quint8*)data.pointer(1);

    *byteCount = pointer ? 1 : 0;
    return pointer ? QVariant::fromValue<UInt8>(UInt8(*pointer)) : QVariant();
}

QByteArray UInt8Codec::valueToBytes(const QVariant& value) const
{
    const quint8 number = value.value<UInt8>().value;

    return QByteArray((const char*)&number, sizeof(quint8));
}

bool UInt8Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<UInt8>().value == otherValue.value<UInt8>().value);
}

}
