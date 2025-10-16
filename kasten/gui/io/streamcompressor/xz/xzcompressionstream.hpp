/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_XZCOMPRESSIONSTREAM_HPP
#define KASTEN_XZCOMPRESSIONSTREAM_HPP

// lib
#include "abstractcompressionstream.hpp"
// lzma
#include <lzma.h>
// Std
#include <array>

class KLocalizedString;

namespace Kasten {

class XzCompressionStream : public AbstractCompressionStream
{
private:
    static inline constexpr std::size_t OutputBufferSize = 16 * 1024;

public:
    explicit XzCompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                 int compressionPresetLevel = LZMA_PRESET_DEFAULT,
                                 bool useExtremeLevel = false,
                                 ::lzma_check check = ::LZMA_CHECK_CRC64);
    ~XzCompressionStream() override;

protected: // AbstractCompressionStream API
    void setInputBuffer(const char* buffer, std::size_t bufferSize) override;

    [[nodiscard]]
    bool compress(CompressMode compressMode) override;

private:
    void setLzmaError(const KLocalizedString& errorMessage, ::lzma_ret returnCode);

private:
    std::array<uint8_t, OutputBufferSize> m_outputBuffer;

    ::lzma_stream m_lzmaStream = LZMA_STREAM_INIT;
};

}

#endif
