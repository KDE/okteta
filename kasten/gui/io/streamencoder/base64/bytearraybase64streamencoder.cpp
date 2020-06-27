/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraybase64streamencoder.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>

namespace Kasten {

const char base64EncodeMap[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

static constexpr const char* base64PaddingData[2] = {
    "==",
    "="
};
static inline constexpr const char* base64Padding(ByteArrayBase64StreamEncoder::InputByteIndex index)
{
    return base64PaddingData[static_cast<int>(index) - 1];
}

ByteArrayBase64StreamEncoder::ByteArrayBase64StreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Base64"), QStringLiteral("application/base64"))
{}

ByteArrayBase64StreamEncoder::~ByteArrayBase64StreamEncoder() = default;

bool ByteArrayBase64StreamEncoder::encodeDataToStream(QIODevice* device,
                                                      const ByteArrayView* byteArrayView,
                                                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                      const Okteta::AddressRange& range)
{
    Q_UNUSED(byteArrayView);

    bool success = true;

    // encode
    QTextStream textStream(device);

    // prepare
    InputByteIndex inputByteIndex = InputByteIndex::First;
    int outputGroupsPerLine = 0;
    unsigned char bitsFromLastByte;

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        const Okteta::Byte byte = byteArrayModel->byte(i);

        switch (inputByteIndex)
        {
        case InputByteIndex::First:
            // bits 7..2
            textStream << base64EncodeMap[(byte >> 2)];
            // bits 1..0 -> 5..4 for next
            bitsFromLastByte = (byte & 0x3) << 4;
            inputByteIndex = InputByteIndex::Second;
            break;
        case InputByteIndex::Second:
            // from last and bits 7..4 as 3..0 from this
            textStream << base64EncodeMap[(bitsFromLastByte | byte >> 4)];
            // bits 3..0 -> 5..2 for next
            bitsFromLastByte = (byte & 0xf) << 2;
            inputByteIndex = InputByteIndex::Third;
            break;
        case InputByteIndex::Third:
            // from last and bits 7..6 as 1..0 from this
            textStream << base64EncodeMap[(bitsFromLastByte | byte >> 6)];
            // bits 5..0
            textStream << base64EncodeMap[(byte & 0x3F)];
            inputByteIndex = InputByteIndex::First;
            ++outputGroupsPerLine;
            if (outputGroupsPerLine >= maxOutputGroupsPerLine && i < range.end()) {
                textStream << "\r\n";
                outputGroupsPerLine = 0;
            }
            break;
        }
    }

    const bool hasBitsLeft = (inputByteIndex != InputByteIndex::First);
    if (hasBitsLeft) {
        textStream << base64EncodeMap[bitsFromLastByte]
                   << base64Padding(inputByteIndex);
    }

    return success;
}

}
