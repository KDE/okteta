/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayxxencodingstreamencoder.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QTextStream>
// Std
#include <algorithm>
#include <array>

namespace Kasten {

const QString XxencodingStreamEncoderSettings::DefaultFileName;

static constexpr std::array<char, 64> xxencodeMap = {
    '+', '-', '0', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
    'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};
static constexpr std::array<const char*, 2> paddingData = {
    "++",
    "+",
};

static constexpr char xxmapByte(char byte)
{
    const auto index = static_cast<std::size_t>(byte);
    return xxencodeMap[index];
}

static constexpr const char* xxpadding(ByteArrayXxencodingStreamEncoder::InputByteIndex index)
{
    const auto dataIndex = static_cast<std::size_t>(index) - 1;
    return paddingData[dataIndex];
}

XxencodingStreamEncoderSettings::XxencodingStreamEncoderSettings() = default;

bool XxencodingStreamEncoderSettings::operator==(const XxencodingStreamEncoderSettings& other) const
{
    return (fileName == other.fileName);
}

void XxencodingStreamEncoderSettings::loadConfig(const KConfigGroup& configGroup)
{
    fileName = configGroup.readEntry(FileNameConfigKey, DefaultFileName);
}

void XxencodingStreamEncoderSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(FileNameConfigKey, fileName);
}

const QString ByteArrayXxencodingStreamEncoder::ConfigGroupId = QStringLiteral("ByteArrayXxencodingStreamEncoder");

ByteArrayXxencodingStreamEncoder::ByteArrayXxencodingStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Xxencoding"), QStringLiteral("text/x-xxencode"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mSettings.loadConfig(configGroup);
}

ByteArrayXxencodingStreamEncoder::~ByteArrayXxencodingStreamEncoder() = default;

void ByteArrayXxencodingStreamEncoder::setSettings(const XxencodingStreamEncoderSettings& settings)
{
    if (mSettings == settings) {
        return;
    }

    mSettings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mSettings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

// TODO: make this algorithm shared with ByteArrayUuencodingStreamEncoder again
bool ByteArrayXxencodingStreamEncoder::encodeDataToStream(QIODevice* device,
                                                          const ByteArrayView* byteArrayView,
                                                          const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                          const Okteta::AddressRange& range)
{
    Q_UNUSED(byteArrayView)

    const char header[] = "begin";
    const char footer[] = "\n+\nend\n";

    bool success = true;

    // encode
    QTextStream textStream(device);

    // prepare
    InputByteIndex inputByteIndex = InputByteIndex::First;
    int inputGroupsPerLine = 0;
    unsigned char bitsFromLastByte;

    // header
    textStream << header << " 644 " << mSettings.fileName.toLatin1();

    const int firstLineLength = std::min(range.width(), inputLineLength);
    if (firstLineLength > 0) {
        textStream << '\n';
        textStream << xxmapByte(firstLineLength);
    }

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        const Okteta::Byte byte = byteArrayModel->byte(i);

        switch (inputByteIndex)
        {
        case InputByteIndex::First:
            // bits 7..2
            textStream << xxmapByte(byte >> 2);
            // bits 1..0 -> 5..4 for next
            bitsFromLastByte = (byte & 0x3) << 4;
            inputByteIndex = InputByteIndex::Second;
            break;
        case InputByteIndex::Second:
            // from last and bits 7..4 as 3..0 from this
            textStream << xxmapByte(bitsFromLastByte | byte >> 4);
            // bits 3..0 -> 5..2 for next
            bitsFromLastByte = (byte & 0xf) << 2;
            inputByteIndex = InputByteIndex::Third;
            break;
        case InputByteIndex::Third:
            // from last and bits 7..6 as 1..0 from this
            textStream << xxmapByte(bitsFromLastByte | byte >> 6);
            // bits 5..0
            textStream << xxmapByte(byte & 0x3F);
            inputByteIndex = InputByteIndex::First;
            ++inputGroupsPerLine;
            if (inputGroupsPerLine >= maxInputGroupsPerLine && i < range.end()) {
                const int remainsCount = range.end() - i;
                const int nextLineLength = std::min(remainsCount, inputLineLength);
                textStream << '\n';
                textStream << xxmapByte(nextLineLength);
                inputGroupsPerLine = 0;
            }
            break;
        }
    }

    const bool hasBitsLeft = (inputByteIndex != InputByteIndex::First);
    if (hasBitsLeft) {
        textStream << xxmapByte(bitsFromLastByte)
                   << xxpadding(inputByteIndex);
    }
    // footer
    textStream << footer;

    return success;
}

}

#include "moc_bytearrayxxencodingstreamencoder.cpp"
