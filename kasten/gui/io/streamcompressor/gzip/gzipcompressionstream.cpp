/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "gzipcompressionstream.hpp"

// lib
#include "abstractbytearrayoutput.hpp"
#include <logging.hpp>
// KF
#include <KLocalizedString>
// Std
#include <ctime>

namespace Kasten {

GZipCompressionStream::GZipCompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                             int compressionLevel, int strategy)
    : AbstractCompressionStream(byteArrayOutput)
{
    // no custom memory allocations
    m_zlibStream.zalloc = static_cast<::alloc_func>(nullptr);
    m_zlibStream.zfree = static_cast<::free_func>(nullptr);
    m_zlibStream.opaque = static_cast<::voidpf>(nullptr);

    constexpr int windowBits = MAX_WBITS + (1 << 4) /* gzip flag */;
    constexpr int memoryLevel = 8; // use maximum memory for optimal speed, default

    const int deflateInitResult = ::deflateInit2(&m_zlibStream, compressionLevel, Z_DEFLATED, windowBits, memoryLevel, strategy);

    if (deflateInitResult != Z_OK) {
        setZlibError(ki18n("Internal zlib error: %1"), deflateInitResult);
    }
}

GZipCompressionStream::~GZipCompressionStream()
{
    // TODO: how to deal with NotYetWritten?
    std::ignore = finish();

    std::ignore = ::deflateEnd(&m_zlibStream);
}

void GZipCompressionStream::setZlibError(const KLocalizedString& errorMessage, int zlibErrorCode)
{
    const char* const zlibErrorString = ::zError(zlibErrorCode);

    const QString argument = zlibErrorString ?
        QString::fromLocal8Bit(zlibErrorString) :
        i18n("Unknown zlib error, code %1", zlibErrorCode);

    setError(errorMessage.subs(argument).toString());
}

bool GZipCompressionStream::compress(CompressMode compressMode)
{
    AbstractByteArrayOutput* byteArrayOutput = this->byteArrayOutput();

    int deflateResult;
    const int flush = (compressMode == FinalCompress) ? Z_FINISH : Z_NO_FLUSH;
    do {
        m_zlibStream.next_out = m_outputBuffer.data();
        m_zlibStream.avail_out = m_outputBuffer.size();

        deflateResult = ::deflate(&m_zlibStream, flush);

        if (deflateResult != Z_OK) {
            if ((compressMode == FinalCompress) && (deflateResult != Z_STREAM_END)) {
                setZlibError(ki18n("Internal zlib error when compressing: %1"), deflateResult);
                return false;
            }
        }

        const int usedOutputSize = m_outputBuffer.size() - m_zlibStream.avail_out;

        const bool isWritten = byteArrayOutput->writeData(reinterpret_cast<char*>(m_outputBuffer.data()), usedOutputSize);

        if (!isWritten) {
            setError(i18n("Error writing to output: %1", byteArrayOutput->errorString()));

            return false;
        }
    } while (m_zlibStream.avail_out == 0); //  as long as the full output buffer is used

    const int expectedDeflateResult = (compressMode == FinalCompress) ? Z_STREAM_END :Z_OK;
    return (deflateResult == expectedDeflateResult);
}

bool GZipCompressionStream::writeHeader()
{
    if (state() != NotYetWritten) {
        return false;
    }

    // header info: text, time, os, extra field, name, comment 
    // reset whole data structure
    ::memset(&m_gzipHeader, 0, sizeof(::gz_header));
    // must consist of ISO 8859-1 characters;
    m_gzipHeader.name = reinterpret_cast<::Bytef*>(const_cast<char*>(m_fileName.data())); 
    // must consist of ISO 8859-1characters. 
    // Line breaks should be denoted by a single line feed character.
    m_gzipHeader.comment = reinterpret_cast<::Bytef*>(const_cast<char*>(m_comment.data())); //nullptr;
    // TODO: support user specifying also time, os, extra field
    m_gzipHeader.extra = nullptr; // unused
    m_gzipHeader.time = ::time(nullptr);
    m_gzipHeader.os = 0x03; // always use Unix for now TODO: find way to query zlib for it

    const int deflateSetHeaderResult = ::deflateSetHeader(&m_zlibStream, &m_gzipHeader);
    if (deflateSetHeaderResult != Z_OK) {
        setZlibError(ki18n("Internal zlib error: %1"), deflateSetHeaderResult);
        return false;
    }

    return true;
}

void GZipCompressionStream::setInputBuffer(const char* buffer, std::size_t bufferSize)
{
    m_zlibStream.next_in = reinterpret_cast<::Bytef*>(const_cast<char*>(buffer));
    m_zlibStream.avail_in = bufferSize;
}

}
