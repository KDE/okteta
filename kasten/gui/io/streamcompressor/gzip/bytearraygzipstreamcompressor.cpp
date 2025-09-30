/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraygzipstreamcompressor.hpp"

// lib
#include "gzipcompressionstream.hpp"
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Std
#include <algorithm>
#include <iterator>

const std::array<QString, Kasten::GZipStreamCompressorSettings::StrategyCount>
Kasten::GZipStreamCompressorSettings::strategyConfigValueList = {
    QStringLiteral("Default"),
    QStringLiteral("Filter"),
    QStringLiteral("HuffmanEncodingOnly"),
    QStringLiteral("RunLengthEncoding"),
    QStringLiteral("FixedHuffmanEncoding"),
};

template <>
inline Kasten::GZipStreamCompressorSettings::StrategyId KConfigGroup::readEntry(const char *key,
                                                                                const Kasten::GZipStreamCompressorSettings::StrategyId &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    const auto it = std::find(Kasten::GZipStreamCompressorSettings::strategyConfigValueList.cbegin(),
                              Kasten::GZipStreamCompressorSettings::strategyConfigValueList.cend(),
                              entry);
    if (it == Kasten::GZipStreamCompressorSettings::strategyConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(Kasten::GZipStreamCompressorSettings::strategyConfigValueList.cbegin(), it);
    return static_cast<Kasten::GZipStreamCompressorSettings::StrategyId>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::GZipStreamCompressorSettings::StrategyId &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, Kasten::GZipStreamCompressorSettings::strategyConfigValueList[listIndex], flags);
}

namespace Kasten {

const QString GZipStreamCompressorSettings::DefaultFileName = QString();
const QString GZipStreamCompressorSettings::DefaultComment = QString();

GZipStreamCompressorSettings::GZipStreamCompressorSettings() = default;

bool GZipStreamCompressorSettings::operator==(const GZipStreamCompressorSettings& other) const
{
    return
        (compressionLevel == other.compressionLevel) &&
        (strategyId == other.strategyId) &&
        (fileName == other.fileName) &&
        (comment == other.comment);
}

void GZipStreamCompressorSettings::loadConfig(const KConfigGroup& configGroup)
{
    compressionLevel = configGroup.readEntry(CompressionLevelConfigKey, DefaultCompressionLevel);
    if ((compressionLevel < -1) || (9 < compressionLevel)) {
        compressionLevel = DefaultCompressionLevel;
    }
    strategyId = configGroup.readEntry(StrategyConfigKey, DefaultStrategy);
    fileName = configGroup.readEntry(FileNameConfigKey, DefaultFileName);
    comment = configGroup.readEntry(CommentConfigKey, DefaultComment);
}

void GZipStreamCompressorSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(CompressionLevelConfigKey, compressionLevel);
    configGroup.writeEntry(StrategyConfigKey, strategyId);
    configGroup.writeEntry(FileNameConfigKey, fileName);
    configGroup.writeEntry(CommentConfigKey, comment);
}

ByteArrayGZipStreamCompressor::ByteArrayGZipStreamCompressor()
    : AbstractByteArrayStreamCompressor(i18nc("name of the encoding target", "gzip"), QStringLiteral("application/gzip"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.loadConfig(configGroup);
}

ByteArrayGZipStreamCompressor::~ByteArrayGZipStreamCompressor() = default;

void ByteArrayGZipStreamCompressor::setSettings(const GZipStreamCompressorSettings& settings)
{
    if (m_settings == settings) {
        return;
    }

    m_settings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

int zlibStrategy(GZipStreamCompressorSettings::StrategyId strategyId)
{
    int listIndex = static_cast<int>(strategyId);
    if ((listIndex < 0) || (listIndex > GZipStreamCompressorSettings::StrategyCount)) {
        listIndex = 0;
    }
    static const std::array<int, GZipStreamCompressorSettings::StrategyCount> zLibByIdTable = {
        Z_DEFAULT_STRATEGY,
        Z_FILTERED,
        Z_HUFFMAN_ONLY,
        Z_RLE,
        Z_FIXED,
    };
    return zLibByIdTable[listIndex];
}

std::unique_ptr<AbstractCompressionStream> ByteArrayGZipStreamCompressor::createCompressionStream(AbstractByteArrayOutput* byteArrayOutput)
{
    auto compressionStream = std::make_unique<GZipCompressionStream>(byteArrayOutput, m_settings.compressionLevel, zlibStrategy(m_settings.strategyId));
    compressionStream->setFileName(m_settings.fileName.toStdString());
    compressionStream->setComment(m_settings.comment.toStdString());
    return compressionStream;
}

}

#include "moc_bytearraygzipstreamcompressor.cpp"
