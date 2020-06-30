/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "float64codec.hpp"

// tool
#include "../types/float64.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

Float64Codec::Float64Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Float 64-bit"))
{}

Float64Codec::~Float64Codec() = default;

QVariant Float64Codec::value(const PODData& data, int* byteCount) const
{
    const double* pointer = (double*)data.pointer(8);

    *byteCount = pointer ? 8 : 0;
    return pointer ? QVariant::fromValue<Float64>(Float64(*pointer)) : QVariant();
}

QByteArray Float64Codec::valueToBytes(const QVariant& value) const
{
    const double number = value.value<Float64>().value;

    return QByteArray((const char*)&number, sizeof(double));
}

bool Float64Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Float64>().value == otherValue.value<Float64>().value);
}

}
