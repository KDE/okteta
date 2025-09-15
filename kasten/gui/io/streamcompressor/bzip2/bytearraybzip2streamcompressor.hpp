/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBZIP2STREAMCOMPRESSOR_HPP
#define KASTEN_BYTEARRAYBZIP2STREAMCOMPRESSOR_HPP

// lib
#include "abstractbytearraystreamcompressor.hpp"

class KConfigGroup;

namespace Kasten {

class BZip2StreamCompressorSettings
{
private:
    static inline constexpr char BlockSizeConfigKey[] = "BlockSize";

    static inline constexpr int DefaultBlockSize = 5;

public:
    BZip2StreamCompressorSettings();
    BZip2StreamCompressorSettings(const BZip2StreamCompressorSettings&) = default;
    BZip2StreamCompressorSettings(BZip2StreamCompressorSettings&&) = default;

    ~BZip2StreamCompressorSettings() = default;

    BZip2StreamCompressorSettings& operator=(const BZip2StreamCompressorSettings&) = default;
    BZip2StreamCompressorSettings& operator=(BZip2StreamCompressorSettings&&) = default;

    [[nodiscard]]
    bool operator==(const BZip2StreamCompressorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    /// 1..9
    int blockSize = DefaultBlockSize;
};

class ByteArrayBZip2StreamCompressor : public AbstractByteArrayStreamCompressor
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ByteArrayBZip2StreamCompressor";

public:
    ByteArrayBZip2StreamCompressor();
    ~ByteArrayBZip2StreamCompressor() override;

public:
    [[nodiscard]]
    BZip2StreamCompressorSettings settings() const;
    void setSettings(const BZip2StreamCompressorSettings& settings);

protected: // AbstractByteArrayStreamCompressor API
    [[nodiscard]]
    std::unique_ptr<AbstractCompressionStream> createCompressionStream(AbstractByteArrayOutput* byteArrayOutput) override;

private:
    BZip2StreamCompressorSettings m_settings;
};

inline BZip2StreamCompressorSettings ByteArrayBZip2StreamCompressor::settings() const { return m_settings; }

}

#endif
