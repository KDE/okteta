/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UINT8CODEC_HPP
#define KASTEN_UINT8CODEC_HPP

// tool
#include "../abstracttypecodec.hpp"

namespace Okteta {

class UInt8Codec : public AbstractTypeCodec
{
public:
    UInt8Codec();

    ~UInt8Codec() override;

public: // AbstractTypeCodec API
    QVariant value(const PODData& data, int* byteCount) const override;
    QByteArray valueToBytes(const QVariant& value) const override;

    bool areEqual(const QVariant& value, QVariant& otherValue) const override;
};

}

#endif
