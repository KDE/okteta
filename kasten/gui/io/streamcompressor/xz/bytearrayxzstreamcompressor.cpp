/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayxzstreamcompressor.hpp"

// lib
#include "xzcompressionstream.hpp"
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Std
#include <algorithm>
#include <iterator>

const std::array<QString, Kasten::XzStreamCompressorSettings::CompressionPresetLevelVariantCount>
Kasten::XzStreamCompressorSettings::compressionPresetLevelVariantConfigValueList = {
    QStringLiteral("Normal"),
    QStringLiteral("Extreme"),
};

template <>
inline Kasten::XzStreamCompressorSettings::CompressionPresetLevelVariantId KConfigGroup::readEntry(const char *key,
                                                                                                   const Kasten::XzStreamCompressorSettings::CompressionPresetLevelVariantId &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(Kasten::XzStreamCompressorSettings::compressionPresetLevelVariantConfigValueList.cbegin(),
                        Kasten::XzStreamCompressorSettings::compressionPresetLevelVariantConfigValueList.cend(),
                        entry);
    if (it == Kasten::XzStreamCompressorSettings::compressionPresetLevelVariantConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(Kasten::XzStreamCompressorSettings::compressionPresetLevelVariantConfigValueList.cbegin(), it);
    return static_cast<Kasten::XzStreamCompressorSettings::CompressionPresetLevelVariantId>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::XzStreamCompressorSettings::CompressionPresetLevelVariantId &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, Kasten::XzStreamCompressorSettings::compressionPresetLevelVariantConfigValueList[listIndex], flags);
}

const std::array<QString, Kasten::XzStreamCompressorSettings::IntegrityCheckCount>
Kasten::XzStreamCompressorSettings::integrityCheckConfigValueList = {
    QStringLiteral("None"),
    QStringLiteral("CRC32"),
    QStringLiteral("CRC64"),
    QStringLiteral("SHA-256"),
};

template <>
inline Kasten::XzStreamCompressorSettings::IntegrityCheckId KConfigGroup::readEntry(const char *key,
                                                                                    const Kasten::XzStreamCompressorSettings::IntegrityCheckId &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(Kasten::XzStreamCompressorSettings::integrityCheckConfigValueList.cbegin(),
                        Kasten::XzStreamCompressorSettings::integrityCheckConfigValueList.cend(),
                        entry);
    if (it == Kasten::XzStreamCompressorSettings::integrityCheckConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(Kasten::XzStreamCompressorSettings::integrityCheckConfigValueList.cbegin(), it);
    return static_cast<Kasten::XzStreamCompressorSettings::IntegrityCheckId>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::XzStreamCompressorSettings::IntegrityCheckId &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, Kasten::XzStreamCompressorSettings::integrityCheckConfigValueList[listIndex], flags);
}


namespace Kasten {

XzStreamCompressorSettings::XzStreamCompressorSettings() = default;

bool XzStreamCompressorSettings::operator==(const XzStreamCompressorSettings& other) const
{
    return
        (compressionPresetLevel == other.compressionPresetLevel) &&
        (compressionPresetLevelVariantId == other.compressionPresetLevelVariantId) &&
        (integrityCheckId == other.integrityCheckId);
}

void XzStreamCompressorSettings::loadConfig(const KConfigGroup& configGroup)
{
    compressionPresetLevel = configGroup.readEntry(CompressionPresetLevelConfigKey, DefaultCompressionPresetLevel);
    if ((compressionPresetLevel < 0) || (9 < compressionPresetLevel)) {
        compressionPresetLevel = DefaultCompressionPresetLevel;
    }
    compressionPresetLevelVariantId = configGroup.readEntry(CompressionPresetLevelVariantConfigKey, DefaultCompressionPresetLevelVariant);
    integrityCheckId = configGroup.readEntry(IntegrityCheckConfigKey, DefaultIntegrityCheck);
}

void XzStreamCompressorSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(CompressionPresetLevelConfigKey, compressionPresetLevel);
    configGroup.writeEntry(CompressionPresetLevelVariantConfigKey, compressionPresetLevelVariantId);
    configGroup.writeEntry(IntegrityCheckConfigKey, integrityCheckId);
}

ByteArrayXzStreamCompressor::ByteArrayXzStreamCompressor()
    : AbstractByteArrayStreamCompressor(i18nc("name of the encoding target", "xz"), QStringLiteral("application/x-xz"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.loadConfig(configGroup);
}

ByteArrayXzStreamCompressor::~ByteArrayXzStreamCompressor() = default;

void ByteArrayXzStreamCompressor::setSettings(const XzStreamCompressorSettings& settings)
{
    if (m_settings == settings) {
        return;
    }

    m_settings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    m_settings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

::lzma_check lzmaCheck(XzStreamCompressorSettings::IntegrityCheckId integrityCheckId)
{
    int listIndex = static_cast<int>(integrityCheckId);
    if ((listIndex < 0) || (listIndex > XzStreamCompressorSettings::IntegrityCheckCount)) {
        listIndex = 0;
    }
    static const std::array<::lzma_check, XzStreamCompressorSettings::IntegrityCheckCount> lzmaByIdTable = {
        LZMA_CHECK_NONE,
        LZMA_CHECK_CRC32,
        LZMA_CHECK_CRC64,
        LZMA_CHECK_SHA256,
    };
    return lzmaByIdTable[listIndex];
}

std::unique_ptr<AbstractCompressionStream> ByteArrayXzStreamCompressor::createCompressionStream(AbstractByteArrayOutput* byteArrayOutput)
{
    const bool useExtreme = (m_settings.compressionPresetLevelVariantId == XzStreamCompressorSettings::CompressionPresetLevelVariantId::Extreme);
    const ::lzma_check check = lzmaCheck(m_settings.integrityCheckId);
    return std::make_unique<XzCompressionStream>(byteArrayOutput, m_settings.compressionPresetLevel, useExtreme, check);
}

}

#include "moc_bytearrayxzstreamcompressor.cpp"
