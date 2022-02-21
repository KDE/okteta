/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraybase32streamencoder.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QTextStream>
// Std
#include <array>
#include <algorithm>
#include <iterator>

static constexpr int encodingTypeCount =
    static_cast<int>(Kasten::Base32StreamEncoderSettings::EncodingType::_Count);
static const std::array<QString, encodingTypeCount> encodingTypeConfigValueList = {
    QStringLiteral("Classic"),
    QStringLiteral("base32hex"),
    QStringLiteral("z-base-32"),
};

template <>
inline Kasten::Base32StreamEncoderSettings::EncodingType
KConfigGroup::readEntry(const char *key,
                        const Kasten::Base32StreamEncoderSettings::EncodingType &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(encodingTypeConfigValueList.cbegin(), encodingTypeConfigValueList.cend(), entry);
    if (it == encodingTypeConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(encodingTypeConfigValueList.cbegin(), it);
    return static_cast<Kasten::Base32StreamEncoderSettings::EncodingType>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::Base32StreamEncoderSettings::EncodingType &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, encodingTypeConfigValueList[listIndex], flags);
}

namespace Kasten {

static constexpr Base32StreamEncoderSettings::EncodingType DefaultEncodingType =
    Base32StreamEncoderSettings::EncodingType::Classic;

static constexpr char ByteArrayBase32StreamEncoderConfigGroupId[] = "ByteArrayBase32StreamEncoder";
static constexpr char EncodingTypeConfigKey[] = "EncodingType";

static constexpr char base32ClassicEncodeMap[32] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '2', '3', '4', '5', '6', '7'
};
static constexpr char base32HexEncodeMap[32] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'
};
static constexpr char base32ZHexEncodeMap[32] =
{
    'y', 'b', 'n', 'd', 'r', 'f', 'g', '8',
    'e', 'j', 'k', 'm', 'c', 'p', 'q', 'x',
    'o', 't', '1', 'u', 'w', 'i', 's', 'z',
    'a', '3', '4', '5', 'h', '7', '6', '9'
};

static constexpr const char* base32PaddingData[4] =
{
    "======",
    "====",
    "===",
    "="
};

static inline constexpr const char* base32Padding(ByteArrayBase32StreamEncoder::InputByteIndex index)
{
    return base32PaddingData[static_cast<int>(index) - 1];
}
static inline constexpr const char* noPadding(ByteArrayBase32StreamEncoder::InputByteIndex /*index*/)
{
    return "";
}

struct Base32EncodingData
{
    const char* const encodeMap;
    const char* (* padding)(ByteArrayBase32StreamEncoder::InputByteIndex);
};

static constexpr Base32EncodingData
    base32EncodingData[3] =
{
    {base32ClassicEncodeMap, &base32Padding},
    {base32HexEncodeMap, &base32Padding},
    {base32ZHexEncodeMap, &noPadding}
};

Base32StreamEncoderSettings::Base32StreamEncoderSettings() = default;

bool Base32StreamEncoderSettings::operator==(const Base32StreamEncoderSettings& other) const
{
    return (encodingType == other.encodingType);
}

void Base32StreamEncoderSettings::loadConfig(const KConfigGroup& configGroup)
{
    encodingType = configGroup.readEntry(EncodingTypeConfigKey, DefaultEncodingType);
}

void Base32StreamEncoderSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(EncodingTypeConfigKey, encodingType);
}


ByteArrayBase32StreamEncoder::ByteArrayBase32StreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Base32"), QStringLiteral("text/x-base32"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ByteArrayBase32StreamEncoderConfigGroupId);
    mSettings.loadConfig(configGroup);
}

ByteArrayBase32StreamEncoder::~ByteArrayBase32StreamEncoder() = default;


void ByteArrayBase32StreamEncoder::setSettings(const Base32StreamEncoderSettings& settings)
{
    if (mSettings == settings) {
        return;
    }

    mSettings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ByteArrayBase32StreamEncoderConfigGroupId);
    mSettings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

bool ByteArrayBase32StreamEncoder::encodeDataToStream(QIODevice* device,
                                                      const ByteArrayView* byteArrayView,
                                                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                      const Okteta::AddressRange& range)
{
    Q_UNUSED(byteArrayView);

    bool success = true;

    // encode
    QTextStream textStream(device);

    // prepare
    const auto& encodingTypeData = base32EncodingData[static_cast<int>(mSettings.encodingType)];
    const char* const base32EncodeMap = encodingTypeData.encodeMap;
    const char* (* base32Padding)(InputByteIndex) = encodingTypeData.padding;

    InputByteIndex inputByteIndex = InputByteIndex::First;
    int outputGroupsPerLine = 0;
    unsigned char bitsFromLastByte;

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        const Okteta::Byte byte = byteArrayModel->byte(i);

        switch (inputByteIndex)
        {
        case InputByteIndex::First:
            // bits 7..3
            textStream << base32EncodeMap[(byte >> 3)];
            // bits 2..0 -> 4..2 for next
            bitsFromLastByte = (byte & 0x7) << 2;
            inputByteIndex = InputByteIndex::Second;
            break;
        case InputByteIndex::Second:
            // from last and bits 7..6 as 1..0 from this
            textStream << base32EncodeMap[(bitsFromLastByte | byte >> 6)];
            // bits 5..1 as 4..0
            textStream << base32EncodeMap[(byte & 0x3E) >> 1];
            // bits 0 -> 4 for next
            bitsFromLastByte = (byte & 0x1) << 4;
            inputByteIndex = InputByteIndex::Third;
            break;
        case InputByteIndex::Third:
            // from last and bits 7..4 as 3..0 from this
            textStream << base32EncodeMap[(bitsFromLastByte | byte >> 4)];
            // bits 3..0 -> 4..1 for next
            bitsFromLastByte = (byte & 0xF) << 1;
            inputByteIndex = InputByteIndex::Fourth;
            break;
        case InputByteIndex::Fourth:
            // from last and bit 7 as 0 from this
            textStream << base32EncodeMap[(bitsFromLastByte | byte >> 7)];
            // bits 6..2 as 4..0
            textStream << base32EncodeMap[(byte & 0x7C) >> 2];
            // bits 1..0 -> 4..3 for next
            bitsFromLastByte = (byte & 0x3) << 3;
            inputByteIndex = InputByteIndex::Fifth;
            break;
        case InputByteIndex::Fifth:
            // from last and bits 7..5 as 2..0 from this
            textStream << base32EncodeMap[(bitsFromLastByte | byte >> 5)];
            // bits 4..0
            textStream << base32EncodeMap[(byte & 0x1F)];
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
        textStream << base32EncodeMap[bitsFromLastByte]
                   << base32Padding(inputByteIndex);
    }

    return success;
}

}
