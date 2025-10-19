/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYXZSTREAMCOMPRESSOR_HPP
#define KASTEN_BYTEARRAYXZSTREAMCOMPRESSOR_HPP

// lib
#include "abstractbytearraystreamcompressor.hpp"

class KConfigGroup;

namespace Kasten {

class XzStreamCompressorSettings
{
public:
    enum class CompressionPresetLevelVariantId
    {
        Normal = 0,
        Extreme = 1,
        _Count,
    };

    static constexpr auto CompressionPresetLevelVariantCount = static_cast<std::size_t>(CompressionPresetLevelVariantId::_Count);
    static const std::array<QString, CompressionPresetLevelVariantCount> compressionPresetLevelVariantConfigValueList;

    enum class IntegrityCheckId
    {
        None = 0,
        Crc32 = 1,
        Crc64 = 2,
        Sha256 = 3,
        _Count,
    };

    static constexpr auto IntegrityCheckCount = static_cast<std::size_t>(IntegrityCheckId::_Count);
    static const std::array<QString, IntegrityCheckCount> integrityCheckConfigValueList;

private:
    static constexpr char CompressionPresetLevelConfigKey[] = "CompressionPresetLevel";
    static constexpr char CompressionPresetLevelVariantConfigKey[] = "CompressionPresetLevelVariant";
    static constexpr char IntegrityCheckConfigKey[] = "IntegrityCheck";

    static constexpr int DefaultCompressionPresetLevel = 6;
    static constexpr CompressionPresetLevelVariantId DefaultCompressionPresetLevelVariant = CompressionPresetLevelVariantId::Normal;
    static constexpr IntegrityCheckId DefaultIntegrityCheck = IntegrityCheckId::Crc64;

public:
    XzStreamCompressorSettings();
    XzStreamCompressorSettings(const XzStreamCompressorSettings&) = default;
    XzStreamCompressorSettings(XzStreamCompressorSettings&&) = default;

    ~XzStreamCompressorSettings() = default;

    XzStreamCompressorSettings& operator=(const XzStreamCompressorSettings&) = default;
    XzStreamCompressorSettings& operator=(XzStreamCompressorSettings&&) = default;

    [[nodiscard]]
    bool operator==(const XzStreamCompressorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    int compressionPresetLevel = DefaultCompressionPresetLevel;
    CompressionPresetLevelVariantId compressionPresetLevelVariantId = DefaultCompressionPresetLevelVariant;
    IntegrityCheckId integrityCheckId = DefaultIntegrityCheck;
};

class ByteArrayXzStreamCompressor : public AbstractByteArrayStreamCompressor
{
    Q_OBJECT

private:
    static constexpr char ConfigGroupId[] = "ByteArrayXzStreamCompressor";

public:
    ByteArrayXzStreamCompressor();
    ~ByteArrayXzStreamCompressor() override;

public:
    [[nodiscard]]
    XzStreamCompressorSettings settings() const;
    void setSettings(const XzStreamCompressorSettings& settings);

protected: // AbstractByteArrayStreamCompressor API
    [[nodiscard]]
    std::unique_ptr<AbstractCompressionStream> createCompressionStream(AbstractByteArrayOutput* byteArrayOutput) override;

private:
    XzStreamCompressorSettings m_settings;
};

inline XzStreamCompressorSettings ByteArrayXzStreamCompressor::settings() const { return m_settings; }

}

#endif
