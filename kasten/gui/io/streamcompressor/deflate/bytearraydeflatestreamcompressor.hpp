/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYDEFLATESTREAMCOMPRESSOR_HPP
#define KASTEN_BYTEARRAYDEFLATESTREAMCOMPRESSOR_HPP

// lib
#include "abstractbytearraystreamcompressor.hpp"

class KConfigGroup;

namespace Kasten {

class DeflateStreamCompressorSettings
{
public:
    enum class StrategyId
    {
        Default = 0,
        Filter = 1,
        HuffmanEncodingOnly = 2,
        RunLengthEncoding = 3,
        FixedHuffmanEncoding = 4,
        _Count,
    };

    static inline constexpr int StrategyCount = static_cast<int>(StrategyId::_Count);
    static const std::array<QString, StrategyCount> strategyConfigValueList;

    enum class FormatId
    {
        Raw = 0,
        Full = 1,
        _Count,
    };

    static inline constexpr int FormatCount = static_cast<int>(FormatId::_Count);
    static const std::array<QString, FormatCount> formatConfigValueList;

private:
    static inline constexpr char CompressionLevelConfigKey[] = "CompressionLevel";
    static inline constexpr char StrategyConfigKey[] = "Strategy";
    static inline constexpr char FormatConfigKey[] = "Format";

    static inline constexpr int DefaultCompressionLevel = -1;
    static inline constexpr StrategyId DefaultStrategy = StrategyId::Default;
    static inline constexpr FormatId DefaultFormat = FormatId::Full;

public:
    DeflateStreamCompressorSettings();
    DeflateStreamCompressorSettings(const DeflateStreamCompressorSettings&) = default;
    DeflateStreamCompressorSettings(DeflateStreamCompressorSettings&&) = default;

    ~DeflateStreamCompressorSettings() = default;

    DeflateStreamCompressorSettings& operator=(const DeflateStreamCompressorSettings&) = default;
    DeflateStreamCompressorSettings& operator=(DeflateStreamCompressorSettings&&) = default;

    [[nodiscard]]
    bool operator==(const DeflateStreamCompressorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    int compressionLevel = DefaultCompressionLevel;
    StrategyId strategyId = DefaultStrategy;
    FormatId formatId = DefaultFormat;
};

class ByteArrayDeflateStreamCompressor : public AbstractByteArrayStreamCompressor
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ByteArrayDeflateStreamCompressor";

public:
    ByteArrayDeflateStreamCompressor();
    ~ByteArrayDeflateStreamCompressor() override;

public:
    [[nodiscard]]
    DeflateStreamCompressorSettings settings() const;
    void setSettings(const DeflateStreamCompressorSettings& settings);

protected: // AbstractByteArrayStreamCompressor API
    [[nodiscard]]
    std::unique_ptr<AbstractCompressionStream> createCompressionStream(AbstractByteArrayOutput* byteArrayOutput) override;

private:
    DeflateStreamCompressorSettings m_settings;
};

inline DeflateStreamCompressorSettings ByteArrayDeflateStreamCompressor::settings() const { return m_settings; }

}

#endif
