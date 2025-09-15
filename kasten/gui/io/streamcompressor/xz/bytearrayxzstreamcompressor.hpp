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

    static inline constexpr int CompressionPresetLevelVariantCount = static_cast<int>(CompressionPresetLevelVariantId::_Count);
    static const std::array<QString, CompressionPresetLevelVariantCount> compressionPresetLevelVariantConfigValueList;

    enum class IntegrityCheckId
    {
        None = 0,
        Crc32 = 1,
        Crc64 = 2,
        Sha256 = 3,
        _Count,
    };

    static inline constexpr int IntegrityCheckCount = static_cast<int>(IntegrityCheckId::_Count);
    static const std::array<QString, IntegrityCheckCount> integrityCheckConfigValueList;

private:
    static inline constexpr char CompressionPresetLevelConfigKey[] = "CompressionPresetLevel";
    static inline constexpr char CompressionPresetLevelVariantConfigKey[] = "CompressionPresetLevelVariant";
    static inline constexpr char IntegrityCheckConfigKey[] = "IntegrityCheck";

    static inline constexpr int DefaultCompressionPresetLevel = 6;
    static inline constexpr CompressionPresetLevelVariantId DefaultCompressionPresetLevelVariant = CompressionPresetLevelVariantId::Normal;
    static inline constexpr IntegrityCheckId DefaultIntegrityCheck = IntegrityCheckId::Crc64;

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
    static inline constexpr char ConfigGroupId[] = "ByteArrayXzStreamCompressor";

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
