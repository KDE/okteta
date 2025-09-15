/*
    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UTF16CODEC_HPP
#define KASTEN_UTF16CODEC_HPP

// tool
#include "abstracttypecodec.hpp"
// Std
#include <memory>

class QTextEncoder;
class QTextDecoder;

namespace Okteta {

class Utf16Codec : public AbstractTypeCodec
{
public:
    Utf16Codec();

    ~Utf16Codec() override;

public: // AbstractTypeCodec API
    [[nodiscard]]
    QVariant value(const PODData& data, int* byteCount) const override;
    [[nodiscard]]
    QByteArray valueToBytes(const QVariant& value) const override;

    [[nodiscard]]
    bool areEqual(const QVariant& value, QVariant& otherValue) const override;

private:
    std::unique_ptr<QTextEncoder> mUtf16Encoder;
    std::unique_ptr<QTextDecoder> mUtf16Decoder;
};

}

#endif
