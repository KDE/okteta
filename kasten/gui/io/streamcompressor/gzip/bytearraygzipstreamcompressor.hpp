/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYGZIPSTREAMCOMPRESSOR_HPP
#define KASTEN_BYTEARRAYGZIPSTREAMCOMPRESSOR_HPP

// lib
#include "abstractbytearraystreamcompressor.hpp"
// Qt
#include <QString>
// Std
#include <array>

class KConfigGroup;

namespace Kasten {

class GZipStreamCompressorSettings
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

private:
    static inline constexpr char CompressionLevelConfigKey[] = "CompressionLevel";
    static inline constexpr char StrategyConfigKey[] = "Strategy";
    static inline constexpr char FileNameConfigKey[] = "FileName";
    static inline constexpr char CommentConfigKey[] = "Comment";

    static inline constexpr int DefaultCompressionLevel = -1;
    static inline constexpr StrategyId DefaultStrategy = StrategyId::Default;

public:
    GZipStreamCompressorSettings();
    GZipStreamCompressorSettings(const GZipStreamCompressorSettings&) = default;
    GZipStreamCompressorSettings(GZipStreamCompressorSettings&&) = default;

    ~GZipStreamCompressorSettings() = default;

    GZipStreamCompressorSettings& operator=(const GZipStreamCompressorSettings&) = default;
    GZipStreamCompressorSettings& operator=(GZipStreamCompressorSettings&&) = default;

    [[nodiscard]]
    bool operator==(const GZipStreamCompressorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    int compressionLevel = DefaultCompressionLevel;
    StrategyId strategyId = DefaultStrategy;
    QString fileName;
    QString comment;
};

class ByteArrayGZipStreamCompressor : public AbstractByteArrayStreamCompressor
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ByteArrayGZipStreamCompressor";

public:
    ByteArrayGZipStreamCompressor();
    ~ByteArrayGZipStreamCompressor() override;

public:
    [[nodiscard]]
    GZipStreamCompressorSettings settings() const;
    void setSettings(const GZipStreamCompressorSettings& settings);

protected: // AbstractByteArrayStreamCompressor API
    [[nodiscard]]
    std::unique_ptr<AbstractCompressionStream> createCompressionStream(AbstractByteArrayOutput* byteArrayOutput) override;

private:
    GZipStreamCompressorSettings m_settings;
};

inline GZipStreamCompressorSettings ByteArrayGZipStreamCompressor::settings() const { return m_settings; }

}

#endif
