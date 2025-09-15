/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "deflatecompressionstream.hpp"

// lib
#include "abstractbytearrayoutput.hpp"
#include <logging.hpp>
// KF
#include <KLocalizedString>

namespace Kasten {

DeflateCompressionStream::DeflateCompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                                   int compressionLevel, int strategy, bool isRaw)
    : AbstractCompressionStream(byteArrayOutput)
{
    // no custom memory allocations
    m_zlibStream.zalloc = static_cast<::alloc_func>(nullptr);
    m_zlibStream.zfree = static_cast<::free_func>(nullptr);
    m_zlibStream.opaque = static_cast<::voidpf>(nullptr);

    const int windowBits = isRaw ? -MAX_WBITS : MAX_WBITS;
    constexpr int memoryLevel = 8; // use maximum memory for optimal speed, default

    const int deflateInitResult = ::deflateInit2(&m_zlibStream, compressionLevel, Z_DEFLATED, windowBits, memoryLevel, strategy);

    if (deflateInitResult != Z_OK) {
        setZlibError(ki18n("Internal zlib error: %1"), deflateInitResult);
    }
}

DeflateCompressionStream::~DeflateCompressionStream()
{
    // TODO: how to deal with NotYetWritten?
    std::ignore = finish();

    std::ignore = ::deflateEnd(&m_zlibStream);
}

void DeflateCompressionStream::setZlibError(const KLocalizedString& errorMessage, int zlibErrorCode)
{
    const char* const zlibErrorString = ::zError(zlibErrorCode);

    const QString argument = zlibErrorString ?
        QString::fromLocal8Bit(zlibErrorString) :
        i18n("Unknown zlib error, code %1", zlibErrorCode);

    setError(errorMessage.subs(argument).toString());
}

bool DeflateCompressionStream::compress(CompressMode compressMode)
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
                return deflateResult;
            }
        }

        const int usedOutputSize = m_outputBuffer.size() - m_zlibStream.avail_out;

        const bool isWritten = byteArrayOutput->writeData(reinterpret_cast<char*>(m_outputBuffer.data()), usedOutputSize);

        if (!isWritten) {
            setError(i18n("Error writing to output: %1", byteArrayOutput->errorString()));

            return Z_ERRNO;
        }
    } while (m_zlibStream.avail_out == 0); //  as long as the full output buffer is used

    const int expectedDeflateResult = (compressMode == FinalCompress) ? Z_STREAM_END :Z_OK;
    return (deflateResult == expectedDeflateResult);
}

void DeflateCompressionStream::setInputBuffer(const char* buffer, std::size_t bufferSize)
{
    m_zlibStream.next_in = reinterpret_cast<::Bytef*>(const_cast<char*>(buffer));
    m_zlibStream.avail_in = bufferSize;
}

}
