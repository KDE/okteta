/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_GZIPCOMPRESSIONSTREAM_HPP
#define KASTEN_GZIPCOMPRESSIONSTREAM_HPP

// lib
#include "abstractcompressionstream.hpp"
// zlib
#include <zlib.h>
// Std
#include <array>
#include <string>

class KLocalizedString;

namespace Kasten {

class GZipCompressionStream : public AbstractCompressionStream
{
private:
    static inline constexpr int OutputBufferSize = 16 * 1024;

public:
    explicit GZipCompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                   int compressionLevel = Z_DEFAULT_COMPRESSION,
                                   int strategy = Z_DEFAULT_STRATEGY);
    ~GZipCompressionStream() override;

public:
    void setFileName(std::string&& fileName);
    void setComment(std::string&& comment);

public: // AbstractCompressionStream API
    [[nodiscard]]
    bool writeHeader() override;

protected: // AbstractCompressionStream API
    void setInputBuffer(const char* buffer, std::size_t bufferSize) override;

    [[nodiscard]]
    bool compress(CompressMode compressMode) override;

private:
    void setZlibError(const KLocalizedString& errorMessage, int zlibErrorCode);

private:
    std::array<::Bytef, OutputBufferSize> m_outputBuffer;

    std::string m_fileName;
    std::string m_comment;

    ::gz_header m_gzipHeader;

    ::z_stream m_zlibStream;
};

inline void GZipCompressionStream::setFileName(std::string&& fileName)
{
    m_fileName = fileName;
}

inline void GZipCompressionStream::setComment(std::string&& comment)
{
    m_comment = comment;
}

}

#endif