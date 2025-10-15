/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydeflatestreamcompressor.hpp"

// lib
#include "deflatecompressionstream.hpp"
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Std
#include <algorithm>
#include <iterator>

const std::array<QString, Kasten::DeflateStreamCompressorSettings::StrategyCount>
Kasten::DeflateStreamCompressorSettings::strategyConfigValueList = {
    QStringLiteral("Default"),
    QStringLiteral("Filter"),
    QStringLiteral("HuffmanEncodingOnly"),
    QStringLiteral("RunLengthEncoding"),
    QStringLiteral("FixedHuffmanEncoding"),
};

template <>
inline Kasten::DeflateStreamCompressorSettings::StrategyId KConfigGroup::readEntry(const char *key,
                                                                                   const Kasten::DeflateStreamCompressorSettings::StrategyId &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    const auto it = std::find(Kasten::DeflateStreamCompressorSettings::strategyConfigValueList.cbegin(),
                              Kasten::DeflateStreamCompressorSettings::strategyConfigValueList.cend(),
                              entry);
    if (it == Kasten::DeflateStreamCompressorSettings::strategyConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(Kasten::DeflateStreamCompressorSettings::strategyConfigValueList.cbegin(), it);
    return static_cast<Kasten::DeflateStreamCompressorSettings::StrategyId>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::DeflateStreamCompressorSettings::StrategyId &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, Kasten::DeflateStreamCompressorSettings::strategyConfigValueList[listIndex], flags);
}

const std::array<QString, Kasten::DeflateStreamCompressorSettings::FormatCount>
Kasten::DeflateStreamCompressorSettings::formatConfigValueList = {
    QStringLiteral("Raw"),
    QStringLiteral("Full"),
};

template <>
inline Kasten::DeflateStreamCompressorSettings::FormatId KConfigGroup::readEntry(const char *key,
                                                                                 const Kasten::DeflateStreamCompressorSettings::FormatId &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    const auto it = std::find(Kasten::DeflateStreamCompressorSettings::formatConfigValueList.cbegin(),
                              Kasten::DeflateStreamCompressorSettings::formatConfigValueList.cend(),
                              entry);
    if (it == Kasten::DeflateStreamCompressorSettings::formatConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(Kasten::DeflateStreamCompressorSettings::formatConfigValueList.cbegin(), it);
    return static_cast<Kasten::DeflateStreamCompressorSettings::FormatId>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::DeflateStreamCompressorSettings::FormatId &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, Kasten::DeflateStreamCompressorSettings::formatConfigValueList[listIndex], flags);
}


namespace Kasten {

DeflateStreamCompressorSettings::DeflateStreamCompressorSettings() = default;

bool DeflateStreamCompressorSettings::operator==(const DeflateStreamCompressorSettings& other) const
{
    return
        (compressionLevel == other.compressionLevel) &&
        (strategyId == other.strategyId) &&
        (formatId == other.formatId);
}

void DeflateStreamCompressorSettings::loadConfig(const KConfigGroup& configGroup)
{
    compressionLevel = configGroup.readEntry(CompressionLevelConfigKey, DefaultCompressionLevel);
    if ((compressionLevel < -1) || (9 < compressionLevel)) {
        compressionLevel = DefaultCompressionLevel;
    }
    strategyId = configGroup.readEntry(StrategyConfigKey, DefaultStrategy);
    formatId = configGroup.readEntry(FormatConfigKey, DefaultFormat);
}

void DeflateStreamCompressorSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(CompressionLevelConfigKey, compressionLevel);
    configGroup.writeEntry(StrategyConfigKey, strategyId);
    configGroup.writeEntry(FormatConfigKey, formatId);
}

ByteArrayDeflateStreamCompressor::ByteArrayDeflateStreamCompressor()
    : AbstractByteArrayStreamCompressor(i18nc("name of the encoding target", "deflate"), QStringLiteral("application/zlib"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.loadConfig(configGroup);
}

ByteArrayDeflateStreamCompressor::~ByteArrayDeflateStreamCompressor() = default;

void ByteArrayDeflateStreamCompressor::setSettings(const DeflateStreamCompressorSettings& settings)
{
    if (m_settings == settings) {
        return;
    }

    m_settings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

int zlibStrategy(DeflateStreamCompressorSettings::StrategyId strategyId)
{
    int listIndex = static_cast<int>(strategyId);
    if ((listIndex < 0) || (listIndex >= DeflateStreamCompressorSettings::StrategyCount)) {
        listIndex = 0;
    }
    static const std::array<int, DeflateStreamCompressorSettings::StrategyCount> zLibByIdTable = {
        Z_DEFAULT_STRATEGY,
        Z_FILTERED,
        Z_HUFFMAN_ONLY,
        Z_RLE,
        Z_FIXED,
    };
    return zLibByIdTable[listIndex];
}

std::unique_ptr<AbstractCompressionStream> ByteArrayDeflateStreamCompressor::createCompressionStream(AbstractByteArrayOutput* byteArrayOutput)
{
    const bool createRaw = (m_settings.formatId == DeflateStreamCompressorSettings::FormatId::Raw);
    auto compressionStream = std::make_unique<DeflateCompressionStream>(byteArrayOutput, m_settings.compressionLevel, zlibStrategy(m_settings.strategyId), createRaw);
    return compressionStream;
}

}

#include "moc_bytearraydeflatestreamcompressor.cpp"
