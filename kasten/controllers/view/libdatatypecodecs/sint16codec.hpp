/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_SINT16CODEC_HPP
#define OKTETA_SINT16CODEC_HPP

// lib
#include "abstracttypecodec.hpp"

namespace Okteta {

class SInt16Codec : public AbstractTypeCodec
{
public:
    SInt16Codec();

    ~SInt16Codec() override;

public: // AbstractTypeCodec API
    [[nodiscard]]
    QVariant value(const PODData& data, int* byteCount) const override;
    [[nodiscard]]
    QByteArray valueToBytes(const QVariant& value) const override;

    [[nodiscard]]
    bool areEqual(const QVariant& value, QVariant& otherValue) const override;
};

}

#endif
