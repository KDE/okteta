/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "sint16codec.hpp"

// tool
#include "../types/sint16.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

SInt16Codec::SInt16Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Signed 16-bit"))
{}

SInt16Codec::~SInt16Codec() = default;

QVariant SInt16Codec::value(const PODData& data, int* byteCount) const
{
    const qint16* pointer = (qint16*)data.pointer(2);

    *byteCount = pointer ? 2 : 0;
    return pointer ? QVariant::fromValue<SInt16>(SInt16(*pointer)) : QVariant();
}

QByteArray SInt16Codec::valueToBytes(const QVariant& value) const
{
    const qint16 number = value.value<SInt16>().value;

    return QByteArray((const char*)&number, sizeof(qint16));
}

bool SInt16Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<SInt16>().value == otherValue.value<SInt16>().value);
}

}
