/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBASE64STREAMENCODER_HPP
#define KASTEN_BYTEARRAYBASE64STREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

namespace Kasten {
extern const char base64EncodeMap[64];

class ByteArrayBase64StreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    static constexpr int inputGroupLength = 3;

    static constexpr int outputLineLength = 76;
    static constexpr int outputGroupLength = 4;
    static constexpr int maxOutputGroupsPerLine = outputLineLength / outputGroupLength;

    enum class InputByteIndex
    {
        First = 0,
        Second,
        Third
    };

public:
    ByteArrayBase64StreamEncoder();
    ~ByteArrayBase64StreamEncoder() override;

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;
};

}

#endif
