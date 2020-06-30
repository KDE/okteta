/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "sint8codec.hpp"

// tool
#include "../types/sint8.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

SInt8Codec::SInt8Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Signed 8-bit"))
{}

SInt8Codec::~SInt8Codec() = default;

QVariant SInt8Codec::value(const PODData& data, int* byteCount) const
{
    const qint8* pointer = (qint8*)data.pointer(1);

    *byteCount = pointer ? 1 : 0;
    return pointer ? QVariant::fromValue<SInt8>(SInt8(*pointer)) : QVariant();
}

QByteArray SInt8Codec::valueToBytes(const QVariant& value) const
{
    const qint8 number = value.value<SInt8>().value;

    return QByteArray((const char*)&number, sizeof(qint8));
}

bool SInt8Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<SInt8>().value == otherValue.value<SInt8>().value);
}

}
