/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraybzip2streamcompressor.hpp"

// lib
#include "bzip2compressionstream.hpp"
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>

namespace Kasten {

BZip2StreamCompressorSettings::BZip2StreamCompressorSettings() = default;

bool BZip2StreamCompressorSettings::operator==(const BZip2StreamCompressorSettings& other) const
{
    return
        (blockSize == other.blockSize);
}

void BZip2StreamCompressorSettings::loadConfig(const KConfigGroup& configGroup)
{
    blockSize = configGroup.readEntry(BlockSizeConfigKey, DefaultBlockSize);
    if ((blockSize < 1) || (9 < blockSize)) {
        blockSize = DefaultBlockSize;
    }
}

void BZip2StreamCompressorSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(BlockSizeConfigKey, blockSize);
}

ByteArrayBZip2StreamCompressor::ByteArrayBZip2StreamCompressor()
    : AbstractByteArrayStreamCompressor(i18nc("name of the encoding target", "bzip2"), QStringLiteral("application/x-bzip2"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.loadConfig(configGroup);
}

ByteArrayBZip2StreamCompressor::~ByteArrayBZip2StreamCompressor() = default;

void ByteArrayBZip2StreamCompressor::setSettings(const BZip2StreamCompressorSettings& settings)
{
    if (m_settings == settings) {
        return;
    }

    m_settings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

std::unique_ptr<AbstractCompressionStream> ByteArrayBZip2StreamCompressor::createCompressionStream(AbstractByteArrayOutput* byteArrayOutput)
{
    auto compressionStream = std::make_unique<BZip2CompressionStream>(byteArrayOutput, m_settings.blockSize);
    return compressionStream;
}

}

#include "moc_bytearraybzip2streamcompressor.cpp"
