/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uint16codec.hpp"

// tool
#include "../types/uint16.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

UInt16Codec::UInt16Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Unsigned 16-bit"))
{}

UInt16Codec::~UInt16Codec() = default;

QVariant UInt16Codec::value(const PODData& data, int* byteCount) const
{
    const quint16* pointer = (quint16*)data.pointer(2);

    *byteCount = pointer ? 2 : 0;
    return pointer ? QVariant::fromValue<UInt16>(UInt16(*pointer)) : QVariant();
}

QByteArray UInt16Codec::valueToBytes(const QVariant& value) const
{
    const quint16 number = value.value<UInt16>().value;

    return QByteArray((const char*)&number, sizeof(quint16));
}

bool UInt16Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<UInt16>().value == otherValue.value<UInt16>().value);
}

}
