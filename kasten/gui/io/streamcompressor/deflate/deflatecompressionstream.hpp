/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DEFLATECOMPRESSIONSTREAM_HPP
#define KASTEN_DEFLATECOMPRESSIONSTREAM_HPP

// lib
#include "abstractcompressionstream.hpp"
// zlib
#include <zlib.h>
// Std
#include <array>

class KLocalizedString;

namespace Kasten {

class DeflateCompressionStream : public AbstractCompressionStream
{
private:
    static constexpr std::size_t OutputBufferSize = 16 * 1024;

public:
    explicit DeflateCompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                      int compressionLevel = Z_DEFAULT_COMPRESSION,
                                      int strategy = Z_DEFAULT_STRATEGY,
                                      bool isRaw = false);
    ~DeflateCompressionStream() override;

protected: // AbstractCompressionStream API
    void setInputBuffer(const char* buffer, std::size_t bufferSize) override;

    [[nodiscard]]
    bool compress(CompressMode compressMode) override;

private:
    void setZlibError(const KLocalizedString& errorMessage, int zlibErrorCode);

private:
    std::array<::Bytef, OutputBufferSize> m_outputBuffer;

    ::z_stream m_zlibStream;
};

}

#endif