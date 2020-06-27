/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "sint64codec.hpp"

// tool
#include "../types/sint64.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

SInt64Codec::SInt64Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Signed 64-bit"))
{}

SInt64Codec::~SInt64Codec() = default;

QVariant SInt64Codec::value(const PODData& data, int* byteCount) const
{
    const qint64* pointer = (qint64*)data.pointer(8);

    *byteCount = pointer ? 8 : 0;
    return pointer ? QVariant::fromValue<SInt64>(SInt64(*pointer)) : QVariant();
}

QByteArray SInt64Codec::valueToBytes(const QVariant& value) const
{
    const qint64 number = value.value<SInt64>().value;

    return QByteArray((const char*)&number, sizeof(qint64));
}

bool SInt64Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<SInt64>().value == otherValue.value<SInt64>().value);
}

}
