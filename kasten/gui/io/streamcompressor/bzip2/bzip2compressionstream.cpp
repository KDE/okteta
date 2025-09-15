/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bzip2compressionstream.hpp"

// lib
#include "abstractbytearrayoutput.hpp"
#include <logging.hpp>
// KF
#include <KLocalizedString>

namespace Kasten {

BZip2CompressionStream::BZip2CompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                               int blockSize)
    : AbstractCompressionStream(byteArrayOutput)
{
    // no custom memory allocations
    m_bzStream.bzalloc = nullptr;
    m_bzStream.bzfree = nullptr;
    m_bzStream.opaque = nullptr;

    constexpr int verboseMode = 0; // silent
    constexpr int workFactor = 0; // default

    const int compressInitResult = ::BZ2_bzCompressInit(&m_bzStream, blockSize, verboseMode, workFactor);

    // Handle error.
    if (compressInitResult != BZ_OK) {
        setBZip2Error(ki18n("Internal bzip2 error: %1"), compressInitResult);
    }
}

BZip2CompressionStream::~BZip2CompressionStream()
{
    // TODO: how to deal with NotYetWritten?
    std::ignore = finish();

    std::ignore = ::BZ2_bzCompressEnd(&m_bzStream);
}

void BZip2CompressionStream::setBZip2Error(const KLocalizedString& errorMessage, int bzip2ErrorCode)
{
    const QString argument = i18n("Error code %1", bzip2ErrorCode);

    setError(errorMessage.subs(argument).toString());
}

bool BZip2CompressionStream::compress(CompressMode compressMode)
{
    AbstractByteArrayOutput* byteArrayOutput = this->byteArrayOutput();

    int compressResult;
    const int compressAction = (compressMode == FinalCompress) ? BZ_FINISH : BZ_RUN;
    do {
        m_bzStream.next_out = m_outputBuffer.data();
        m_bzStream.avail_out = m_outputBuffer.size();

        compressResult = ::BZ2_bzCompress(&m_bzStream, compressAction);

        if (((compressMode == OngoingCompress) && (compressResult != BZ_RUN_OK)) ||
            ((compressMode == FinalCompress) && (compressResult != BZ_FINISH_OK) && (compressResult != BZ_STREAM_END))) {
            setBZip2Error(ki18n("Internal zlib error when compressing: %1"), compressResult);
            return false;
        }

        const int usedOutputSize = m_outputBuffer.size() - m_bzStream.avail_out;

        const bool isWritten = byteArrayOutput->writeData(m_outputBuffer.data(), usedOutputSize);

        if (!isWritten) {
            setError(i18n("Error writing to output: %1", byteArrayOutput->errorString()));

            return false;
        }
    } while (m_bzStream.avail_out == 0); //  as long as the full output buffer is used

    const int expectedDeflateResult = (compressMode == FinalCompress) ? BZ_STREAM_END : BZ_RUN_OK;
    return (compressResult == expectedDeflateResult);
}

void BZip2CompressionStream::setInputBuffer(const char* buffer, std::size_t bufferSize)
{
    m_bzStream.next_in = const_cast<char*>(buffer);
    m_bzStream.avail_in = bufferSize;
}

}
