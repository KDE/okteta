/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BZIP2COMPRESSIONSTREAM_HPP
#define KASTEN_BZIP2COMPRESSIONSTREAM_HPP

// lib
#include "abstractcompressionstream.hpp"
// bzip2
#define BZ_NO_STDIO // limiting API to memory-only
#include <bzlib.h>
// Std
#include <array>

class KLocalizedString;

namespace Kasten {

class BZip2CompressionStream : public AbstractCompressionStream
{
private:
    static constexpr std::size_t OutputBufferSize = 16 * 1024;

public:
    explicit BZip2CompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                    int blockSize = 5);
    ~BZip2CompressionStream();

private:
    void setBZip2Error(const KLocalizedString& errorMessage, int bzip2ErrorCode);

protected: // AbstractCompressionStream API
    void setInputBuffer(const char* buffer, std::size_t bufferSize) override;

    [[nodiscard]]
    bool compress(CompressMode compressMode) override;

private:
    std::array<char, OutputBufferSize> m_outputBuffer;

    ::bz_stream m_bzStream;
};

}

#endif