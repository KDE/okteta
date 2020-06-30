/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uint64codec.hpp"

// tool
#include "../types/uint64.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

UInt64Codec::UInt64Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Unsigned 64-bit"))
{}

UInt64Codec::~UInt64Codec() = default;

QVariant UInt64Codec::value(const PODData& data, int* byteCount) const
{
    const quint64* pointer = (quint64*)data.pointer(8);

    *byteCount = pointer ? 8 : 0;
    return pointer ? QVariant::fromValue<UInt64>(UInt64(*pointer)) : QVariant();
}

QByteArray UInt64Codec::valueToBytes(const QVariant& value) const
{
    const quint64 number = value.value<UInt64>().value;

    return QByteArray((const char*)&number, sizeof(quint64));
}

bool UInt64Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<UInt64>().value == otherValue.value<UInt64>().value);
}

}
