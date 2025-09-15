/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYZSTDSTREAMCOMPRESSOR_HPP
#define KASTEN_BYTEARRAYZSTDSTREAMCOMPRESSOR_HPP

// lib
#include <abstractbytearraystreamcompressor.hpp>

class KConfigGroup;

namespace Kasten {

class ZStdStreamCompressorSettings
{
private:
    static inline constexpr char CompressionLevelConfigKey[] = "CompressionLevel";
    static inline constexpr char WriteFrameContentSizeConfigKey[] = "WriteFrameContentSize";
    static inline constexpr char WriteFrameChecksumConfigKey[] = "WriteFrameChecksum";

    static inline constexpr int DefaultCompressionLevel = 0;
    static inline constexpr bool DefaultWriteFrameContentSize = true;
    static inline constexpr bool DefaultWriteFrameChecksum = false;

public:
    ZStdStreamCompressorSettings();
    ZStdStreamCompressorSettings(const ZStdStreamCompressorSettings&) = default;
    ZStdStreamCompressorSettings(ZStdStreamCompressorSettings&&) = default;

    ~ZStdStreamCompressorSettings() = default;

    ZStdStreamCompressorSettings& operator=(const ZStdStreamCompressorSettings&) = default;
    ZStdStreamCompressorSettings& operator=(ZStdStreamCompressorSettings&&) = default;

    [[nodiscard]]
    bool operator==(const ZStdStreamCompressorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    int compressionLevel = DefaultCompressionLevel;
    bool writeFrameContentSize = DefaultWriteFrameContentSize;
    bool writeFrameChecksum = DefaultWriteFrameChecksum;
};

class ByteArrayZStdStreamCompressor : public AbstractByteArrayStreamCompressor
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ByteArrayZStdStreamCompressor";

public:
    ByteArrayZStdStreamCompressor();
    ~ByteArrayZStdStreamCompressor() override;

public:
    [[nodiscard]]
    ZStdStreamCompressorSettings settings() const;
    void setSettings(const ZStdStreamCompressorSettings& settings);

protected: // AbstractByteArrayStreamCompressor API
    [[nodiscard]]
    std::unique_ptr<AbstractCompressionStream> createCompressionStream(AbstractByteArrayOutput* byteArrayOutput) override;

private:
    ZStdStreamCompressorSettings m_settings;
};

inline ZStdStreamCompressorSettings ByteArrayZStdStreamCompressor::settings() const { return m_settings; }

}

#endif
