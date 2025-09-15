/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayzstdstreamcompressor.hpp"

// lib
#include "zstdcompressionstream.hpp"
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>

namespace Kasten {

ZStdStreamCompressorSettings::ZStdStreamCompressorSettings() = default;

bool ZStdStreamCompressorSettings::operator==(const ZStdStreamCompressorSettings& other) const
{
    return
        (compressionLevel == other.compressionLevel) &&
        (writeFrameContentSize == other.writeFrameContentSize) &&
        (writeFrameChecksum == other.writeFrameChecksum);
}

void ZStdStreamCompressorSettings::loadConfig(const KConfigGroup& configGroup)
{
    compressionLevel = configGroup.readEntry(CompressionLevelConfigKey, DefaultCompressionLevel);
    if ((compressionLevel < 0) || (9 < compressionLevel)) {
        compressionLevel = DefaultCompressionLevel;
    }
    writeFrameContentSize = configGroup.readEntry(WriteFrameContentSizeConfigKey, DefaultWriteFrameContentSize);
    writeFrameChecksum = configGroup.readEntry(WriteFrameChecksumConfigKey, DefaultWriteFrameChecksum);
}

void ZStdStreamCompressorSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(CompressionLevelConfigKey, compressionLevel);
    configGroup.writeEntry(WriteFrameContentSizeConfigKey, writeFrameContentSize);
    configGroup.writeEntry(WriteFrameChecksumConfigKey, writeFrameChecksum);
}

ByteArrayZStdStreamCompressor::ByteArrayZStdStreamCompressor()
    : AbstractByteArrayStreamCompressor(i18nc("name of the encoding target", "ZStandard"), QStringLiteral("application/zstd"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.loadConfig(configGroup);
}

ByteArrayZStdStreamCompressor::~ByteArrayZStdStreamCompressor() = default;

void ByteArrayZStdStreamCompressor::setSettings(const ZStdStreamCompressorSettings& settings)
{
    if (m_settings == settings) {
        return;
    }

    m_settings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

std::unique_ptr<AbstractCompressionStream> ByteArrayZStdStreamCompressor::createCompressionStream(AbstractByteArrayOutput* byteArrayOutput)
{
    ZStdCompressionStream::FrameFlags frameFlags;
    // TODO: find a nicer way to map separate bools to a flag set
    if (m_settings.writeFrameContentSize) {
        frameFlags.setFlag(ZStdCompressionStream::WriteFrameContentSize);
    }
    if (m_settings.writeFrameChecksum) {
        frameFlags.setFlag(ZStdCompressionStream::WriteFrameChecksum);
    }
    return std::make_unique<ZStdCompressionStream>(byteArrayOutput, m_settings.compressionLevel, frameFlags);
}

}

#include "moc_bytearrayzstdstreamcompressor.cpp"
