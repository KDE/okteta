/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "sint32codec.hpp"

// tool
#include "../types/sint32.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>

namespace Okteta {

SInt32Codec::SInt32Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "Signed 32-bit"))
{}

SInt32Codec::~SInt32Codec() = default;

QVariant SInt32Codec::value(const PODData& data, int* byteCount) const
{
    const qint32* pointer = (qint32*)data.pointer(4);

    *byteCount = pointer ? 4 : 0;
    return pointer ? QVariant::fromValue<SInt32>(SInt32(*pointer)) : QVariant();
}

QByteArray SInt32Codec::valueToBytes(const QVariant& value) const
{
    const qint32 number = value.value<SInt32>().value;

    return QByteArray((const char*)&number, sizeof(qint32));
}

bool SInt32Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<SInt32>().value == otherValue.value<SInt32>().value);
}

}
