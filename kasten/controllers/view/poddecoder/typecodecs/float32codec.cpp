/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "float32codec.hpp"

// tool
#include "../types/float32.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

Float32Codec::Float32Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Float 32-bit"))
{}

Float32Codec::~Float32Codec() = default;

QVariant Float32Codec::value(const PODData& data, int* byteCount) const
{
    const float* pointer = (float*)data.pointer(4);

    *byteCount = pointer ? 4 : 0;
    return pointer ? QVariant::fromValue<Float32>(Float32(*pointer)) : QVariant();
}

QByteArray Float32Codec::valueToBytes(const QVariant& value) const
{
    const float number = value.value<Float32>().value;

    return QByteArray((const char*)&number, sizeof(float));
}

bool Float32Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Float32>().value == otherValue.value<Float32>().value);
}

}
