/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayuuencodingstreamencoder.hpp"

// lib
#include "../base64/bytearraybase64streamencoder.hpp"
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>

namespace Kasten {

static constexpr int defaultUuInputLineLength = 45;
static constexpr int uuInputLineLength = defaultUuInputLineLength;
static constexpr int uuInputGroupLength = 3;
static constexpr int maxInputGroupsPerLine = uuInputLineLength / uuInputGroupLength;

static inline constexpr char uumapByteHistorical(char byte) { return (byte > 0) ? (byte + 32) : '`'; }
static inline char uumapByteBase64(char byte)     { return base64EncodeMap[(int)byte]; }

struct UumapEncodeData
{
    char (* mapByte)(char);
    const char* header;
    const char* footer;
    const char* paddingData[2];
    bool hasLength;

    inline const char* padding(ByteArrayUuencodingStreamEncoder::InputByteIndex index) const
    {
        return paddingData[static_cast<int>(index) - 1];
    }
};

static constexpr UumapEncodeData historicalUumapEncodeData =
{
    &uumapByteHistorical,
    "begin",
    "\n`\nend\n",
    {"``", "`"},
    true
};

static constexpr UumapEncodeData base64UumapEncodeData =
{
    &uumapByteBase64,
    "begin-base64",
    "\n====\n",
    {"==", "="},
    false
};

UuencodingStreamEncoderSettings::UuencodingStreamEncoderSettings()
    : fileName(QStringLiteral("okteta-export"))
{}

ByteArrayUuencodingStreamEncoder::ByteArrayUuencodingStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Uuencoding"), QStringLiteral("text/x-uuencode"))
{}

ByteArrayUuencodingStreamEncoder::~ByteArrayUuencodingStreamEncoder() = default;

bool ByteArrayUuencodingStreamEncoder::encodeDataToStream(QIODevice* device,
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
    int inputGroupsPerLine = 0;
    unsigned char bitsFromLastByte;

    const UumapEncodeData* encodeData =
        (mSettings.algorithmId == UuencodingStreamEncoderSettings::AlgorithmId::Historical) ?
            &historicalUumapEncodeData :
        /* else */
            &base64UumapEncodeData;

    // header
    textStream << encodeData->header << " 644 " << mSettings.fileName.toLatin1();

    const int firstLineLength = qMin(range.width(), uuInputLineLength);
    if (firstLineLength > 0) {
        textStream << '\n';
        if (encodeData->hasLength) {
            textStream << encodeData->mapByte(firstLineLength);
        }
    }

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        const Okteta::Byte byte = byteArrayModel->byte(i);

        switch (inputByteIndex)
        {
        case InputByteIndex::First:
            // bits 7..2
            textStream << encodeData->mapByte(byte >> 2);
            // bits 1..0 -> 5..4 for next
            bitsFromLastByte = (byte & 0x3) << 4;
            inputByteIndex = InputByteIndex::Second;
            break;
        case InputByteIndex::Second:
            // from last and bits 7..4 as 3..0 from this
            textStream << encodeData->mapByte(bitsFromLastByte | byte >> 4);
            // bits 3..0 -> 5..2 for next
            bitsFromLastByte = (byte & 0xf) << 2;
            inputByteIndex = InputByteIndex::Third;
            break;
        case InputByteIndex::Third:
            // from last and bits 7..6 as 1..0 from this
            textStream << encodeData->mapByte(bitsFromLastByte | byte >> 6);
            // bits 5..0
            textStream << encodeData->mapByte(byte & 0x3F);
            inputByteIndex = InputByteIndex::First;
            ++inputGroupsPerLine;
            if (inputGroupsPerLine >= maxInputGroupsPerLine && i < range.end()) {
                const int remainsCount = range.end() - i;
                const int nextLineLength = qMin(remainsCount, uuInputLineLength);
                textStream << '\n';
                if (encodeData->hasLength) {
                    textStream << encodeData->mapByte(nextLineLength);
                }
                inputGroupsPerLine = 0;
            }
            break;
        }
    }

    const bool hasBitsLeft = (inputByteIndex != InputByteIndex::First);
    if (hasBitsLeft) {
        textStream << encodeData->mapByte(bitsFromLastByte)
                   << encodeData->padding(inputByteIndex);
    }
    // footer
    textStream << encodeData->footer;

    return success;
}

}
