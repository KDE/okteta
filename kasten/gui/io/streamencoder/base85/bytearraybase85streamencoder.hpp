/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBASE85STREAMENCODER_HPP
#define KASTEN_BYTEARRAYBASE85STREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"

namespace Kasten {

class ByteArrayBase85StreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    static constexpr int outputLineLength = 72;
    static constexpr int maxOutputBytesPerLine = outputLineLength;

    enum class InputByteIndex
    {
        First = 0,
        Second,
        Third,
        Fourth
    };

public:
    ByteArrayBase85StreamEncoder();
    ~ByteArrayBase85StreamEncoder() override;

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;
};

}

#endif
