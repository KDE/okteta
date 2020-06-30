/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uint32codec.hpp"

// tool
#include "../types/uint32.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

UInt32Codec::UInt32Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Unsigned 32-bit"))
{}

UInt32Codec::~UInt32Codec() = default;

QVariant UInt32Codec::value(const PODData& data, int* byteCount) const
{
    const quint32* pointer = (quint32*)data.pointer(4);

    *byteCount = pointer ? 4 : 0;
    return pointer ? QVariant::fromValue<UInt32>(UInt32(*pointer)) : QVariant();
}

QByteArray UInt32Codec::valueToBytes(const QVariant& value) const
{
    const quint32 number = value.value<UInt32>().value;

    return QByteArray((const char*)&number, sizeof(quint32));
}

bool UInt32Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<UInt32>().value == otherValue.value<UInt32>().value);
}

}
